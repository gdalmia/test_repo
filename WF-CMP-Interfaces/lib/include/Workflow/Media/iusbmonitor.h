// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "iusb.h"
#include <vector>
#include <memory>


namespace Philips { namespace Workflow { namespace Media {
	
	//! Monitor class monitors the media devices connected to the system and configuration
	class IUsbMonitor
	{
	public:
		IUsbMonitor() = default;
		virtual ~IUsbMonitor() = default;

		//! \brief Returns usb devices connected to the system
		virtual const std::vector<std::unique_ptr<IUsb>>& usbs() const = 0;
		
		//! Event raised when a media node is connected
		Event<IUsb&> usbAdded;

		//! Event raised when a media Usb is disconnected
		Event<IUsb&> usbRemoved;

		IUsbMonitor(const IUsbMonitor&) = delete;
		IUsbMonitor(const IUsbMonitor&&) = delete;
		IUsbMonitor& operator=(const IUsbMonitor&) = delete;
		IUsbMonitor& operator=(IUsbMonitor&&) = delete;
	};

}}}