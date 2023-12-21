// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "Workflow/stringconverter.h"
#include <codecvt>

namespace Philips { namespace Workflow {

	std::wstring convert(const std::string& string)
	{
		static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(string);
	}

	std::string convert(const std::wstring& string)
	{
		static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(string);
	}
}}