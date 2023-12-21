// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/inireader.h"

#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <sstream>
#include <locale>

namespace
{
	void convert(wchar_t& value)
	{
		if (value >= '0' && value <= '9') value = value - '0';	
		if (value >= 'a' && value <= 'f') value = value - 'a' + 10;
	}

	void convert(const std::wstring& value, int index, std::wstring& result)
	{
		auto v1 = wchar_t(tolower(value[index + 1]));	convert(v1);
		auto v2 = wchar_t(tolower(value[index + 2]));	convert(v2);
		auto v3 = wchar_t(tolower(value[index + 3]));	convert(v3);
		auto v4 = wchar_t(tolower(value[index + 4]));	convert(v4);

		result.push_back((v1 << 12) | (v2 << 8) | (v3 << 4) | (v4 << 0));
	}

	std::wstring iniToString(const std::wstring &value)
	{
		std::wstring result;
		result.reserve(value.size());
		auto i = 0;
		while (i < int(value.size()))
		{
			if (value[i] == 0) break;

			if (i < int(value.size()) - 4 && value[i] == '%')
			{
				convert(value, i, result);
				i += 4;
			}
			else
			{
				result.push_back(value[i]);
			}
			i++;
		}

		return result;
	}
}

namespace Philips { namespace Workflow {

	IniReader::IniReader(const std::filesystem::path & path)
		: _path(path)
		, _exists(std::filesystem::exists(path))
	{
	}

	std::vector<std::wstring> IniReader::sections() const
	{
		if (!_exists) return std::vector<std::wstring>();

		std::vector<std::wstring> result;

		wchar_t sections[4096] = L"";
		if (GetPrivateProfileString(nullptr, nullptr, nullptr, sections, 4096, _path.c_str()) == 0) return result;

		auto section = sections;

		while (*section != 0)
		{
			result.emplace_back(section);
			section += wcslen(section) + 1;
		}

		return result;
	}

	std::wstring IniReader::get(const wchar_t* group, const wchar_t * name, const wchar_t * def) const
	{
		if (!_exists) return def;

		wchar_t result[4096] = L"";
		if (GetPrivateProfileString(group, name, L"", result, 4096, _path.c_str()) == 0) return def;

		return iniToString(result);
	}

	int IniReader::get(const wchar_t* group, const wchar_t * name, int def) const
	{
		if (!_exists) return def;

		wchar_t result[64] = L"";
		if (GetPrivateProfileString(group, name, L"", result, 64, _path.c_str()) == 0)
		{
			if (result[0] == 0) return def;
		}

		return _wtoi(result);
	}

	bool IniReader::get(const wchar_t* group, const wchar_t * name, bool def) const
	{
		if (!_exists) return def;

		wchar_t result[64] = L"";

		if (GetPrivateProfileString(group, name, L"", result, 64, _path.c_str()) == 0)
		{
			if (result[0] == 0) return def;
		}
		return (result[0] == L't' || result[0] == L'T');
	}

	double IniReader::get(const wchar_t* group, const wchar_t * name, double def) const
	{
		if (!_exists) return def;

		wchar_t result[64] = L"";

		if (GetPrivateProfileString(group, name, L"", result, 64, _path.c_str()) == 0)
		{
			if (result[0] == 0) return def;
		}

		auto tempDef = def;
		std::wistringstream str(result);
		str.imbue(std::locale("C"));
		str >> tempDef;

		return tempDef;
	}

	float IniReader::get(const wchar_t * group, const wchar_t * name, float def) const
	{
		if (!_exists) return def;

		return static_cast<float>(get(group, name, static_cast<double>(def)));
	}


}}