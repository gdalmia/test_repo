// Copyright 2017 Koninklijke Philips N.V.
#pragma once
#include "usbeject.h"
#define WIN32_LEAN_AND_MEAN
#include <thread>
#include <atomic>
#include <iostream>
#include <filesystem>
#include <stdio.h>
#include <winioctl.h>
#include <winioctl.h>
#include <Setupapi.h>

#pragma comment (lib, "Setupapi.lib")

namespace Philips { namespace Workflow { namespace Media {

	const GUID* UsbEject::getDeviceInterfaceGUID(UINT DriveType)
	{
		const GUID* guid;
		switch (DriveType)
		{
		case DRIVE_REMOVABLE:
			//Since we are not considering floppy drive,so we always returns usb guid
			guid = reinterpret_cast<const GUID*>(&GUID_DEVINTERFACE_DISK);
			break;
		case DRIVE_FIXED:
			guid = reinterpret_cast<const GUID*>(&GUID_DEVINTERFACE_DISK);
			break;
		case DRIVE_CDROM:
			guid = reinterpret_cast<const GUID*>(&GUID_DEVINTERFACE_CDROM);
			break;
		default:
			return 0;
		}
		return guid;
	}

	DEVINST UsbEject::GetDrivesDevInstByDeviceNumber(long DeviceNumber, UINT DriveType)
	{
		//TraceScope();
		const GUID* guid = getDeviceInterfaceGUID(DriveType);
		// Get device interface info set handle
		// for all devices attached to system
		HDEVINFO hDevInfo = SetupDiGetClassDevs(guid, NULL, NULL, static_cast<DWORD>(DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

		if (hDevInfo == INVALID_HANDLE_VALUE)
		{
			return 0;
		}

		// Retrieve a context structure for a device interface
		// of a device information set.
		DWORD dwIndex = 0;
		BOOL bRet = FALSE;
		BYTE buf[1024];
		PSP_DEVICE_INTERFACE_DETAIL_DATA pspdidd = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(buf);
		SP_DEVICE_INTERFACE_DATA         spdid;
		SP_DEVINFO_DATA                  spdd;
		DWORD                            dwSize;

		spdid.cbSize = sizeof(spdid);

		while (true)
		{
			bRet = SetupDiEnumDeviceInterfaces(hDevInfo, NULL, guid, dwIndex, &spdid);
			if (!bRet)
			{
				break;
			}
			dwSize = 0;
			SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, NULL, 0, &dwSize, NULL);

			if (dwSize != 0 && dwSize <= sizeof(buf))
			{
				pspdidd->cbSize = sizeof(*pspdidd); // 5 Bytes!
				ZeroMemory(reinterpret_cast<PVOID>(&spdd), sizeof(spdd));
				spdd.cbSize = sizeof(spdd);

				BOOL res = SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, pspdidd, dwSize, &dwSize, &spdd);
				if (res)
				{
					HANDLE hDrive = CreateFile(pspdidd->DevicePath, 0,
						FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
					if (hDrive != INVALID_HANDLE_VALUE)
					{
						STORAGE_DEVICE_NUMBER sdn;
						DWORD dwBytesReturned = 0;
						res = DeviceIoControl(hDrive, IOCTL_STORAGE_GET_DEVICE_NUMBER,
							NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL);
						if (res)
						{
							if (DeviceNumber == static_cast<long>(sdn.DeviceNumber))
							{
								CloseHandle(hDrive);
								SetupDiDestroyDeviceInfoList(hDevInfo);
								return spdd.DevInst;
							}
						}
						CloseHandle(hDrive);
					}
				}
			}
			dwIndex++;
		}
		SetupDiDestroyDeviceInfoList(hDevInfo);
		return 0;
	}

	bool  UsbEject::eject(const std::string& drive)
	{
		// for GetDriveType
		auto wStringDrive = convert(drive);
		wchar_t szRootPath[] = L"Z:\\";
		szRootPath[0] = *(wStringDrive.c_str());

		//to open the volume
		wchar_t szVolumeAccessPath[] = L"\\\\.\\Z:";
		szVolumeAccessPath[4] = *(wStringDrive.c_str());

		long DeviceNumber = -1;
		HANDLE hVolume = CreateFile(szVolumeAccessPath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, 0, NULL);
		if (hVolume == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		STORAGE_DEVICE_NUMBER sdn;
		DWORD dwBytesReturned = 0;
		bool res = DeviceIoControl(hVolume, IOCTL_STORAGE_GET_DEVICE_NUMBER,
			NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL);
		if (res)
		{
			DeviceNumber = static_cast<long>(sdn.DeviceNumber);
		}
		CloseHandle(hVolume);

		if (DeviceNumber == -1) return false;
		UINT DriveType = GetDriveType(szRootPath);

		DEVINST DevInst = GetDrivesDevInstByDeviceNumber(DeviceNumber, DriveType);
		if (DevInst == 0)
		{
			return false;
		}
		PNP_VETO_TYPE VetoType = PNP_VetoTypeUnknown;
		WCHAR VetoNameW[MAX_PATH];
		bool bSuccess = false;

		// get drives's parent, e.g. 
		// the SATA port, an IDE channel with two drives!
		DEVINST DevInstParent = 0;
		CM_Get_Parent(&DevInstParent, DevInst, 0);

		for (int tries = 0; tries <= 3; ++tries)
		{
			VetoNameW[0] = 0;
			res = CM_Request_Device_EjectW(DevInstParent, &VetoType, VetoNameW, MAX_PATH, 0);

			bSuccess = (res == CR_SUCCESS && VetoType == PNP_VetoTypeUnknown);
			if (bSuccess)
			{
				break;
			}
			Sleep(500); // required to give the next try a chance
		}
		return bSuccess;
	}

	
}}}