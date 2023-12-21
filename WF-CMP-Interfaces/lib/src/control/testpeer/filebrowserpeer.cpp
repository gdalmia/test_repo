// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "Workflow/control/testpeer/filebrowserpeer.h"
#include "../filebrowserview.h"
#include "Workflow/Media/usbmonitor.h"

namespace Philips { namespace Workflow { namespace Control { namespace TestPeer {

	namespace
	{
		template<typename TView>
		TView &view(const std::unique_ptr<Sense::Control> &control)
		{
			return *dynamic_cast<TView*>(control.get());
		}
	}
	
	FileBrowserPeer::FileBrowserPeer(FileBrowser& control)
	{
		if(control._style == UiStyle::Integrated)
		{
			_browserDialogParent = std::make_unique<Peer<Sense::Control>>(control);
			_browserDialog = std::make_unique<Peer<DialogMinimal>>(control._browserDialog);
			_selectButton = std::make_unique<Peer<ButtonBase>>(view<FileBrowserIntegrated>(control._browser).btnSelect);
			_cancelButton = std::make_unique<Peer<ButtonBase>>(view<FileBrowserIntegrated>(control._browser).btnCancel);
			_backButton = std::make_unique<Peer<ButtonBase>>(view<FileBrowserIntegrated>(control._browser).btnBack);
			_addFolderButton = std::make_unique<Peer<ButtonBase>>(view<FileBrowserIntegrated>(control._browser).btnAddFolder);
			_listFolder = std::make_unique<Peer<ListView>>(view<FileBrowserIntegrated>(control._browser).listFile);
			_editFolderName = std::make_unique<Peer<EditBoxBase>>(view<FileBrowserIntegrated>(control._browser).editFolderName);
			_createFolderButton = std::make_unique<Peer<ButtonBase>>(view<FileBrowserIntegrated>(control._browser).btnCreateFolder);
			_wrongFileNameVisibility = std::make_unique<Peer<Label>>(view<FileBrowserIntegrated>(control._browser).wrongFileNameLabel);
			_titleLabel = std::make_unique<Peer<Label>>(view<FileBrowserIntegrated>(control._browser).headerLabel);
			_selectedPathLabel = std::make_unique<Peer<Label>>(view<FileBrowserIntegrated>(control._browser).pathLabel);
			_usbUnlockPasswordBox = std::make_unique<Peer<EditBoxBase>>(view<FileBrowserIntegrated>(control._browser).usbUnlockPasswordBox);
			_btnShowHide = std::make_unique<Peer<ButtonBase>>(view<FileBrowserIntegrated>(control._browser).btnShowHide);
		}
		else
		{
			_browserDialogParent = std::make_unique<Peer<Sense::Control>>(control);
			_browserDialog = std::make_unique<Peer<DialogMinimal>>(control._browserDialog);
			_selectButton = std::make_unique<Peer<ButtonBase>>(view<FileBrowserTouch>(control._browser).btnSelect);
			_cancelButton = std::make_unique<Peer<ButtonBase>>(view<FileBrowserTouch>(control._browser).btnCancel);
			_backButton = std::make_unique<Peer<ButtonBase>>(view<FileBrowserTouch>(control._browser).btnBack);
			_addFolderButton = std::make_unique<Peer<ButtonBase>>(view<FileBrowserTouch>(control._browser).btnAddFolder);
			_listFolder = std::make_unique<Peer<ListView>>(view<FileBrowserTouch>(control._browser).listFile);
			_editFolderName = std::make_unique<Peer<EditBoxBase>>(view<FileBrowserTouch>(control._browser).editFolderName);
			_createFolderButton = std::make_unique<Peer<ButtonBase>>(view<FileBrowserTouch>(control._browser).btnCreateFolder);
			_wrongFileNameVisibility = std::make_unique<Peer<Label>>(view<FileBrowserTouch>(control._browser).wrongFileNameLabel);
			_titleLabel = std::make_unique<Peer<Label>>(view<FileBrowserTouch>(control._browser).headerLabel);
			_selectedPathLabel = std::make_unique<Peer<Label>>(view<FileBrowserTouch>(control._browser).pathLabel);
			_usbUnlockPasswordBox = std::make_unique<Peer<EditBoxBase>>(view<FileBrowserTouch>(control._browser).usbUnlockPasswordBox);
			_btnShowHide = std::make_unique<Peer<ButtonBase>>(view<FileBrowserTouch>(control._browser).btnShowHide);
		}
	}


	bool FileBrowserPeer::selectFolder(const std::wstring& folderName) const
	{
		auto& items = _listFolder->control.getItems();
		for (int index = 0; index < static_cast<int>(items.size()); index++)
		{
			if (items[index].cells.at(2).text == folderName)
			{
				_listFolder->ensureVisible(index);
				_listFolder->select(index);
				return true;
			}
		}
		return false;
	}

	int FileBrowserPeer::selectedFolder() const
	{
		return _listFolder->control.getSelectedItem(); 
	}
	std::wstring FileBrowserPeer::getFolderName(int index1, int index2) const
	{
		return _listFolder->control.getItem(index1).cells.at(index2).text;
	}

	bool FileBrowserPeer::isAddFolderButtonVisible() const
	{
		return _addFolderButton->control.isVisible();
	}

	std::wstring FileBrowserPeer::getBrowserTitleText() const
	{
		return _titleLabel->control.text;
	}

	std::wstring FileBrowserPeer::getSelectedPathText() const
	{
		return _selectedPathLabel->control.text;
	}

	bool FileBrowserPeer::clickAddFolderButton() const
	{
		if (!_addFolderButton->control.visible)
		{
			return false;
		}
		_addFolderButton->mouseClick();
		return true;
	}

	Point FileBrowserPeer::getPosition() const
	{
		return _browserDialog->control.position;
	}

	bool FileBrowserPeer::ensureVisible(const std::wstring& folderPath) const
	{
		auto& items = _listFolder->control.getItems();
		for (int index = 0; index < static_cast<int>(items.size()); index++)
		{
			if (items[index].cells.at(2).text == folderPath)
			{
				_listFolder->ensureVisible(index);
				return true;
			}
		}
		return false;
	}

	bool FileBrowserPeer::expandFolder(const std::wstring& folderName) const
	{
		for (const auto& items : _listFolder->getChildren())
		{
			const auto item = dynamic_cast<CustomListView::CustomListViewItem*>(items);
			if (item != nullptr && item->cells->at(2).text == folderName)
			{
				if(!ensureVisible(folderName))
					return false;

				Peer<Sense::Control> itemPeer(*item);
				itemPeer.mouseDoubleClick();
				return true;
			}
		}
		return false;
	}

	bool FileBrowserPeer::browse(const std::wstring& path) const
	{
		auto folderPath = std::filesystem::path(path);
		for (const auto& folder : folderPath)
		{
			if (folder == *--folderPath.end())
				return selectFolder(folder.wstring());

			if (!expandFolder(folder.wstring()))
				break;
		}
		return false;
	}

	bool FileBrowserPeer::addFolder(const std::wstring& folderName) const
	{
		if (!clickAddFolderButton())
		{
			return false;
		}
		_editFolderName->enterText(folderName);
		_createFolderButton->mouseClick();
		return !wrongFileNameVisibility();
	}

	bool FileBrowserPeer::folderUp() const
	{
		if (!_backButton->control.visible)
		{
			return false;
		}
		_backButton->mouseClick();
		return true;
	}

	bool FileBrowserPeer::visiblity() const
	{
		return _browserDialogParent->control.visible;
	}

	bool FileBrowserPeer::select() const
	{
		if (!_selectButton->control.visible)
		{
			return false;
		}
		_selectButton->mouseClick();
		return !visiblity();
	}

	bool FileBrowserPeer::cancel() const
	{
		if (!_cancelButton->control.visible)
		{
			return false;
		}
		_cancelButton->mouseClick();
		return !visiblity();
	}
	bool FileBrowserPeer::wrongFileNameVisibility() const
	{
		return _wrongFileNameVisibility->control.visible;
	}

	bool FileBrowserPeer::isBackButtonEnabled() const
	{
		return _backButton->control.isEnabled();
	}

	bool FileBrowserPeer::showHide() const
	{
		if (!_btnShowHide->control.visible)
		{
			return false;
		}
		_btnShowHide->mouseClick();
		return true;
	}

	bool FileBrowserPeer::unlockUsb(const std::wstring& password) const
	{
		_usbUnlockPasswordBox->enterText(password);
		_createFolderButton->mouseClick();
		return !wrongFileNameVisibility();
	}

	bool FileBrowserPeer::selectUsb(const std::wstring& volumeName, const std::wstring& password) const
	{
		Media::UsbMonitor usbMonitor;
		auto usbCount = usbMonitor.usbs().size();
		if (usbCount == 0) return false;

		if (getSelectedPathText() != L"This System")
		{
			while (isBackButtonEnabled())
				folderUp();
		}

		auto& items = _listFolder->control.getItems();
		for (int index = 0; index < static_cast<int>(items.size()); index++)
		{
			auto usbVolumeName = items[index].cells.at(2).text;
			if (usbVolumeName == volumeName)
				break;

			if (usbMonitor.usbs()[index]->locked())
			{
				if(selectUsbAfterUnlocking(index, usbVolumeName, volumeName, password))
					return true;
			}
		}

		if (selectFolder(volumeName))
			return true;

		return false;
	}

	bool FileBrowserPeer::selectUsbAfterUnlocking(const int index, const std::wstring& usbVolumeName, const std::wstring& volumeName, const std::wstring& password) const
	{
		bool status = false;
		if (usbVolumeName == L"USB")
		{
			_listFolder->select(index);
			select();
			status = unlockUsb(password);
			folderUp();

			if (status)
			{
				status = selectFolder(volumeName);
			}
		}
		return status;
	}
}}}}