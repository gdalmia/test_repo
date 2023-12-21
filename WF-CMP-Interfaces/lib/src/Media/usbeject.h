// Copyright 2017 Koninklijke Philips N.V.
#pragma once
#include <string>
#include <Workflow/stringconverter.h>
#include <Windows.h>
#include <cfgmgr32.h>

namespace Philips { namespace Workflow { namespace Media {

	class UsbEject
	{
	public:
		static bool eject(const std::string& drive);

	private:
		static const GUID* getDeviceInterfaceGUID(UINT DriveType);
		static DEVINST GetDrivesDevInstByDeviceNumber(long DeviceNumber, UINT DriveType);
	};
}}}