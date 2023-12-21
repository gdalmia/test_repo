// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/iniwriter.h"

#include <Trace.h>
#include <sstream>
#include <Windows.h>

namespace Philips { namespace Workflow {

	namespace
	{
		inline unsigned char convert(const unsigned char& value)
		{
			return (value < 10) ? value + L'0' : value - 10 + L'A';
		}

		void convert(const wchar_t& value, std::wstring& result)
		{
			if (value < L' ' || value == L';' || value == L'#' || value == L'\'' || value == L'\"' || value == L'%' || value > L'z')
			{
				result.push_back(L'%');
				const auto v1 = static_cast<unsigned char>((value >> 12) & 0x0F);	result.push_back(convert(v1));
				const auto v2 = static_cast<unsigned char>((value >> 8) & 0x0F);	result.push_back(convert(v2));
				const auto v3 = static_cast<unsigned char>((value >> 4) & 0x0F);	result.push_back(convert(v3));
				const auto v4 = static_cast<unsigned char>((value >> 0) & 0x0F);	result.push_back(convert(v4));
			}
			else
			{
				result.push_back(value);
			}
		}

		std::wstring stringToIni(const std::wstring &value)
		{
			std::wstring result;
			result.reserve(value.size() * 2);

			for (const auto& character : value)
			{
				convert(character, result);
			}

			return result;
		}
		
		inline bool validate(const wchar_t *group, const wchar_t *name)
		{
			return (group != nullptr && name != nullptr);
		}

	}

	IniWriter::IniWriter(const std::filesystem::path& path)
		: _path(path)
	{
		TraceScope() << "File: " << _path;
	}

	IniWriter::~IniWriter() = default;

	bool IniWriter::set(const wchar_t *group, const wchar_t *name, const wchar_t *value) const
	{
		if (!validate(group, name))
		{
			TraceError() << "Either section or key is null. File name: " << _path;
			return false;
		}

		if (value == nullptr)
		{
			TraceError() << "Value is null. File name: " << _path;
			return false;
		}

		auto str = stringToIni(value);
		if (WritePrivateProfileString(group, name, str.c_str(), _path.c_str()) == static_cast<BOOL>(0))
		{
			TraceError() << "Fail to write to the file. " << "System Error Code: " << GetLastError() << std::endl
				<< "File name : " << _path << std::endl
				<< "Section: " << group << std::endl
				<< "Key: " << name << std::endl
				<< "Value: " << value << std::endl;

			return false;
		}

		return true;
	}

	bool IniWriter::set(const wchar_t *group, const wchar_t *name, int value) const
	{
		if (!validate(group, name))
		{
			TraceError() << "Either section or key is null. File name: " << _path;
			return false;
		}

		wchar_t result[64];
		_itow_s(value, result, 10);
		if (WritePrivateProfileString(group, name, result, _path.c_str()) == static_cast<BOOL>(0))
		{
			TraceError() << "Fail to write to the file. " << "System Error Code: " << GetLastError() << std::endl
				<< "File name : " << _path << std::endl
				<< "Section: " << group << std::endl
				<< "Key: " << name << std::endl
				<< "Value: " << value << std::endl;

			return false;
		}

		return true;
	}

	bool IniWriter::set(const wchar_t *group, const wchar_t *name, bool value) const
	{
		if (!validate(group, name))
		{
			TraceError() << "Either section or key is null. File name: " << _path;
			return false;
		}

		if (WritePrivateProfileString(group, name, value ? L"true" : L"false", _path.c_str()) == static_cast<BOOL>(0))
		{
			TraceError() << "Fail to write to the file. " << "System Error Code: " << GetLastError() << std::endl
				<< "File name : " << _path << std::endl
				<< "Section: " << group << std::endl
				<< "Key: " << name << std::endl
				<< "Value: " << value << std::endl;

			return false;
		}

		return true;
	}

	bool IniWriter::set(const wchar_t *group, const wchar_t *name, double value) const
	{
		if (!validate(group, name))
		{
			TraceError() << "Either section or key is null. File name: " << _path;
			return false;
		}

		std::wostringstream result;
		result.imbue(std::locale("C"));
		result << value;

		if (WritePrivateProfileString(group, name, result.str().c_str(), _path.c_str()) == static_cast<BOOL>(0))
		{
			TraceError() << "Fail to write to the file. " << "System Error Code: " << GetLastError() << std::endl
				<< "File name : " << _path << std::endl
				<< "Section: " << group << std::endl
				<< "Key: " << name << std::endl
				<< "Value: " << value << std::endl;

			return false;
		}

		return true;
	}

	bool IniWriter::set(const wchar_t *group, const wchar_t *name, float value) const
	{
		return set(group, name, double(value));
	}

	bool IniWriter::remove(const wchar_t * group) const
	{
		if (group == nullptr)
		{
			TraceError() << "Section is null. File name: " << _path;
			return false;
		}

		if (WritePrivateProfileString(group, nullptr, nullptr, _path.c_str()) == static_cast<BOOL>(0))
		{
			TraceError() << "Fail to remove section from the file. " << "System Error Code: " << GetLastError() << std::endl
				<< "File name : " << _path << std::endl
				<< "Section: " << group << std::endl;

			return false;
		}
		return true;
	}

}}