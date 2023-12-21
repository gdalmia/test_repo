// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "Workflow/binding/property.h"
#include "Workflow/event.h"
#include <filesystem>
#include <mutex>
#include "Workflow/Media/iusbmonitor.h"

//TICS -INT#002: View models needs data member in public

namespace std { namespace filesystem = std::experimental::filesystem; }

namespace Philips { namespace Workflow { namespace Control {
    class FileBrowserViewModel 
    {
    public:
        FileBrowserViewModel();
		explicit FileBrowserViewModel(const Media::IUsbMonitor& usbMonitor);
        enum class                                 FolderName { Valid, Invalid, Duplicate, UnsupportedCharacters, invalidFolderPathLength }; 
        enum class                                 View { Directory, AddFolder, UsbList, UnLockUsb };
		enum class                                 UsbUnlock { UnKnown, UnlockFailed, UnlockSuccess };

        virtual                                   ~FileBrowserViewModel();
        void                                      setPath(std::filesystem::path path, const std::string& volumeName) const;
        int                                       selectedIndex() const;  
        std::wstring                              selectedPath() const;   
        std::wstring                              newFolderName() const;
		std::wstring                              newPassword() const;
        bool                                      visibleState() const; 
        View                                      view() const;
        const std::vector<std::filesystem::path>& directoryList() const;  
        std::wstring                              calenderDate(std::chrono::system_clock::time_point time) const;
        void                                      select(); 
        void                                      updateIndex(int); 
        void                                      setNewFolderName(const std::wstring& name);
		void                                      unlockUsb(const std::wstring& password);
		void                                      setUsbPassword(const std::wstring& password);
        FolderName                                folderNameState() const;  
        int                                       folderUpState() const;    
        void                                      openFolder(); 
        void                                      cancel();
        void                                      folderUp(); 
        void                                      addFolder(); 
        void                                      createNewFolder(); 
        void                                      updatePath(std::wstring path);  
        void                                      updateDirectoryList(const std::filesystem::path& path);
        void                                      validateFolderName(); 
        void                                      trimTrailingSpaces();
        void                                      openSelection();
        const std::vector <std::unique_ptr<Media::IUsb>>& usbList() const;
        void                                      setAllowedFileTypes(const std::vector<std::string>& fileType);
        bool                                      importContext() const;
        bool                                      isValidFileToDisplay(const std::filesystem::path& path) const;
		UsbUnlock                                 unlockStatus() const;
		std::string                               usbVolumeName(int index) const;

        Binding::Property<bool> readOnly; //TICS !INT#002: Property needs to be public so that it can bound in view

        Event<>                                   changed;
        Event<const std::filesystem::path&>       selected;
        Event<bool&>                              importFileSelected;
        
    private:
		void subscribeEvents();
		void subscribeUsbEvents(const Media::IUsbMonitor& usbMonitor);

        class                                     Singleton;
        std::shared_ptr<Singleton>                _instance;
		bool                                      _importContext;
		//bool                                      _usbUnlockStatus;
    };
}}}