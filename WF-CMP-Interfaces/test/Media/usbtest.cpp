// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Workflow/Media/usb.h>

namespace Philips { namespace Workflow { namespace Media {


	class UsbTest : public ::testing::Test
	{
	};

	TEST_F(UsbTest, When_UsbCreatedWithNoDrive_Then_DefaultValuesAreSet)
	{
		Usb usb;
		ASSERT_EQ(usb.drive(), "");
		ASSERT_EQ(usb.volumeName(), "USB");
		ASSERT_EQ(usb.totalNumberOfBytes(), 0);
		ASSERT_EQ(usb.totalNumberOfFreeBytes(), 0);
		ASSERT_FALSE(usb.locked());
	}

	TEST_F(UsbTest, When_UsbCreatedWithDrive_Then_ParatemerConstructorIsCalled)
	{
		Usb usb("P:\\");
		ASSERT_EQ(usb.drive(), "P:\\");
	}

	TEST_F(UsbTest, When_UsbCreatedWithDrive_Then_DefaultVolumeNameIsSet)
	{
		Usb usb("P:\\");
		ASSERT_EQ(usb.volumeName(), "USB");
	}

	TEST_F(UsbTest, When_UsbCreatedWithDifferentDrive_Then_NotEqualToOperatorIsCalled)
	{
		const Usb usb("b");
		const Usb usb1("a");

		ASSERT_NE(usb, usb1);
	}

	TEST_F(UsbTest, When_UsbCreatedCopied_Then_EqualToOperatorIsCalled)
	{
		Usb usb("P:\\");
		const Usb usb1(usb);

		ASSERT_EQ(usb, usb1);
	}

	TEST_F(UsbTest, Given_UsbCreatedWithAllParameters_When_VolumeNameIsNotEmpty_Then_DefaultVolumeNameIsSetAsExpected)
	{
		const auto volumeName = "name";
		Usb usb("P:\\", volumeName, 0, 0);
		ASSERT_EQ(usb.volumeName(), volumeName);
	}

	TEST_F(UsbTest, Given_UsbCreatedWithAllParameters_When_VolumeNameIsEmpty_Then_DefaultVolumeNameIsSetAsDefaultName)
	{
		Usb usb("P:\\", "", 0, 0);
		ASSERT_EQ(usb.volumeName(), "USB");
	}

	TEST_F(UsbTest, Given_NonExistingUsbDrive_When_UnlockIsCalled_Then_NoExceptionsAreThrown)
	{
		Usb usb("P:\\", "", 0, 0, true);
		ASSERT_NO_THROW(usb.unlock("password")); //TICS !ERR#017: Catch is part of Google test.
	}

	TEST_F(UsbTest, Given_UsbDriveWithEmptyDrive_When_UnlockIsCalled_Then_ReturnsFalse)
	{
		Usb usb("", "", 0, 0, true);
		ASSERT_FALSE(usb.unlock("password")); 
	}

	TEST_F(UsbTest, Given_UnlockedUsb_When_UnlockIsCalled_Then_ReturnsFalse)
	{
		Usb usb("", "", 0, 0, false);
		ASSERT_FALSE(usb.unlock("password")); 
	}
	TEST_F(UsbTest, Given_Usb_When_Copied_Then_CopyConstructorIsCalled)
	{
		Usb usb("", "", 0, 0, false);
		Usb usb1(usb);
		
		ASSERT_EQ(usb1.drive(), "");
		ASSERT_EQ(usb1.volumeName(), "USB");
		ASSERT_EQ(usb1.totalNumberOfBytes(), 0);
		ASSERT_EQ(usb1.totalNumberOfFreeBytes(), 0);
		ASSERT_FALSE(usb1.locked());
	}

	TEST_F(UsbTest, Given_Usb_When_RecalculateSize_Then_VerifyNoThrow)
	{
		Usb usb("P:\\", "", 0, 0, true);
		ASSERT_NO_THROW(usb.recalculateSize()); //TICS !ERR#017: Catch is part of Google test.
	}

	TEST_F(UsbTest, Given_Usb_When_Ejected_Then_VerifyNoThrow)
	{
		Usb usb("P:\\", "", 0, 0, true);
		ASSERT_NO_THROW(usb.eject()); //TICS !ERR#017: Catch is part of Google test.
	}

}}}


