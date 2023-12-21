// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "devicereader.h"
#include <Workflow/stringconverter.h>
#include <windows.h>

namespace Philips { namespace Workflow { namespace Media { 

	//Get volume name of the device
	std::wstring readVolumeName(const std::wstring& drive)
	{	
		TCHAR fileSystemName[MAX_PATH + 1] = { 0 };
		DWORD serialNumber = 0;
		DWORD maxComponentLen = 0;
		DWORD fileSystemFlags = 0;
		TCHAR volumeName[MAX_PATH + 1] = { 0 };

		//WinAPI:Gets volume information
		GetVolumeInformationW(drive.c_str(),
			volumeName,
			ARRAYSIZE(volumeName),
			&serialNumber,
			&maxComponentLen,
			&fileSystemFlags,
			fileSystemName,
			ARRAYSIZE(fileSystemName));

		return volumeName;
	}

	//! Reads the volume name of the drive
	std::string readVolumeName(const std::string& drive)
	{
		return convert(readVolumeName(convert(drive)));
	}

	//Gets free space 
	void readFreeSpace(const std::wstring& drive, uint64_t& totalNumberOfBytes, uint64_t& totalNumberOfFreeBytes)
	{
		__int64 freeBytes;
		//WinAPI
		GetDiskFreeSpaceEx(
			drive.c_str(),
			reinterpret_cast<PULARGE_INTEGER>(&freeBytes),
			reinterpret_cast<PULARGE_INTEGER>(&totalNumberOfBytes),
			reinterpret_cast<PULARGE_INTEGER>(&totalNumberOfFreeBytes));
	}

	void readFreeSpace(const std::string& drive, uint64_t& totalNumberOfBytes, uint64_t& totalNumberOfFreeBytes)
	{
		readFreeSpace(convert(drive), totalNumberOfBytes, totalNumberOfFreeBytes);
	}
}}}
