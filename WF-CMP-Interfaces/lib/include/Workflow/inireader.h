// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include <filesystem>

namespace std { namespace filesystem = experimental::filesystem; }

namespace Philips { namespace Workflow {

	//! This class provide a mechanism to read the ini file.
	class IniReader
	{
	public:
		explicit IniReader(const std::filesystem::path& path);
		~IniReader() = default;

		//! Get all sections in the file.
		std::vector<std::wstring> sections() const;

		//! Get the string value for the key defined in the given section else return the default value.
		//! @param group Contain the group to which the key belongs to
		//! @param name Contain the name of the key
		//! @param def Contain the default value to return in case the key or file is not present.
		std::wstring get(const wchar_t *group, const wchar_t *name, const wchar_t* def) const;

		//! Get the int value for the key defined in the given section else return the default value.
		//! @param group Contain the group to which the key belongs to
		//! @param name Contain the name of the key
		//! @param def Contain the default value to return in case the key is not present.
		int get(const wchar_t *group, const wchar_t *name, int def) const;

		//! Get the bool value for the key defined in the given section else return the default value.
		//! @param group Contain the group to which the key belongs to
		//! @param name Contain the name of the key
		//! @param def Contain the default value to return in case the key is not present.
		bool get(const wchar_t *group, const wchar_t *name, bool def) const;

		//! Get the double value for the key defined in the given section else return the default value.
		//! @param group Contain the group to which the key belongs to
		//! @param name Contain the name of the key
		//! @param def Contain the default value to return in case the key is not present.
		double get(const wchar_t *group, const wchar_t *name, double def) const;

		//! Get the float value for the key defined in the given section else return the default value.
		//! @param group Contain the group to which the key belongs to
		//! @param name Contain the name of the key
		//! @param def Contain the default value to return in case the key is not present.
		float get(const wchar_t *group, const wchar_t *name, float def) const;

	private:
		std::filesystem::path	_path;
		bool					_exists;
	};
}}
