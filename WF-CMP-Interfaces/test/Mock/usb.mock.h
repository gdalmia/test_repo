// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include <Workflow/Media/usb.h>
#include <gmock/gmock.h>
#include <string>

namespace Philips { namespace Workflow { namespace Test { namespace Interoperability { namespace Mock {
	
	class MockUsb : public Workflow::Media::IUsb
	{
	public:
		MOCK_METHOD(bool, unlock, (const std::string&), (override));
		MOCK_METHOD(std::string, drive, (), (const, override));
		MOCK_METHOD(std::string, volumeName, (), (const, override));
		MOCK_METHOD(uint64_t, totalNumberOfBytes, (), (const, override));
		MOCK_METHOD(uint64_t, totalNumberOfFreeBytes, (), (const, override));
		MOCK_METHOD(bool, locked, (), (const, override));
		MOCK_METHOD(void, recalculateSize, (), (override));
	};
	
}}}}}
