// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "Ui\filebrowserintegrated.ui.h"
#include "Ui\filebrowsertouch.ui.h"
#include "filebrowserviewmodel.h"
#include <filesystem>

namespace std { namespace filesystem = std::experimental::filesystem; }


namespace Philips { namespace Workflow { namespace Control {
	template <class T>
	class FileBrowserView  : public T 
	{
	public:
		 FileBrowserView(Sense::Control &parent, FileBrowserViewModel &model, VirtualKeyboard &keyboard, const std::wstring& title);
		 
	private:
		void registerEvents();
		void updateControls();
		void updateAddFolderView();
		void updateDirectoryView();
		void resizeAddFolderView();
		void resizeDirectoryView();
		void updateUsbView();
		void updateUsbDirectoryView();
		void updateUnlockUsbView();
		void setShowImageIcon();
		void setHideImageIcon();
		
	private:		
		FileBrowserViewModel& _model;	 
		VirtualKeyboard&	  _keyboard;
		std::wstring          _title;
		bool                  _importFile;
	};

	typedef FileBrowserView<Ui::FileBrowserIntegrated>	  FileBrowserIntegrated;
	typedef FileBrowserView<Ui::FileBrowserTouch>		  FileBrowserTouch;
}}}

