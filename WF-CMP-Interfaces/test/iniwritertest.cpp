// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>
#include "Workflow/iniwriter.h"
#include <Trace.h>

using namespace Philips::Workflow;

class IniWriterTest : public testing::Test
{
public:
	IniWriterTest()
		: _path(std::filesystem::temp_directory_path() / L"testinifile.ini")
	{}

	void SetUp() override
	{
		auto testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
		TraceMessage() << "Test start-> " << testInfo->test_case_name() << "." << testInfo->name();
		remove();
	}

	void TearDown() override
	{
		auto testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
		TraceMessage() << "Test end-> " << testInfo->test_case_name() << "." << testInfo->name();
		remove();
	}

	void remove() const
	{
		if (!std::filesystem::exists(_path)) return;

		std::error_code error;
		if (!std::filesystem::remove(_path, error))
		{
			TraceError() << L"File remove failed. Error->" << error.message() << std::endl
				<< "Path: " << _path;
		}
	}

	const std::filesystem::path& path() const
	{
		return _path;
	}

private:
	std::filesystem::path _path;
};

TEST_F(IniWriterTest, Given_FileNotExist_WhenWriteIscalled_WriteIsSuccess)
{
	const IniWriter writer(path());
	ASSERT_TRUE(writer.set(L"TestSection", L"TestKey", L"TestValue"));
}

TEST_F(IniWriterTest, When_SetCalledWithEmptyGroup_Then_ReturnFailure)
{
	const IniWriter writer(path());
	ASSERT_FALSE(writer.set(nullptr, L"TestKey1", L"TestValue"));
	ASSERT_FALSE(writer.set(nullptr, L"TestKey2", 1));
	ASSERT_FALSE(writer.set(nullptr, L"TestKey3", false));
	ASSERT_FALSE(writer.set(nullptr, L"TestKey4", double(12.567)));
	ASSERT_FALSE(writer.set(nullptr, L"TestKey5", float(16.57)));
}

TEST_F(IniWriterTest, When_SetCalledWithEmptyName_Then_ReturnFailure)
{
	const IniWriter writer(path());
	ASSERT_FALSE(writer.set(L"TestSection", nullptr, L"TestValue"));
	ASSERT_FALSE(writer.set(L"TestSection", nullptr, 1));
	ASSERT_FALSE(writer.set(L"TestSection", nullptr, false));
	ASSERT_FALSE(writer.set(L"TestSection", nullptr, double(12.567)));
	ASSERT_FALSE(writer.set(L"TestSection", nullptr, float(16.57)));
}

TEST_F(IniWriterTest, Given_InvalidFilePath_When_SetCalled_Then_ReturnFailure)
{
	const IniWriter writer(L"D\\InvalidFolder\\invalid.ini");
	ASSERT_FALSE(writer.set(L"TestSection", L"TestKey1", L"TestValue"));
	ASSERT_FALSE(writer.set(L"TestSection", L"TestKey2", 1));
	ASSERT_FALSE(writer.set(L"TestSection", L"TestKey3", false));
	ASSERT_FALSE(writer.set(L"TestSection", L"TestKey4", double(12.567)));
	ASSERT_FALSE(writer.set(L"TestSection", L"TestKey5", float(16.57)));
}

TEST_F(IniWriterTest, When_SetCalledWithEmptyStringValue_Then_ReturnFailure)
{
	const IniWriter writer(path());
	ASSERT_FALSE(writer.set(L"TestSection", L"TestKey", nullptr));
}

TEST_F(IniWriterTest, When_SetCalledWithValidStringValue_Then_ReturnSuccess)
{
	const IniWriter writer(path());
	ASSERT_TRUE(writer.set(L"TestSection", L"TestKey", L"TestValue"));
}

TEST_F(IniWriterTest, When_SetCalledForNonASCIIStringValue_Then_ReturnSuccess)
{
	const auto value = L"©Test%Value";

	const IniWriter writer(path());
	ASSERT_TRUE(writer.set(L"TestSection", L"TestKey", value));
}

TEST_F(IniWriterTest, When_SetCalledWithValidIntValue_Then_ReturnSuccess)
{
	const IniWriter writer(path());
	ASSERT_TRUE(writer.set(L"TestSection", L"TestKey", 1));
}

TEST_F(IniWriterTest, When_SetCalledWithValidBoolValue_Then_ReturnSuccess)
{
	const IniWriter writer(path());
	ASSERT_TRUE(writer.set(L"TestSection", L"TestKey", true));
}

TEST_F(IniWriterTest, When_SetCalledWithValidDoubleValue_Then_ReturnSuccess)
{
	const IniWriter writer(path());
	ASSERT_TRUE(writer.set(L"TestSection", L"TestKey", double(12.567)));
}

TEST_F(IniWriterTest, When_SetCalledWithValidFloatValue_Then_ReturnSuccess)
{
	const IniWriter writer(path());
	ASSERT_TRUE(writer.set(L"TestSection", L"TestKey", float(16.57)));
}

TEST_F(IniWriterTest, When_RemoveCalled_Then_ReturnSuccess)
{
	const IniWriter writer(path());
	ASSERT_TRUE(writer.set(L"TestSection", L"TestKey", float(16.57)));
	ASSERT_TRUE(writer.set(L"TestSection1", L"TestKey1", L"TestValue"));

	ASSERT_TRUE(writer.remove(L"TestSection1"));
}

TEST_F(IniWriterTest, When_RemoveCalledForEmptyGroup_Then_ReturnFailure)
{
	const IniWriter writer(path());
	ASSERT_FALSE(writer.remove(nullptr));
}

TEST_F(IniWriterTest, Given_InvalidFilePath_When_RemoveCalled_Then_ReturnFailure)
{
	const IniWriter writer(L"D\\InvalidFolder\\invalid.ini");
	ASSERT_FALSE(writer.remove(L"TestSection"));
}
