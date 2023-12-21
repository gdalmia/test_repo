// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "Workflow/Media/usb.h"
#include "usbunlocker.h"
#include "devicereader.h"
#include "usbeject.h"
#include <Workflow/stringconverter.h>
#include <Trace.h>


namespace Philips { namespace Workflow { namespace Media {

	const std::string defaultUsbName = "USB";
	
	Usb::Usb()
		: _drive("")
		, _volumeName(defaultUsbName)
		, _totalNumberOfBytes(0)
		, _totalNumberOfFreeBytes(0)
		, _locked(false)
	{
	}

	Usb::Usb(const std::string& drive)
		: _drive(drive)
		, _volumeName(defaultUsbName)
		, _totalNumberOfBytes(0)
		, _totalNumberOfFreeBytes(0)
		, _locked(false)
	{
	}

	Usb::Usb(const std::string& drive, const std::string& volumeName, 
		uint64_t totalNumberOfBytes, uint64_t totalNumberOfFreeBytes, bool locked)
		: _drive(drive)
		, _volumeName(!volumeName.empty() ? volumeName: defaultUsbName)
		, _totalNumberOfBytes(totalNumberOfBytes)
		, _totalNumberOfFreeBytes(totalNumberOfFreeBytes)
		, _locked(locked)
	{
	}

	Usb::Usb(const Usb& usb)
	{
		_drive = usb.drive();
		_volumeName = usb.volumeName();
		_totalNumberOfBytes = usb.totalNumberOfBytes();
		_totalNumberOfFreeBytes = usb.totalNumberOfFreeBytes();
		_locked = usb.locked();
	}

	bool Usb::operator==(const Usb& usb)const
	{
		return _drive == usb._drive;
	}

	bool Usb::operator!=(const Usb& usb) const
	{
		return !(*this == usb);
	}

	std::string Usb::drive() const
	{
		return _drive;
	}

	std::string Usb::volumeName() const
	{
		return _volumeName;
	}

	uint64_t Usb::totalNumberOfBytes() const
	{
		return _totalNumberOfBytes;
	}

	uint64_t Usb::totalNumberOfFreeBytes() const
	{
		return _totalNumberOfFreeBytes;
	}

	void Usb::recalculateSize()
	{
		readFreeSpace(_drive, _totalNumberOfBytes, _totalNumberOfFreeBytes);
	}

	bool Usb::locked() const
	{
		return _locked;
	}

	std::wstring trim(const std::string& drive)
	{
		auto value = drive;
		value.erase(std::remove(value.begin(), value.end(), L'\\'), value.end());
		return convert(value);
	}

	bool Usb::unlock(const std::string& password) 
	{
		TraceScope();
		if (_locked)
		{
			const auto result = UsbUnlocker::unLock(trim(_drive), convert(password));
			TraceMessage() << "Unlock of usb device [Volume Name = " << _volumeName << ", Drive=" << _drive << "]"
				<< (result ? "Success" : "Failed");
			if(result)
			{
				const auto volumeName = readVolumeName(_drive);
				_volumeName = volumeName.empty() ? defaultUsbName : volumeName;
				readFreeSpace(_drive, _totalNumberOfBytes, _totalNumberOfFreeBytes);
				changed();
			}
			
			_locked = !result;
			return result;
		}
		return false;
	}

	bool Usb::eject()
	{
		return UsbEject::eject(_drive);
	}
}}}
