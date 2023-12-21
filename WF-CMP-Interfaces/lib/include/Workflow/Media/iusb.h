// Copyright Koninklijke Philips N.V., 2020. All Right Reserved4

#pragma once
#include <string>
#include "Workflow/event.h"

namespace Philips { namespace Workflow { namespace Media {
	
	//! Represents a USB device
	class IUsb
	{
	public:
		IUsb() = default;
		
		virtual ~IUsb() = default;
		//! Gets drive letter
		virtual std::string drive() const = 0;

		//! Gets volume name
		virtual std::string volumeName() const = 0;

		//! Gets totalNumber of bytes
		virtual uint64_t totalNumberOfBytes() const = 0;

		//! Gets total number of free bytes
		virtual uint64_t totalNumberOfFreeBytes() const = 0;

		//! Gets locked status
		virtual bool locked() const = 0;

		// Unlocks the usb
		//! \param password The password to unlock.
		virtual bool unlock(const std::string& password) = 0;

		//!< recalculates usb size
		virtual void recalculateSize() = 0;

		//!< ejects usb
		virtual bool eject() = 0;

		//! Event triggers when its changed
		Event<> changed; //TICS !INT#002: Event needs to be public so that it can be subscribed
	};
}}}