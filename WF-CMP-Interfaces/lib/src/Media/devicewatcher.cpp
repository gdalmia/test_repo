// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "devicewatcher.h"
#include "devicereader.h"
#include "usbunlocker.h"
#include <Workflow/stringconverter.h>
#include <Trace.h>
#include <tchar.h>
#include <strsafe.h>
#include <string>
#include <functional>
#include <dbt.h>
#include <Windows.h>

namespace Philips { namespace Workflow { namespace Media { 

	// This GUID is for all USB serial host PnP drivers, but you can replace it 
	// with any valid device class guid.
	const GUID WceusbshGUID = { 0x25dbce51, 0x6c8f, 0x4a72,0x8a,0x6d,0xb5,0x4c,0x2b,0x4f,0xc8,0x35 };
	const std::wstring WindowClassName = L"USB DETECTION";

	class DeviceWatcher::Private
	{
	public:
		Private();
		~Private();

		Private(const Private &) = delete;
		Private(Private &&) = delete;
		Private& operator=(const Private &) = delete;
		Private& operator=(Private &&) = delete;

		void start() const;
		void stop() const;
		static std::vector <std::unique_ptr<IUsb>> addExistingNodes();

		std::function<void(const Usb&)> nodeAdded;
		std::function<void(const std::string&) > nodeRemoved;
		
	private:
		
		static bool initWindowClass();
        void messagePump() const;
		static INT_PTR WINAPI winProcCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static bool registerDeviceInterfaceToHwnd(HWND hWnd, HDEVNOTIFY* hDeviceNotify);
		static void processDeviceChanged(const LPARAM &lParam, const HWND &hWnd, const WPARAM &wParam);
		static bool isUsbDevice(const std::wstring& driveLetter, bool isRemoved);
		static Usb getDevice(const std::wstring& driveLetter);
		static std::vector<char> getDriveLetter(DWORD drive_mask);

		HWND _handle;
		HDEVNOTIFY _hDeviceNotify;
	};

	DeviceWatcher::DeviceWatcher(): _private(std::make_unique<Private>())
	{
		TraceScope();
		//Subscribe events
		_private->nodeAdded = [this](const Usb& node)
		{
			if (added)
			{
				added(node);
			}
		};
		_private->nodeRemoved = [this](const std::string& drive)
		{
			if (removed)
			{
				removed(drive);
			}
		};
	}

	DeviceWatcher::~DeviceWatcher()
	{
		TraceScope();
		_private.reset( nullptr);
	}

	void DeviceWatcher::stop() const
	{
		TraceScope();
		_private->stop();
	}

	void DeviceWatcher::start() const
	{
		TraceScope();
		_private->start();
	}

	std::vector<std::unique_ptr<IUsb>> DeviceWatcher::addExistingNodes()
	{
		TraceScope();
		return Private::addExistingNodes();
	}

	DeviceWatcher::Private::Private()
	{
		initWindowClass();
		//WinAPI: CreateWindowEx
		_handle = CreateWindowEx(0, WindowClassName.c_str(), nullptr, NULL, NULL, 0, 
			0, 0, nullptr, nullptr, GetModuleHandle(nullptr), this);
	}

	void DeviceWatcher::Private::start() const
	{
		messagePump();
	}

	void DeviceWatcher::Private::stop() const
	{
		if (_handle)
		{
			PostMessage(_handle, WM_CLOSE, NULL, NULL);
		}
	}

	DeviceWatcher::Private::~Private()
	{
		_handle = nullptr;
		_hDeviceNotify = nullptr;
	}
	
	//Message pump to listen the events in different thread
	void DeviceWatcher::Private::messagePump() const
	{
		MSG message;
		int result;
		//WinAPI: GetMessage,TranslateMessage and DispatchMessage
		while ((result = GetMessage(&message, _handle, 0, 0)) != 0)
		{
			//Window is destroyed
			if(result == -1)
			{
				break;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	/*!
		\brief this function registers Device Notifications
		\param hWnd handle to window
		\param hDeviceNotify device notification handle\
		\return true if success otherwise false
		*/
	bool DeviceWatcher::Private::registerDeviceInterfaceToHwnd(HWND hWnd, HDEVNOTIFY *hDeviceNotify)
	{
		DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
		ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
		NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
		NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
		NotificationFilter.dbcc_classguid = WceusbshGUID;
		*hDeviceNotify = RegisterDeviceNotification(
			hWnd,                       // events recipient
			&NotificationFilter,        // type of device
			DEVICE_NOTIFY_WINDOW_HANDLE // type of recipient handle
		);
		return (nullptr != *hDeviceNotify);
	}

	//The window procedure is responsible for handling all messages that are sent to a window
	INT_PTR WINAPI DeviceWatcher::Private::winProcCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT retVal = 1;
		Private* watcher;
		switch (message)
		{
		case WM_CREATE:
			watcher = reinterpret_cast<Private*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
			DeviceWatcher::Private::registerDeviceInterfaceToHwnd(hWnd, &watcher->_hDeviceNotify);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams));
			break;
		case WM_DEVICECHANGE:
		{
			switch (wParam)
			{
			case DBT_DEVICEARRIVAL:
			case DBT_DEVICEREMOVECOMPLETE:
				processDeviceChanged(lParam, hWnd, wParam);
				break;
			case DBT_DEVNODES_CHANGED:
			default:
				break;
			}
		}
		break;
		case WM_CLOSE:
			watcher = reinterpret_cast<Private*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			UnregisterDeviceNotification(watcher->_hDeviceNotify);
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			break;
		default:
			// Send all other messages on to the default windows handler.
			retVal = DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		return retVal;
	}

	void DeviceWatcher::Private::processDeviceChanged(const LPARAM &lParam, const HWND &hWnd, const WPARAM &wParam)
	{
		const auto lpInfo = reinterpret_cast<PDEV_BROADCAST_HDR>(lParam);
		const auto broadcastInfo = reinterpret_cast<PDEV_BROADCAST_VOLUME>(lParam);
		
		if (DBT_DEVTYP_VOLUME == lpInfo->dbch_devicetype)
		{
			auto driveLetters = getDriveLetter(broadcastInfo->dbcv_unitmask);
			for (const auto& driveLetter : driveLetters) //loop through if multiple partition
			{
				auto drive = std::wstring(1, driveLetter) + L":\\";
				if (isUsbDevice(drive, wParam == DBT_DEVICEREMOVECOMPLETE))
				{
					
					const auto watcher = reinterpret_cast<Private*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
					if (watcher)
					{
						if (wParam == DBT_DEVICEARRIVAL)
						{
							auto node = getDevice(drive);
							watcher->nodeAdded(node);
						}
						if (wParam == DBT_DEVICEREMOVECOMPLETE)
						{
							watcher->nodeRemoved(convert(drive));
						}
					}
				}
			}
		}
	}

	bool DeviceWatcher::Private::initWindowClass()
	{
		WNDCLASSEX wndClass;
		ZeroMemory(&wndClass, sizeof(wndClass));
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.lpfnWndProc = reinterpret_cast<WNDPROC>(DeviceWatcher::Private::winProcCallback);
		wndClass.hInstance = GetModuleHandle(nullptr);
		wndClass.lpszClassName = WindowClassName.c_str();
		return RegisterClassEx(&wndClass) != 0;
	}

	std::wstring trim(const std::wstring& drive)
	{
		auto value = drive;
		value.erase(std::remove(value.begin(), value.end(), L'\\'), value.end());
		return value;
	}
	
	//Gets device details from drive
	Usb DeviceWatcher::Private::getDevice(const std::wstring& drive)
	{
		const auto volumeName = readVolumeName(drive);
		const auto locked = UsbUnlocker::getLockStatus(trim(drive));

		uint64_t totalNumberOfBytes, totalNumberOfFreeBytes;
		readFreeSpace(drive, totalNumberOfBytes, totalNumberOfFreeBytes);
		
		return Usb{
			  convert(drive)
			, convert(volumeName)
			, totalNumberOfBytes
			, totalNumberOfFreeBytes
			, locked };
	}

	//Adds the pre existing nodes connected
	std::vector <std::unique_ptr<IUsb>> DeviceWatcher::Private::addExistingNodes()
	{
		//WinAPI: GetLogicalDrives();
		std::vector <std::unique_ptr<IUsb>> usbs;
		const auto drivesBitMask = GetLogicalDrives();
		auto driveLetters = getDriveLetter(drivesBitMask);

		for (const auto& driveLetter : driveLetters)
		{
			auto drive = std::wstring(1, driveLetter) + L":\\";
			if (isUsbDevice(drive, false))
			{
				auto node = getDevice(drive);
				usbs.push_back(std::make_unique<Usb>(node));
			}
		}
		return usbs;
	}
	   	
	//gets list of drives from drive mask(includes partitioned)
	std::vector<char> DeviceWatcher::Private::getDriveLetter(ULONG drive_mask)
	{
		std::vector<char> driveLetters;
		for (char i = 0; i < 26; ++i)
		{
			if (drive_mask & 0x1)
			{

				driveLetters.push_back('A' + i);
			}
			drive_mask = drive_mask >> 1;
		}
		return driveLetters;
	}

	bool DeviceWatcher::Private::isUsbDevice(const std::wstring& driveLetter, bool isRemoved)
	{
		//WinAPI: GetDriveTypeW
		const auto driveType = GetDriveTypeW(driveLetter.c_str());
		if (isRemoved)
		{
			return (driveType == DRIVE_REMOVABLE || driveType == DRIVE_NO_ROOT_DIR);
		}

		return (driveType == DRIVE_REMOVABLE);
	}
}}}
