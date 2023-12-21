// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "usb.h"
#include "iusbmonitor.h"
#include <vector>
#include <memory>
#include <filesystem>


namespace std { namespace filesystem = experimental::filesystem; }

namespace Philips { namespace Workflow { namespace Media { 
	
	//! Monitor class monitors the media devices connected to the system and configuration
	class UsbMonitor : public IUsbMonitor
	{
	public:
		UsbMonitor();
		~UsbMonitor();

		//! \brief Returns usb devices connected to the system
		//! \return std::vector<Node>: usb devices connected
		const std::vector<std::unique_ptr<IUsb>>& usbs() const override;
		
	private:
		void subscribe();
		
		struct Private;
		std::unique_ptr<Private> _private;
	};

}}}