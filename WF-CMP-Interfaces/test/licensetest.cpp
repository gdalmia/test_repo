// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Workflow/license.h"
#include "licensefactory.mock.h"
#include "platformlicense.mock.h"

using namespace Philips::Workflow;

class LicenseTest : public ::testing::Test
{

};

TEST_F(LicenseTest, Given_License_When_FeatureIsAvailable_Then_GetsAvailableStatus)
{
	PlatformLicenseMock platformLicense;
	EXPECT_CALL(platformLicense, featureAvailable).WillOnce(testing::Return(true));
	LicenseFactoryMock licenseFactory(&platformLicense, 1);
	License license(licenseFactory);
	
	ASSERT_TRUE(license.available("Coronary"));
}

TEST_F(LicenseTest, Given_License_When_FeatureIsNotAvailable_Then_GetsAvailableStatus)
{
	PlatformLicenseMock platformLicense;
	EXPECT_CALL(platformLicense, featureAvailable).WillOnce(testing::Return(false));
	LicenseFactoryMock licenseFactory(&platformLicense, 1);
	License license(licenseFactory);

	ASSERT_FALSE(license.available("Coronary"));
}

TEST_F(LicenseTest, Given_License_When_LicenseIsNotCreated_Then_ReturnFalse)
{
	PlatformLicenseMock platformLicense;
	LicenseFactoryMock licenseFactory(&platformLicense, 0);
	License license(licenseFactory);

	ASSERT_FALSE(license.available("Coronary"));
}