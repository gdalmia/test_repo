// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "Workflow/control/filebrowser.h"

#include <Automation.h>

namespace Philips { namespace Workflow { namespace Control { namespace TestPeer {

	class FileBrowserPeer 
	{
	public:
		explicit FileBrowserPeer(FileBrowser &control);

		bool selectFolder(const std::wstring& path) const;
		bool expandFolder(const std::wstring& folderName) const;
		bool browse(const std::wstring& path) const;
		bool addFolder(const std::wstring& path) const;
		bool folderUp() const;
		bool visiblity() const;
		bool select() const;
		bool cancel() const;
		bool wrongFileNameVisibility() const;
		bool isBackButtonEnabled() const;
		int selectedFolder() const;
		std::wstring getFolderName(int index1, int index2) const;
		bool isAddFolderButtonVisible() const;
		std::wstring getBrowserTitleText() const;
		std::wstring getSelectedPathText() const;
		bool clickAddFolderButton() const;
		Sense::Point getPosition() const;
		bool showHide() const;
		bool unlockUsb(const std::wstring& password) const;
		bool selectUsb(const std::wstring& volumeName, const std::wstring& password) const;
				
	private:
		bool ensureVisible(const std::wstring& folderPath) const;
		bool selectUsbAfterUnlocking(const int index, const std::wstring& usbVolumeName, const std::wstring& volumeName, const std::wstring& password) const;
		
		std::unique_ptr<Sense::Automation::Peer<Sense::Control>>	   _browserDialogParent    ;
		std::unique_ptr<Sense::Automation::Peer<Sense::DLS::DialogMinimal>>	   _browserDialog  ;
		std::unique_ptr<Sense::Automation::Peer<Sense::ButtonBase>>    _selectButton           ;
		std::unique_ptr<Sense::Automation::Peer<Sense::ButtonBase>>    _cancelButton           ;
		std::unique_ptr<Sense::Automation::Peer<Sense::ButtonBase>>    _backButton             ;
		std::unique_ptr<Sense::Automation::Peer<Sense::ButtonBase>>    _addFolderButton        ;
		std::unique_ptr<Sense::Automation::Peer<Sense::EditBoxBase>>   _editFolderName         ;
		std::unique_ptr<Sense::Automation::Peer<Sense::ButtonBase>>    _createFolderButton     ;
		std::unique_ptr<Sense::Automation::Peer<Sense::DLS::Label>>    _wrongFileNameVisibility;
	    std::unique_ptr<Sense::Automation::Peer<Sense::DLS::ListView>> _listFolder             ;
		std::unique_ptr<Sense::Automation::Peer<Sense::DLS::Label>>    _titleLabel             ;
		std::unique_ptr<Sense::Automation::Peer<Sense::DLS::Label>>    _selectedPathLabel      ;
		std::unique_ptr<Sense::Automation::Peer<Sense::EditBoxBase>>   _usbUnlockPasswordBox   ;
		std::unique_ptr<Sense::Automation::Peer<Sense::ButtonBase>>    _btnShowHide            ;
	};

}}}}
