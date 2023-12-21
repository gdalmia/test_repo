// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/settings.h"
#include <gtest/gtest.h>

using namespace Philips::Workflow;

class SettingsTest : public testing::Test
{
};

void assertSettings(const Settings& expected, const Settings& actual)
{
	ASSERT_EQ(expected.path(), actual.path());
	ASSERT_EQ(expected.test(), actual.test());
	ASSERT_EQ(expected.dateTimeFormat(), actual.dateTimeFormat());
}

TEST_F(SettingsTest, When_SettingsIsCreated_Then_PathFileNameIsSetToDataHandler)
{
	const auto settings = Settings();
	ASSERT_EQ(settings.path().filename().string(), "DataHandler");
}

TEST_F(SettingsTest, When_SettingsIsCreated_Then_PathExists)
{
	const auto settings = Settings();
	ASSERT_TRUE(std::filesystem::exists( settings.path()));
}

TEST_F(SettingsTest, When_SettingsIsCreated_Then_TestModeIsFalseByDefault)
{
	const auto settings = Settings();
	ASSERT_FALSE(settings.test());
}

TEST_F(SettingsTest, When_SettingsIsCreated_Then_DateTimeFormatIsDefault) //"dd/MM/yyyy"
{
	const auto settings = Settings();
	ASSERT_EQ(settings.dateTimeFormat(), "dd/MM/yyyy");
}

TEST_F(SettingsTest, Given_FirstSettingsIsCreated_When_SecondIsCreatedWithoutParameters_Then_SecondIsSameAsFirstSettings)
{
	const auto first = Settings();

	const auto second = Settings();
	assertSettings(first, second);
}

TEST_F(SettingsTest, Given_FirstSettingsIsCreated_When_SecondIsCreatedWithParameters_Then_SecondIsSameAsFirstSettings)
{
	const auto first = Settings();
	
	const auto test{ true };
	const auto second = Settings(std::experimental::filesystem::temp_directory_path(), test);
	assertSettings(first, second);
}

TEST_F(SettingsTest, When_SettingsIsCreatedWithTestParameters_Then_ReturnsSetPath)
{
	const auto path = std::experimental::filesystem::temp_directory_path();
	const auto settings = Settings(path, true);

	ASSERT_EQ(settings.path().filename().string(), "DataHandler");
	ASSERT_EQ(settings.path().parent_path().append("\\"), path);
}

TEST_F(SettingsTest, When_SettingsIsCreatedWithTestParameters_Then_PathExists)
{
	const auto path = std::experimental::filesystem::temp_directory_path();
	const auto settings = Settings(path, true);

	ASSERT_TRUE(std::filesystem::exists(settings.path()));
}

TEST_F(SettingsTest, When_SettingsIsCreatedWithTestParameters_Then_ReturnsSetTestMode)
{
	const auto test{ true };
	const auto settings = Settings(std::experimental::filesystem::temp_directory_path(), test);

	ASSERT_EQ(settings.test(), test);
}

TEST_F(SettingsTest, When_SettingsIsCreatedWithTestParameters_Then_ReturnsSetDateTimeFormat)
{
	const auto settings = Settings(std::experimental::filesystem::temp_directory_path(), true);

	const auto format{ "yyyy/MM/dd" };
	settings.setDateTimeFormat(format);

	ASSERT_EQ(settings.dateTimeFormat(), format);
}

TEST_F(SettingsTest, Given_FirstSettingsIsCreatedWithTestParameters_When_SecondIsCreatedWithoutParameters_Then_SecondIsSameAsFirstSettings)
{
	const auto path = std::experimental::filesystem::temp_directory_path();
	const auto test{ true };
	const auto first = Settings(path, test);
	const auto format{ "yyyy/MM/dd" };
	first.setDateTimeFormat(format);

	const auto second = Settings();
	
	assertSettings(first, second);
}

TEST_F(SettingsTest, Given_FirstSettingsIsCreatedWithTestParameters_When_SecondIsCreatedWithParameters_Then_SecondIsSameAsFirstSettings)
{
	const auto path = std::experimental::filesystem::temp_directory_path();
	const auto test{ true };
	const auto first = Settings(path, test);
	const auto format{ "yyyy/MM/dd" };
	first.setDateTimeFormat(format);
	
	const auto second = Settings(path / "Test", false);
	
	assertSettings(first, second);
}

TEST_F(SettingsTest, Given_TwoSettings_When_DateTimeFormatIsChangedInSecond_Then_FirstDateTimeFormatAlsoChanges)
{
	const auto path = std::experimental::filesystem::temp_directory_path();
	const auto test{ true };
	const auto first = Settings(path, test);
	

	const auto second = Settings(path / "Test", false);
	const auto format{ "yyyy/MM/dd" };
	second.setDateTimeFormat(format);
	
	ASSERT_EQ(second.dateTimeFormat(), first.dateTimeFormat());
}

TEST_F(SettingsTest, When_SettingsIsCreated_Then_TestHeightUnitIsCmByDefault)
{
	const auto settings = Settings();
	ASSERT_EQ(settings.getHeightUnit(), Settings::HeightUnit::Cm);
}

TEST_F(SettingsTest, When_SettingsIsCreatedAndHeightUnitSetToInch_Then_getHeightUnitReturnsInch)
{
	auto settings = Settings();
	settings.setHeightUnit(Settings::HeightUnit::Inch);
	ASSERT_EQ(settings.getHeightUnit(), Settings::HeightUnit::Inch);
}

TEST_F(SettingsTest, When_SettingsIsCreated_Then_TestWeightUnitIsKilogramByDefault)
{
	const auto settings = Settings();
	ASSERT_EQ(settings.getWeightUnit(), Settings::WeightUnit::Kilogram);
}

TEST_F(SettingsTest, When_SettingsIsCreatedAndWeightUnitSetToPound_Then_getWeightUnitReturnsPound)
{
	auto settings = Settings();
	settings.setWeightUnit(Settings::WeightUnit::Pound);
	ASSERT_EQ(settings.getWeightUnit(), Settings::WeightUnit::Pound);
}

TEST_F(SettingsTest, When_SettingsIsCreated_Then_DateFormatIsDefault) //"dd/MM/yyyy"
{
	const auto settings = Settings();
	ASSERT_EQ(settings.dateFormat(), "dd/MM/yyyy");
}

TEST_F(SettingsTest, When_SettingsIsCreatedAndDateFormatIsSet_Then_DateFromatReturnsUpdatedFormat)
{
	auto settings = Settings();
	settings.setDateFormat("yyyyMMdd");
	ASSERT_EQ(settings.dateFormat(), "yyyyMMdd");
}

TEST_F(SettingsTest, When_SettingsIsCreated_Then_TimeFormatIsDefault) //"hh:mm:ss tt"
{
	const auto settings = Settings();
	ASSERT_EQ(settings.timeFormat(), "hh:mm:ss tt");
}

TEST_F(SettingsTest, When_SettingsIsCreatedAndTimeFormatIsSet_Then_TimeFromatReturnsUpdatedFormat)
{
	auto settings = Settings();
	settings.setTimeFormat("HHmmss");
	ASSERT_EQ(settings.timeFormat(), "HHmmss");
}

TEST_F(SettingsTest, When_SettingsIsCreated_Then_LanguageIsUSByDefault)
{
	const Settings settings;
	ASSERT_EQ(settings.language(), Settings::Language::US);
}

TEST_F(SettingsTest, When_SettingsIsCreatedAndLanguageIsSetToES_Then_LanguageReturnsES)
{
	const Settings settings;
	settings.setLanguage(Settings::Language::ES);
	ASSERT_EQ(settings.language(), Settings::Language::ES);
}

TEST_F(SettingsTest, When_SettingsIsCreated_Then_ShowIdeographicIsFalseByDefault)
{
	const Settings settings;
	ASSERT_FALSE(settings.showIdeographic());
}

TEST_F(SettingsTest, When_SettingsIsCreatedAndShowIdeographicIsSetToTrue_Then_ShowIdeographicReturnsTrue)
{
	const Settings settings;
	settings.setShowIdeographic(true);
	ASSERT_TRUE(settings.showIdeographic());
}
