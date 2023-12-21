// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "include/Workflow/Media/iusbmonitor.h"
#include <gmock/gmock.h>

using namespace Philips::Workflow::Media;

class MockMonitor : public IUsbMonitor
{
public:
	MockMonitor() : IUsbMonitor()
	{
		_usbs = std::vector<std::unique_ptr<Philips::Workflow::Media::IUsb>>();

		ON_CALL(*this, usbs()).WillByDefault(testing::ReturnRef(_usbs));
	}

	~MockMonitor() = default;

	MOCK_METHOD(std::vector<std::unique_ptr<Philips::Workflow::Media::IUsb>>&, usbs, (), (const, override));

private:
	std::vector<std::unique_ptr<Philips::Workflow::Media::IUsb>> _usbs;
};

typedef testing::NiceMock<MockMonitor> MockUsbMonitor;