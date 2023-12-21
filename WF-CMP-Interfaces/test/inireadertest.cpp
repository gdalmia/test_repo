// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>
#include "Workflow/inireader.h"

#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <sstream>
#include <locale>
#include "Workflow/settings.h"
#include <Trace.h>

using namespace Philips::Workflow;

class IniWriter
{
public:

	explicit IniWriter(const std::filesystem::path& path)
	: _path(path)
	{
	}

	void set(const wchar_t *group, const wchar_t *name, const wchar_t *value) const
	{
		if (!checkParams(group, name)) return;

		if (value != nullptr)
		{
			auto str = stringToIni(value);
			WritePrivateProfileString(group, name, str.c_str(), _path.c_str());
		}
	}

	void set(const wchar_t *group, const wchar_t *name, int value) const
	{
		if (!checkParams(group, name)) return;

		wchar_t result[64];
		_itow_s(value, result, 10);
		WritePrivateProfileString(group, name, result, _path.c_str());
	}

	void set(const wchar_t *group, const wchar_t *name, bool value) const
	{
		if (!checkParams(group, name)) return;

		WritePrivateProfileString(group, name, value ? L"true" : L"false", _path.c_str());
	}

	void set(const wchar_t *group, const wchar_t *name, double value) const
	{
		if (!checkParams(group, name)) return;

		std::wostringstream result;
		result.imbue(std::locale("C"));
		result << value;

		WritePrivateProfileString(group, name, result.str().c_str(), _path.c_str());
	}

	void set(const wchar_t *group, const wchar_t *name, float value) const
	{
		if (!checkParams(group, name)) return;

		set(group, name, double(value));
	}

private:
	std::wstring stringToIni(const std::wstring &value) const
	{
		std::wstring result;
		result.reserve(value.size() * 2);

		for (auto &i : value)
		{
			if (i < L' ' || i == L';' || i == L'#' || i == L'\'' || i == L'\"' || i == L'%' || i > L'z')
			{
				result.push_back(L'%');
				const auto v1 = static_cast<unsigned char>((i >> 12) & 0x0F);  result.push_back((v1 < 10) ? v1 + L'0' : v1 - 10 + L'A');
				const auto v2 = static_cast<unsigned char>((i >> 8) & 0x0F);  result.push_back((v2 < 10) ? v2 + L'0' : v2 - 10 + L'A');
				const auto v3 = static_cast<unsigned char>((i >> 4) & 0x0F);  result.push_back((v3 < 10) ? v3 + L'0' : v3 - 10 + L'A');
				const auto v4 = static_cast<unsigned char>((i >> 0) & 0x0F);  result.push_back((v4 < 10) ? v4 + L'0' : v4 - 10 + L'A');
			}
			else result.push_back(i);
		}

		return result;
	}

	bool checkParams(const wchar_t *group, const wchar_t *name) const
	{
		return ((group != nullptr) && (name != nullptr));
	}

private:
	std::filesystem::path _path;
};

class IniReaderTest : public testing::Test
{
public:
	IniReaderTest()
		: _path(std::filesystem::temp_directory_path() / L"testinifile.ini")
	{}

	void SetUp() override
	{
		remove();
	}

	void TearDown() override
	{
		remove();
	}

	void remove() const
	{
		std::error_code error;
		if (!std::filesystem::remove(_path, error)) TraceError() << L"File remove failed. Error->" << error.message();
	}

	std::filesystem::path path() const
	{
		return _path;
	}

private:
	std::filesystem::path _path;
};

TEST_F(IniReaderTest, Given_FileNotExist_WhenReadIscalled_ReturnDefault)
{
	IniReader reader(path());

	ASSERT_EQ(reader.sections(), std::vector<std::wstring>());
	ASSERT_EQ(reader.get(L"TestSection", L"TestKey", L"Default") , L"Default");
	ASSERT_EQ(reader.get(L"TestSection", L"TestKey", false), false);
	ASSERT_EQ(reader.get(L"TestSection", L"TestKey", 25), 25);
	ASSERT_DOUBLE_EQ(reader.get(L"TestSection", L"TestKey", double(30.0)), double(30.0));
	ASSERT_FLOAT_EQ(reader.get(L"TestSection", L"TestKey", float(45.0)), float(45.0));
}

TEST_F(IniReaderTest, Given_IniFileWithStringValue_WhenReadIscalled_ReturnStringValue)
{
	TraceScope();
	const auto value = L"TestValue";

	IniWriter writer(path());
	writer.set(L"TestSection", L"TestKey", value);

	IniReader reader(path());

	const auto result = reader.get(L"TestSection", L"TestKey", L"Default");
	ASSERT_EQ(result, value);
}

TEST_F(IniReaderTest, Given_IniFileWithNonASCIIStringValue_WhenReadIscalled_ReturnValue)
{
	TraceScope();
	const auto value = L"©Test%Value"; 

	IniWriter writer(path());
	writer.set(L"TestSection", L"TestKey", value);

	IniReader reader(path());

	const auto result = reader.get(L"TestSection", L"TestKey", L"Default");
	ASSERT_EQ(result, value);
}

TEST_F(IniReaderTest, Given_IniFileWithIntValue_WhenReadIscalled_ReturnIntValue)
{
	TraceScope();
	const auto value = 20;

	IniWriter writer(path());
	writer.set(L"TestSection", L"TestKey", value);

	IniReader reader(path());

	const auto result = reader.get(L"TestSection", L"TestKey", 0);
	ASSERT_EQ(result, value);
}

TEST_F(IniReaderTest, Given_IniFileWithBoolFalseValue_WhenReadIscalled_ReturnBoolValueFalse)
{
	TraceScope();
	const auto value = false;

	IniWriter writer(path());
	writer.set(L"TestSection", L"TestKey", value);

	IniReader reader(path());

	const auto result = reader.get(L"TestSection", L"TestKey", true);
	ASSERT_EQ(result, value);
}

TEST_F(IniReaderTest, Given_IniFileWithBoolTrueValue_WhenReadIscalled_ReturnBoolValueTrue)
{
	TraceScope();
	const auto value = true;

	IniWriter writer(path());
	writer.set(L"TestSection", L"TestKey", value);

	IniReader reader(path());

	const auto result = reader.get(L"TestSection", L"TestKey", false);
	ASSERT_EQ(result, value);
}

TEST_F(IniReaderTest, Given_IniFileWithDoubleValue_WhenReadIscalled_ReturnDoubleValue)
{
	TraceScope();
	const double value = 20.0;
	const double defaultValue = 0.0;

	IniWriter writer(path());
	writer.set(L"TestSection", L"TestKey", value);

	IniReader reader(path());

	const auto result = reader.get(L"TestSection", L"TestKey", defaultValue);
	ASSERT_DOUBLE_EQ(result, value);
}

TEST_F(IniReaderTest, Given_IniFileWithFloatValue_WhenReadIscalled_ReturnFloatValue)
{
	TraceScope();
	const float value = 20.0;
	const float defaultValue = 0.0;

	IniWriter writer(path());
	writer.set(L"TestSection", L"TestKey", value);

	IniReader reader(path());

	const auto result = reader.get(L"TestSection", L"TestKey", defaultValue);
	ASSERT_FLOAT_EQ(result, value);
}

TEST_F(IniReaderTest, Given_IniFileWithMultipleSections_WhenGetSectionsIsCalled_AllSectionsAreReturned)
{
	TraceScope();
	const auto value = L"TestValue";

	IniWriter writer(path());
	writer.set(L"TestSection1", L"TestKey", value);
	writer.set(L"TestSection2", L"TestKey", value);
	writer.set(L"TestSection3", L"TestKey", value);

	IniReader reader(path());

	const auto result = reader.sections();
	ASSERT_EQ(int(result.size()), 3);
	ASSERT_EQ(result[0], L"TestSection1");
	ASSERT_EQ(result[1], L"TestSection2");
	ASSERT_EQ(result[2], L"TestSection3");
}

