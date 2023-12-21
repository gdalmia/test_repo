// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>
#include "Workflow/systemsettings.h"

using namespace Philips::Workflow;

class SystemSettingsTest : public testing::Test
{
};

void assertSettings(const SystemSettings& expected, const SystemSettings& actual)
{
	ASSERT_EQ(expected.systemType(), actual.systemType());
	ASSERT_EQ(expected.integrationType(), actual.integrationType());
	ASSERT_EQ(expected.licensePluginPath(), actual.licensePluginPath());
}

TEST_F(SystemSettingsTest, When_Created_Then_SystemTypeIsSetToNone)
{
	const SystemSettings settings;
	ASSERT_EQ(settings.systemType(), SystemSettings::SystemType::None);
}

TEST_F(SystemSettingsTest, When_Created_Then_IntegrationTypeIsSetToNone)
{
	const SystemSettings settings;
	ASSERT_EQ(settings.integrationType(), SystemSettings::IntegrationType::None);
}

TEST_F(SystemSettingsTest, When_Created_Then_LicensePluginPathIsEmpty)
{
	const SystemSettings settings;
	ASSERT_EQ(settings.licensePluginPath(), std::filesystem::path());
}

TEST_F(SystemSettingsTest, When_SystemTypeIsSet_Then_ReturnTheSystemType)
{
	const SystemSettings settings;
	settings.setSystemType(SystemSettings::SystemType::Integrated);
	ASSERT_EQ(settings.systemType(), SystemSettings::SystemType::Integrated);
}

TEST_F(SystemSettingsTest, When_IntegrationTypeIsSet_Then_ReturnTheIntegrationType)
{
	const SystemSettings settings;
	settings.setIntegrationType(SystemSettings::IntegrationType::Philips);
	ASSERT_EQ(settings.integrationType(), SystemSettings::IntegrationType::Philips);
}

TEST_F(SystemSettingsTest, When_LicensePluginPathIsSet_Then_ReturnLicensePluginPath)
{
	const SystemSettings settings;
	const auto path = std::filesystem::path(L"C:\\path");
	settings.setLicesnePluginPath(path);
	ASSERT_EQ(settings.licensePluginPath(), path);
}

TEST_F(SystemSettingsTest, Given_FirstSettingsIsCreated_When_SecondIsCreated_Then_SecondIsSameAsFirst)
{
	const SystemSettings first;
	const SystemSettings second;

	assertSettings(first, second);
}

TEST_F(SystemSettingsTest, Given_FirstSettingsIsInitialized_When_SecondIsCreated_Then_SecondIsSameAsFirst)
{
	const SystemSettings first;
	first.setSystemType(SystemSettings::SystemType::Mobile);
	first.setIntegrationType(SystemSettings::IntegrationType::Competitor);
	const auto path = std::filesystem::path(L"C:\\path");
	first.setLicesnePluginPath(path);

	const SystemSettings second;

	assertSettings(first, second);
}