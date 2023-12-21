// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "iusb.h"

namespace Philips { namespace Workflow { namespace Media {
	
	//! Represents a Usb device
	class Usb: public IUsb
	{
		
	public:
		//! Creates a Usb with a drive value
		//! \param drive Drive letter of the usb
		explicit Usb(const std::string& drive);

		//! Creates a default Usb
		Usb();

		Usb(const std::string& drive, const std::string& volumeName, 
			uint64_t totalNumberOfBytes, uint64_t totalNumberOfFreeBytes, bool locked=false);

		Usb(const Usb& usb);

		bool operator==(const Usb& usb) const;
		
		bool operator!=(const Usb& usb) const;

		//!< gets drive letter of USB
		std::string drive() const override;

		//!< gets volume name of USB
		std::string volumeName() const override;

		//!< gets total number of bytes of USB
		uint64_t totalNumberOfBytes() const override;

		//!< gets total number of free bytes of USB
		uint64_t totalNumberOfFreeBytes() const override;

		//!< recalculates usb size
		void recalculateSize() override;

		//! gets status of usb : locked/unlocked
		bool locked() const override;

		//! unlocks usb with password passed
		bool unlock(const std::string&) override;

		//! ejects the usb
		bool eject() override;

	private: 
		std::string	_drive;									//!< Usb device drive
		std::string	_volumeName;							//!< Volume name of the Usb
		uint64_t _totalNumberOfBytes;						//!< Usb size in bytes
		uint64_t _totalNumberOfFreeBytes;					//!< Free space in bytes
		bool _locked;										//!< Gets usb lock/unlock status
	};
}}}