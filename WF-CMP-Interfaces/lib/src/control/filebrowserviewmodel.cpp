// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "filebrowserviewmodel.h"
#include "workflow.h"
#include "Workflow/Media/usbmonitor.h"

namespace Philips { namespace Workflow { namespace Control {

	constexpr int invalidIndex = -1;
	constexpr int maxPathLength = 247;

	class FileBrowserViewModel::Singleton
	{
	public:
	static std::shared_ptr<Singleton> instance()
	{
		static std::weak_ptr<Singleton> singleton;

		auto result = singleton.lock();
		if (result) return result;
		result.reset(new Singleton());
		singleton = result;

		return result;		
	}

	static std::shared_ptr<Singleton> instance(const Media::IUsbMonitor& usbMonitor)
	{
		static std::weak_ptr<Singleton> singleton;

		auto result = singleton.lock();
		if (result) return result;
		result.reset(new Singleton(usbMonitor));
		singleton = result;

		return result;
	}
	
	void updateIndex(int index)
	{
		_currIndex = index;
		if (index != invalidIndex)
		{
			if (_currView != View::UsbList)
			updatePath(_folderList.at(_currIndex).generic_wstring());
		}
		else
		{
			updatePath(_rootFolder.generic_wstring());
		}

		changed();
	}

	bool visibleState() const
	{
		return _visible;
	}

	void setPath(std::filesystem::path path, std::string volumeName)
	{
		_folderUpStatus = 0;
		_rootFolder = path;
		_volumeName = volumeName;
		updateDirectoryList(_rootFolder);
		changed();
	}

	void select()
	{
		std::filesystem::path path = _currentPath;
		selected(path);

		changed();
	}

	std::wstring selectedPath() const
	{
		if(!_volumeName.empty())
		{
			std::string currentPath = convert(_currentPath);
			const auto position = currentPath.find(":/");
			if (position != std::string::npos)
				return convert(currentPath.replace(0, position, _volumeName));
		}
		return _currentPath;
	}

	void updatePath(std::wstring path)
	{
		if (_importContext)
		{
			bool isValidCertificate = isValidFileToDisplay(path);
			importFileSelected(isValidCertificate);
		}
		_currentPath = path;
	}

	int selectedIndex() const
	{
		return _currIndex;
	}

	void cancel()
	{
		selected(std::filesystem::path());
		changed();
	}

	void addFolder()
	{
		_currView = View::AddFolder;
		_folderNameStatus = FolderName::Valid;
		_folderName.clear();
		changed();
	}

	std::wstring newFolderName() const
	{
		return _folderName;
	}

	void createNewFolder()
	{
		trimTrailingSpaces();
		validateFolderName();
		std::filesystem::path rootPath = _rootFolder;
		rootPath /= _folderName;
		validateFolderPathLength(rootPath);
		if (_folderNameStatus == FolderName::Valid)
		{
			std::filesystem::create_directory(rootPath);
			updateDirectoryList(_rootFolder);
			_currView = View::Directory;
		}
		changed();
	}

	void validateFolderPathLength(const std::filesystem::path& path)
	{
		if (path.wstring().size() > maxPathLength) _folderNameStatus = FolderName::invalidFolderPathLength;
		return;
	}

	void validateFolderName()
	{
		//Folder Name Validation for unsupported foldernames in windows
		const std::wstring invalidChar = L"\\/:*?<>|";
		const std::vector<std::wstring> invalidNames = { L"CON",L"PRN", L"AUX", L"NUL",
		L"COM0" ,L"COM1" , L"COM2" , L"COM3" , L"COM4" , L"COM5" , L"COM6" , L"COM7" , L"COM8" ,
		L"COM9" , L"LPT0" , L"LPT1" , L"LPT2" , L"LPT3" , L"LPT4" , L"LPT5" , L"LPT6" , L"LPT7" ,
		L"LPT8" , L"LPT9" };

		for (const auto& name : invalidNames)
		{
			bool isEqual = std::equal(name.begin(), name.end(), _folderName.begin(),
				_folderName.end(), [](wchar_t a, wchar_t b) {return towlower(a) ==
				towlower(b); });
			if (isEqual)
			{
				_folderNameStatus = FolderName::Invalid;
				return;
			}
		}

		//Validation for unsupported characters in Windows
		for (const auto& fileChar : invalidChar)
		{
			if (_folderName.find(fileChar) != std::wstring::npos)
			{
				_folderNameStatus = FolderName::UnsupportedCharacters;
				return;
			}
		}

		//Validation for dupicate folder name
		for (const auto& files : _folderList)
		{
			std::wstring file = files.filename().wstring();
			bool isEqual = std::equal(file.begin(), file.end(), _folderName.begin(),
				_folderName.end(), [](wchar_t a, wchar_t b) {return towlower(a) ==
				towlower(b); });
			if (isEqual)
			{
				_folderNameStatus = FolderName::Duplicate;
				return;
			}
		}
	}

	void trimTrailingSpaces()
	{
		const auto begin = 0;
		const auto end = _folderName.find_last_not_of(L" ");
		const auto range = end - begin + 1;
		_folderName = _folderName.substr(begin, range);
	}

	View view() const
	{
		return _currView;
	}

	void openFolder()
	{
		if (_currIndex != invalidIndex)
		{
			const auto path = _folderList[_currIndex];
			_rootFolder = path;
			_folderUpStatus++;
			updateDirectoryList(_rootFolder);
			changed();
		}
	}

	int folderUpState() const
	{
		return _folderUpStatus;
	}

	void folderUp()
	{
		if (_importContext && (1 == _folderUpStatus))
		{
			_currView = View::UsbList;
			_folderUpStatus--;
		}

		if (_currView != View::UsbList)
		{
			if (_currView == View::Directory)
			{
				_rootFolder = _rootFolder.parent_path();
				updatePath(_rootFolder.generic_wstring());
				_folderUpStatus--;	
			}

			updateDirectoryList(_rootFolder);
			_currView = View::Directory;
		}

		changed();
	}

	std::wstring calenderDate(std::chrono::system_clock::time_point time) const
	{
		auto rawTime = std::chrono::system_clock::to_time_t(time);
		struct tm timeInfo;
		if (localtime_s(&timeInfo, &rawTime) != 0)
		{
			return L"";
		}
		const int Size = 100;
		wchar_t buffer[Size];
		std::wcsftime(buffer, Size, L"%F %R", &timeInfo);
		return std::wstring(buffer);
	}

	void updateDirectoryList(const std::filesystem::path& path)
	{		
		std::vector<std::filesystem::path> fileList;
		for (const auto& file : std::filesystem::directory_iterator(path))
		{
			bool isValidCertificate = false;

			if (_importContext)
			{
				isValidCertificate = isValidFileToDisplay(file);
			}

			if (std::filesystem::path(file).filename().wstring() == L"System Volume Information")
			{
				continue;
			}
			if (std::filesystem::is_directory(file) || isValidCertificate)
			{
				fileList.emplace_back(file);
			}
		}
		_folderList = fileList;
		updateIndex(invalidIndex);
	}

	bool isValidFileToDisplay(const std::filesystem::path& path)
	{
		bool found = false;
		auto extn = path.filename().extension().string();
		auto it = std::find(_fileList.begin(), _fileList.end(), extn.c_str());
		if (it != _fileList.end())
		{
			found = true;
		}

		return found;
	}

	const std::vector<std::filesystem::path>& directoryList() const
	{
		return _folderList;
	}

	void setNewFolderName(const std::wstring& name)
	{
		int spaces = 0;
		for (const auto& character : name)
		{
			if (isspace(character))
			{
				spaces++;
			}
		}
		if ((name.size() == 0) || (spaces < name.size()))
		{
			this->_folderName = name;
		}
		_folderNameStatus = FolderName::Valid;
		changed();
	}

	FolderName folderNameState() const
	{
		return _folderNameStatus;
	}

	void setUsbPath()
	{
		if (_currIndex != invalidIndex)
		{
			setPath(_monitorUsb.usbs()[_currIndex]->drive(), _monitorUsb.usbs()[_currIndex]->volumeName());
		}
	}

	std::string getUsbVolumeName(int index)
	{
		std::string usbVolumeName = "";
		if (index != invalidIndex)
		{
			usbVolumeName = _monitorUsb.usbs()[index]->volumeName();
		}
		return usbVolumeName;
	}

	void unlockUsb(const std::wstring& password)
	{
		if (_currIndex != invalidIndex)
		{
			bool status = _monitorUsb.usbs()[_currIndex]->unlock(convert(password));

			if (status)
			{
				setUsbPath();
				_folderUpStatus++;
				_currView = View::Directory;
				_usbUnlockStatus = UsbUnlock::UnlockSuccess;
			}
			else
			{
				_usbUnlockStatus = UsbUnlock::UnlockFailed;
			}
			changed();
		}
	}

	void setUsbPassword(const std::wstring& password)
	{
		_usbUnlockStatus = UsbUnlock::UnKnown;
		this->_usbUnlockPassword = password;

		changed();
	}

	UsbUnlock unlockStatus()
	{
		return _usbUnlockStatus;
	}

	const std::wstring& usbUnlockPassword()
	{
		return _usbUnlockPassword;
	}

	void openUsb()
	{
		if (_currIndex != invalidIndex)
		{
			if (_monitorUsb.usbs()[_currIndex]->locked())
			{
				_folderUpStatus++;
				_currView = View::UnLockUsb;
				_usbUnlockStatus = UsbUnlock::UnKnown;
				_usbUnlockPassword.clear();
				changed();
			}
			else
			{
				setUsbPath();
				_folderUpStatus++;
				_currView = View::Directory;
				changed();
			}
		}
	}

	void onUsbDetection()
	{
		changed();
	}

	void onUsbRemoval()
	{ 
		bool removalStatus = true;
		const auto& usbs = usbList();
		for (const auto &list : usbs)
		{
			if (list->volumeName() == _volumeName)
				removalStatus = false;
		}

		if (removalStatus)
		{
			_currView = View::UsbList;
		}

		changed();
	}

	const std::vector <std::unique_ptr<Media::IUsb>>& usbList() const
	{
		return _monitorUsb.usbs();
	}

	void updateAllowedFileList(const std::vector <std::string>& fileType)
	{
		std::vector <std::string> fileList;
		for (const auto& file : fileType)
		{
			fileList.emplace_back(file);
		}
		_fileList = fileList;

		updateDirectoryList(_rootFolder);
	}

	public:
		Event<>		   changed;
		Event<const std::filesystem::path&>		       selected;
		Event<bool&>							  importFileSelected;


	private:
		Singleton()
		    : _folderNameStatus(FolderName::Valid)
			, _currView(View::Directory)
			, _usbUnlockStatus(UsbUnlock::UnKnown)
			, _visible(true)
			, _currIndex(invalidIndex)
			, _index(invalidIndex)
			, _folderUpStatus(0)
			, _monitorUsb(*(new Media::UsbMonitor))
			, _importContext(false)
		{
			delete &_monitorUsb;
		};

		explicit Singleton(const Media::IUsbMonitor& usbMonitor)
			: _folderNameStatus(FolderName::Valid)
			, _currView(View::UsbList)
			, _usbUnlockStatus(UsbUnlock::UnKnown)
			, _visible(true)
			, _currIndex(invalidIndex)
			, _index(invalidIndex)
			, _folderUpStatus(0)
			, _monitorUsb(usbMonitor)
			, _importContext(true)
		{};

	private:
		FolderName									_folderNameStatus;
		View									    _currView        ;
		UsbUnlock                                   _usbUnlockStatus ;
		bool									    _visible         ;
		int										    _currIndex       ;
		int										    _index           ;
		std::wstring								_folderName      ;
		std::wstring								_currentPath     ;
		std::filesystem::path					    _rootFolder      ;
		int										    _folderUpStatus  ;
		std::vector<std::filesystem::path>			_folderList      ;
		std::string                                 _volumeName      ;
		const Media::IUsbMonitor&     	            _monitorUsb      ;
		bool										_importContext   ;
		std::vector<std::string>					_fileList        ;
		std::wstring								_usbUnlockPassword;
	};

	FileBrowserViewModel::FileBrowserViewModel():
		readOnly(false),
		_importContext(false),
		_instance(Singleton::instance())    
	{
		subscribeEvents();
	}

	FileBrowserViewModel::FileBrowserViewModel(const Media::IUsbMonitor& usbMonitor) :
		readOnly(false),
		_importContext(true),
		_instance(Singleton::instance(usbMonitor))
	{
		subscribeEvents();
		subscribeUsbEvents(usbMonitor);
	}

	void FileBrowserViewModel::subscribeEvents()
	{
		_instance->changed.subscribe([this]()
		{
			changed();
		});
		_instance->selected.subscribe([this](const std::filesystem::path &path)
		{
			selected(path);
		});
		_instance->importFileSelected.subscribe([this](bool &importStatus)
		{
			importFileSelected(importStatus);
		});
	}

	void FileBrowserViewModel::subscribeUsbEvents(const Media::IUsbMonitor& usbMonitor)
	{
		usbMonitor.usbAdded.subscribe([this](const Workflow::Media::IUsb&)
		{
			_instance->onUsbDetection();
		});
		usbMonitor.usbRemoved.subscribe([this](const Workflow::Media::IUsb&)
		{
			_instance->onUsbRemoval();
		});
	}

	void FileBrowserViewModel::updateIndex(int index)
	{
		_instance->updateIndex(index);
	}

	bool FileBrowserViewModel::visibleState() const
	{
		return _instance->visibleState();
	}

	void FileBrowserViewModel::setPath(std::filesystem::path path, const std::string& volumeName) const
	{
		_instance->setPath(path,volumeName);
	}

	void FileBrowserViewModel::select()
	{
		_instance->select();
	}

	std::wstring FileBrowserViewModel::selectedPath() const
	{
		return _instance->selectedPath();
	}

	void FileBrowserViewModel::updatePath(std::wstring path)
	{
		_instance->updatePath(path);
	}

	int FileBrowserViewModel::selectedIndex() const
	{
		return _instance->selectedIndex();
	}

	void FileBrowserViewModel::cancel()
	{
		_instance->cancel();
	}

	void FileBrowserViewModel::addFolder()
	{
		_instance->addFolder();
	}

	std::wstring FileBrowserViewModel::newFolderName() const
	{
		return _instance->newFolderName();
	}

	std::wstring FileBrowserViewModel::newPassword() const
	{
		return _instance->usbUnlockPassword();
	}

	void FileBrowserViewModel::createNewFolder()
	{
		_instance->createNewFolder();
	}

	void FileBrowserViewModel::validateFolderName()
	{
		_instance->validateFolderName();
	}

	void FileBrowserViewModel::trimTrailingSpaces()
	{
		_instance->trimTrailingSpaces();
	}

	FileBrowserViewModel::View FileBrowserViewModel::view() const
	{
		return _instance->view();
	}

	void FileBrowserViewModel::openFolder()
	{
			_instance->openFolder();	
	}

	int FileBrowserViewModel::folderUpState() const
	{
		return _instance->folderUpState();
	}

	void FileBrowserViewModel::folderUp()
	{
		_instance->folderUp();
	}

	std::wstring FileBrowserViewModel::calenderDate(std::chrono::system_clock::time_point time) const
	{
		return _instance->calenderDate(time);
	}

	void FileBrowserViewModel::updateDirectoryList(const std::filesystem::path& path)
	{
		_instance->updateDirectoryList(path);
	}

	const std::vector<std::filesystem::path>& FileBrowserViewModel::directoryList() const
	{
		return _instance->directoryList();
	}

	bool FileBrowserViewModel::isValidFileToDisplay(const std::filesystem::path& path) const
	{
		return _instance->isValidFileToDisplay(path);
	}

	void FileBrowserViewModel::setNewFolderName(const std::wstring& name)
	{
		_instance->setNewFolderName(name);	
	}

	void FileBrowserViewModel::unlockUsb(const std::wstring& password)
	{
		_instance->unlockUsb(password);
	}

	void FileBrowserViewModel::setUsbPassword(const std::wstring& password)
	{
		_instance->setUsbPassword(password);
	}

	FileBrowserViewModel::FolderName FileBrowserViewModel::folderNameState() const
	{
		return _instance->folderNameState();
	}

	const std::vector <std::unique_ptr<Philips::Workflow::Media::IUsb>>& FileBrowserViewModel::usbList() const
	{
		return _instance->usbList();
	}

	void FileBrowserViewModel::openSelection()
	{
		if (_instance->view() == View::UsbList)
		{
			_instance->openUsb();
		}
		else
		{
			_instance->openFolder();
		}
	}

	bool FileBrowserViewModel::importContext() const
	{
		return _importContext;
	}

	void FileBrowserViewModel::setAllowedFileTypes(const std::vector <std::string>& fileType)
	{
		_instance->updateAllowedFileList(fileType);
	}

	FileBrowserViewModel::UsbUnlock FileBrowserViewModel::unlockStatus() const
	{
		return _instance->unlockStatus();
	}

	std::string FileBrowserViewModel::usbVolumeName(int index) const
	{
		return _instance->getUsbVolumeName(index);
	}

	FileBrowserViewModel::~FileBrowserViewModel()
	{
	}	
}}}