// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include <string>

namespace Philips { namespace Workflow { namespace Media { 

	//! Reads the volume name of the drive
	std::wstring readVolumeName(const std::wstring& drive);

	//! Reads the volume name of the drive
	std::string readVolumeName(const std::string& drive);

	//! Reads disk space of the drive
	void readFreeSpace(const std::wstring& drive, uint64_t& totalNumberOfBytes, uint64_t& totalNumberOfFreeBytes);

	//! Reads disk space of the drive
	void readFreeSpace(const std::string& drive, uint64_t& totalNumberOfBytes, uint64_t& totalNumberOfFreeBytes);
	
}}}
