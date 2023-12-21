// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "uistyle.h"
#include <Workflow/event.h>
#include <Sense.h>
#include <DLS.h>
#include <filesystem>
#include "Workflow/Media/iusbmonitor.h"

namespace std { namespace filesystem = std::experimental::filesystem; }

namespace Philips { namespace Workflow { namespace Control {

	namespace TestPeer { class FileBrowserPeer; }
	
	//! This class provides File Browser dialog that can be used to export studies
	//! to a destination path
	class FileBrowser: public Sense::Control  
	{
		friend class TestPeer::FileBrowserPeer;
	public:
		FileBrowser(Sense::Control& parent, UiStyle style,const std::wstring& title = L"", bool readOnly = false);
		explicit FileBrowser(Sense::Control& parent, UiStyle style, const Media::IUsbMonitor& usbMonitor, const std::wstring& title = L"", bool readOnly = true);
		void setPath(const std::filesystem::path &path,const std::string& volumeName) const;			//!< Gets the directory path and launches file browser with given volume name
		void setPosition(const Sense::Point& position);													//!< Sets the position of the file browser dialog to the given point		
		void setAllowedFileTypes(const std::vector <std::string>& fileType);
		~FileBrowser();

	public:
		Event<const std::filesystem::path&>   pathSelected;				//!< Event to be subscribed to get the selected destination path
		
	private:
		void subscribe();
		void updateUi(UiStyle style, const std::wstring& title);
		void updateVariables(UiStyle style, const std::wstring& title, bool readOnly);
		void render(Sense::IRenderer &renderer) const override;

	private:
		Sense::DLS::DialogMinimal		_browserDialog;
		const UiStyle                   _style;						//!< Ui Style to decide the type of view to be initialized
		std::unique_ptr<Sense::Control> _browser;					
		struct Private;
		std::unique_ptr<Private>		_pImpl;	
	};
}}}