// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include <filesystem>

namespace std { namespace filesystem = experimental::filesystem; }

namespace Philips { namespace Workflow {

	//! This class provide the mechanism to write to ini file.
	class IniWriter
	{
	public:
		explicit IniWriter(const std::filesystem::path& path);
		~IniWriter();

		//! Set the string value for the key defined in the given section.
		//! @param group Contain the group to which the key belongs to
		//! @param name Contain the name of the key
		//! @param value Contain the value of the key.
		//! @return false when the group or name or value is null, or the write to the file fails.
		bool set(const wchar_t *group, const wchar_t *name, const wchar_t *value) const;

		//! Set the integer value for the key defined in the given section.
		//! @param group Contain the group to which the key belongs to
		//! @param name Contain the name of the key
		//! @param value Contain the value of the key.
		//! @return false when the group or name is null, or the write to the file fails.
		bool set(const wchar_t *group, const wchar_t *name, int value) const;

		//! Set the bool value for the key defined in the given section.
		//! @param group Contain the group to which the key belongs to
		//! @param name Contain the name of the key
		//! @param value Contain the value of the key.
		//! @return false when the group or name is null, or the write to the file fails.
		bool set(const wchar_t *group, const wchar_t *name, bool value) const;

		//! Set the double value for the key defined in the given section.
		//! @param group Contain the group to which the key belongs to
		//! @param name Contain the name of the key
		//! @param value Contain the value of the key.
		//! @return false when the group or name is null, or the write to the file fails.
		bool set(const wchar_t *group, const wchar_t *name, double value) const;

		//! Set the float value for the key defined in the given section.
		//! @param group Contain the group to which the key belongs to
		//! @param name Contain the name of the key
		//! @param value Contain the value of the key.
		//! @return false when the group or name is null, or the write to the file fails.
		bool set(const wchar_t *group, const wchar_t *name, float value) const;

		//! Remove the group from the ini file.
		//! @param group Contain the group that need to be removed
		//! @return false when the group is null, or the remove fails.
		bool remove(const wchar_t *group) const;
	
	private:
		std::filesystem::path _path;
	};

}}