// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "filebrowserview.h"
#include "../Resource.h"
#include "../Localization.h"
#include "Workflow/binding/bind.h"
#include "Workflow/binding/converter.h"

namespace Philips { namespace Workflow { namespace Control {

	constexpr int invalidIndex = -1;

	template<class T>
	FileBrowserView<T>::FileBrowserView( Sense::Control &parent, FileBrowserViewModel &model , VirtualKeyboard &keyboard, const std::wstring& title)
		:
		 _keyboard(keyboard),
		 T(parent, keyboard),	
		 _model(model),
		_title(title),
		_importFile(false)
	{
		 registerEvents();
		 this->headerLabel.text = _title;
		 Binding::bind_visible(this->btnAddFolder, _model.readOnly, Binding::Invert());
	}

	template<class T>
	void FileBrowserView<T>::registerEvents()
	{
		_model.changed.subscribe([this]() 
		{ 
			updateControls(); 
		});

		_model.importFileSelected.subscribe([this](bool status)
		{
			_importFile = status;
			updateControls();
		});

		this->listFile.eventDoubleClick.subscribe([this](int index) 
		{
			if (!_importFile)
			{
				_model.updateIndex(index);
				_model.openSelection();
			}
		});

		this->listFile.eventSelectionChanged = [this]
		{
			_model.updateIndex(this->listFile.getSelectedItem());
		};
		this->btnBack.eventClicked = [this]
		{
			_model.folderUp();
		};
		this->btnCancel.eventClicked = [this]
		{
			_model.cancel();
		};
		this->btnSelect.eventClicked = [this]
		{
			if (_importFile)
			{
				_model.select();
			}
			else
			{
				if (_model.importContext())
				{
					_model.updateIndex(_model.selectedIndex());
					_model.openSelection();
				}
				else
				{
					_model.select();
				}
			}
		};

		this->editFolderName.eventEnter = [this]
		{
			_model.createNewFolder();
		};

		this->btnAddFolder.eventClicked = [this]
		{
			_model.addFolder();
		};
		this->editFolderName.eventChanged = [this]
		{
			_model.setNewFolderName(this->editFolderName.text);
		};
		this->btnCreateFolder.eventClicked = [this]
		{
			if (_model.importContext())
				_model.unlockUsb(this->usbUnlockPasswordBox.text);
			else
				_model.createNewFolder();
		};
		this->usbUnlockPasswordBox.eventChanged = [this]
		{
			_model.setUsbPassword(this->usbUnlockPasswordBox.text);
		};
		this->btnShowHide.eventClicked = [this]
		{
			if (this->usbUnlockPasswordBox.password == L'\0')
			{	
				this->usbUnlockPasswordBox.password = L'\u25CF';
				setShowImageIcon();
			}
			else
			{
				this->usbUnlockPasswordBox.password = L'\0';
				setHideImageIcon();
			}
		};
	}
	 
	template <class T>
	void FileBrowserView<T>::updateAddFolderView()
	{
		this->headerLabel.text = Localization::newFolder;
		this->pathLabel.visible = false;
		this->pathIcon.visible = false;
		this->listFile.visible = false;
		this->lblAddFolder.visible = true;
		this->lblFolderName.visible = true;
		this->btnCreateFolder.visible = true;
		this->btnSelect.enabled = false;
		this->btnCancel.enabled = false;
		this->wrongFilenameInfoLabel.visible = false;
		this->wrongFileNameLabel.visible = false;
		this->btnBack.enabled = true;
		this->btnBack.text = Localization::back;
		this->editFolderName.visible = true;
		this->editFolderName.setDefaultColor();
		this->editFolderName.assign(_model.newFolderName());
		this->usbUnlockPasswordBox.visible = false;
		this->btnShowHide.visible = false;
		

		if (!_model.newFolderName().empty())
		{
			this->btnCreateFolder.enabled = true;
		}
		else
		{
			this->btnCreateFolder.enabled = false;
		}
		if (_model.folderNameState() != FileBrowserViewModel::FolderName::Valid)
		{
			this->wrongFilenameInfoLabel.visible = true;
			this->wrongFileNameLabel.visible = true;
			this->btnCreateFolder.enabled = false;
			this->editFolderName.setColorInvalidCredentials();
			
		}
		if (_model.folderNameState() == FileBrowserViewModel::FolderName::Duplicate)
		{
			this->wrongFileNameLabel.text =  Localization::duplicateFolderName;
		}
		if (_model.folderNameState() == FileBrowserViewModel::FolderName::UnsupportedCharacters)
		{
			this->wrongFileNameLabel.text =Localization::invalidFolderChar;
		}
		if (_model.folderNameState() == FileBrowserViewModel::FolderName::Invalid)
		{
			this->wrongFileNameLabel.text = Localization::invalidFolderName;
		}
		if (_model.folderNameState() == FileBrowserViewModel::FolderName::invalidFolderPathLength)
		{
			this->wrongFileNameLabel.text = Localization::invalidFolderPathLength;
		}
	}

	template <class T>
	void FileBrowserView<T>::updateUnlockUsbView()
	{
		std::wstring unlockLabel = L"Unlock ";
		int usbIndex = _model.selectedIndex();
		if (usbIndex != invalidIndex)
		{
			unlockLabel += convert(_model.usbVolumeName(usbIndex));
		}
		this->headerLabel.text = unlockLabel;
		this->pathLabel.visible = false;
		this->pathIcon.visible = false;
		this->listFile.visible = false;
		this->lblAddFolder.text = L"The USB drive is locked, enter password to open";
		this->lblAddFolder.visible = true;
		this->lblFolderName.text = L"Password";
		this->lblFolderName.visible = true;
		this->btnCreateFolder.text = L"Unlock";
		this->btnCreateFolder.visible = true;
		this->btnSelect.text = L"Open";
		this->btnSelect.enabled = false;
		this->btnCancel.enabled = false;
		this->wrongFilenameInfoLabel.visible = false;
		this->wrongFileNameLabel.visible = false;
		this->btnBack.enabled = true;
		this->btnBack.text = Localization::back;
		this->btnShowHide.enabled = true;
		this->usbUnlockPasswordBox.visible = true;
		this->usbUnlockPasswordBox.setDefaultColor();

		this->usbUnlockPasswordBox.assign(_model.newPassword());

		if (!_model.newPassword().empty())
		{
			this->btnShowHide.visible = true;
			this->btnCreateFolder.enabled = true;
		}
		else
		{
			this->usbUnlockPasswordBox.margin = Margin(0, 0, 40, 0);
			this->btnShowHide.enabled = false;
			this->btnShowHide.visible = false;
			this->btnCreateFolder.enabled = false;
		}


		if (_model.unlockStatus() == FileBrowserViewModel::UsbUnlock::UnlockFailed)
		{
			this->wrongFileNameLabel.text = Localization::invalidUsbPassword;
			this->wrongFilenameInfoLabel.visible = true;
			this->wrongFilenameInfoLabel.image = Resource::Notice_Boom;
			this->wrongFilenameInfoLabel.color = Palette::S_Yellow30;
			this->wrongFileNameLabel.visible = true;
			this->btnCreateFolder.enabled = false;
			this->usbUnlockPasswordBox.setColorInvalidCredentials();
		}

		if (this->usbUnlockPasswordBox.password == L'\0')
		{
			setHideImageIcon();
		}
		else
		{
			setShowImageIcon();
		}

	}

	template <class T>
	void FileBrowserView<T>::updateDirectoryView()
	{
		this->headerLabel.text = _title;
		this->btnBack.text = Localization::folderUp;
		this->btnBack.enabled = true;
		this->listViewHeader2.text = Localization::name;
		this->listViewHeader3.text = Localization::lastModified;
		this->btnCancel.enabled = true;
		this->btnCancel.visible = true;
		this->pathLabel.text = _model.selectedPath();
		this->pathLabel.visible = true;
		this->btnSelect.visible = true;
		
		this->btnAddFolder.enabled = true;
		this->listFile.visible = true;
		this->editFolderName.visible = false;
		this->lblAddFolder.visible = false;
		this->lblFolderName.visible = false;
		this->btnCreateFolder.visible = false;
		this->wrongFilenameInfoLabel.visible = false;
		this->wrongFileNameLabel.visible = false;
		this->usbUnlockPasswordBox.visible = false;
		this->btnShowHide.visible = false;
		this->btnSelect.visible = true;

		if (_importFile)
		{
			this->pathIcon.image = Resource::USB_24;
			this->pathIcon.visible = true;
			this->btnSelect.text = L"Import";
			this->btnSelect.enabled = true;
		}
		else if (_model.importContext())
		{
			this->pathIcon.image = Resource::USB_24;
			this->pathIcon.visible = true;
			this->btnSelect.text = L"Open";

			if(_model.selectedIndex() == invalidIndex)
				this->btnSelect.enabled = false;
			else
				this->btnSelect.enabled = true;
		}
		else
		{
			this->pathIcon.visible = true;
			this->btnSelect.enabled = true;
		}

		std::vector<ListView::Item> listItems;
		std::vector<ListViewItem::Cell> cell(4);
		const auto files = _model.directoryList();
		for (const auto &list : files)
		{
			cell[0].text = L"";
			if (_model.isValidFileToDisplay(list))
				cell[1].image = Resource::DocumentTest_24;
			else
				cell[1].image = Resource::Folder_notselected_32;

			cell[1].text = L"";
			cell[2].text = list.filename();
			cell[3].text = _model.calenderDate(std::filesystem::last_write_time(list));
			ListView::Item listItem(cell);
			listItems.emplace_back(listItem);
		}
		this->listFile.setItems(listItems);

		if (int(listItems.size()) > _model.selectedIndex())
		{
			this->listFile.assign(_model.selectedIndex());
		}

		if (!_model.folderUpState())
		{
			this->btnBack.enabled = false;
		}
	}

	template <class T>
	void FileBrowserView<T>::updateUsbDirectoryView()
	{
		this->headerLabel.text = _title;
		this->btnBack.text = Localization::folderUp;
		this->btnBack.enabled = false;
		this->listViewHeader2.text = Localization::name;
		this->listViewHeader3.text = Localization::lastModified;
		this->btnCancel.enabled = true;
		this->btnCancel.visible = true;
		this->pathLabel.text = L"This System";
		this->pathIcon.image = Resource::DLS_Screen_48;
		this->pathLabel.visible = true;
		this->pathIcon.visible = true;
		this->btnSelect.visible = true;
		this->btnSelect.text = L"Open";
		this->usbUnlockPasswordBox.visible = false;
		this->btnShowHide.visible = false;

		if (_model.selectedIndex() == invalidIndex)
			this->btnSelect.enabled = false;
		else
			this->btnSelect.enabled = true;

		this->btnAddFolder.enabled = false;
		this->btnAddFolder.visible = false;
		this->listFile.visible = true;
		this->editFolderName.visible = false;
		this->lblAddFolder.visible = false;
		this->lblFolderName.visible = false;
		this->btnCreateFolder.visible = false;
		this->wrongFilenameInfoLabel.visible = false;
		this->wrongFileNameLabel.visible = false;

		std::vector<ListView::Item> listItems;
		std::vector<ListViewItem::Cell> cell(4);
		const auto& usbs = _model.usbList();
		for (const auto &list : usbs)
		{
			cell[0].text = L"";
			if (list->locked())
			{
				cell[1].image = Resource::USBlocked_24;
			}
			else
			{
				cell[1].image = Resource::USB_24;
			}
			cell[1].text = L"";
			cell[2].text = convert(list->volumeName());
			cell[3].text = L"";
			ListView::Item listItem(cell);
			listItems.emplace_back(listItem);
		}
		this->listFile.setItems(listItems);

		if (int(listItems.size()) > _model.selectedIndex())
		{
			this->listFile.assign(_model.selectedIndex());
		}

		if (!_model.folderUpState())
		{
			this->btnBack.enabled = false;
		}
	}

	template<>
	void FileBrowserView<Ui::FileBrowserIntegrated>::resizeAddFolderView()
	{
		this->listPanel.position = Point(24, 129);
		this->listPanel.size = Size(862, 659);
		this->btnBack.position = Point(24, 81);

		this->btnBack.image = Resource::Back_button_24;

		btnBack.textMargin = Margin(5, 9, 113, 9);
		btnBack.imageSize = Size(24, 24);
		btnBack.imageSpacing = 14;
		this->_keyboard.visible = false;
		
	}

	template<class T>
	void FileBrowserView<T>::resizeAddFolderView()
	{
		this->listPanel.position = Point(24, 144);
		this->listPanel.size = Size(652, 412);
		this->btnBack.position = Point(29, 76);

		this->btnBack.image = Resource::Back_button_32;
		this->btnBack.textMargin = Margin(7, 20, 113, 20);
		this->btnBack.imageSize = Size(32, 32);
		this->btnBack.imageSpacing = 17;
		if (!_model.importContext())
		{
			this->_keyboard.scale = 1;
			this->_keyboard.size = Size(_keyboard.size->width, 400);
		}
	}

	template<>
	void FileBrowserView<Ui::FileBrowserIntegrated>::resizeDirectoryView()
	{
		this->listPanel.position = Point(24, 180);
		this->listPanel.size = Size(862, 608);
		this->btnBack.position = Point(24, 132);

		this->btnAddFolder.image = Resource::Add_folder_24;
		this->btnBack.image = Resource::Foler_up_24;

		this->btnBack.textMargin = Margin(39, 9, 38, 9);
		this->btnBack.horizontalAlign = HAlign::Left;
		this->btnBack.imageSize = Size(24, 24);
		this->btnBack.imageSpacing = 9;

	}

	template<class T>
	void FileBrowserView<T>::resizeDirectoryView()
	{
		this->listPanel.position = Point(24, 196);
		this->listPanel.size = Size(652, 360);
		this->btnBack.position = Point(24, 132);

		this->btnAddFolder.image = Resource::Add_folder_32;
		this->btnBack.image = Resource::Folder_up_32;

		this->btnBack.textMargin = Margin(35, 20, 15, 20);
		this->btnBack.horizontalAlign = HAlign::Left;
		this->btnBack.imageSize = Size(32, 32);
		this->btnBack.imageSpacing = 10;
	}

	template<>
	void FileBrowserView<Ui::FileBrowserIntegrated>::setHideImageIcon()
	{
		this->btnShowHide.image = _model.unlockStatus() == FileBrowserViewModel::UsbUnlock::UnlockFailed ? Resource::Hide_password_error_Boom : Resource::Hide_Password_Grey_Boom;
	}

	template<class T>
	void FileBrowserView<T>::setHideImageIcon()
	{
		this->btnShowHide.image = _model.unlockStatus() == FileBrowserViewModel::UsbUnlock::UnlockFailed ? Resource::Hide_password_error_Touch : Resource::Hide_password_Grey_Touch;
	}

	template<>
	void FileBrowserView<Ui::FileBrowserIntegrated>::setShowImageIcon()
	{
		this->btnShowHide.image = _model.unlockStatus() == FileBrowserViewModel::UsbUnlock::UnlockFailed ? Resource::Show_Password_orange_Boom : Resource::Show_Password_Boom;
	}

	template<class T>
	void FileBrowserView<T>::setShowImageIcon()
	{
		this->btnShowHide.image = _model.unlockStatus() == FileBrowserViewModel::UsbUnlock::UnlockFailed ? Resource::Show_password_error_Touch_32 : Resource::Show_password_Touch_32;
	}

	template<class T>
	void FileBrowserView<T>::updateUsbView()
	{
		resizeDirectoryView();
		updateUsbDirectoryView();
	}

	template<class T>
	void FileBrowserView<T>::updateControls()
	{	
		if (_model.view() == FileBrowserViewModel::View::UsbList)
		{
			updateUsbView();
		}
		else if (_model.view() == FileBrowserViewModel::View::UnLockUsb)
		{
			resizeAddFolderView();
			updateUnlockUsbView();
		}
		else if (_model.view() == FileBrowserViewModel::View::AddFolder)
		{
			resizeAddFolderView();
			updateAddFolderView();
		}
		else
		{
			resizeDirectoryView();
			updateDirectoryView();
		}

	}
	template class FileBrowserView<Ui::FileBrowserIntegrated>;
	template class FileBrowserView<Ui::FileBrowserTouch>;
}}}