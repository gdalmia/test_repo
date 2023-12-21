// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include <string>

namespace Philips { namespace Workflow {

	std::wstring convert(const std::string& string);

	std::string convert(const std::wstring& string);
}}