// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/Media/usbmonitor.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class UsbMonitorTest : public ::testing::Test
{
};

TEST_F(UsbMonitorTest, When_UsbsCalled_Then_UsbsIsExecutedWithoutAnyException)
{
	const Philips::Workflow::Media::UsbMonitor monitor;
	ASSERT_NO_THROW(monitor.usbs()); //TICS !ERR#017: Catch is part of Google test.
}