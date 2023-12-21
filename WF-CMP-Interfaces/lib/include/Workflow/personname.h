// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include <string>
#include <map>

namespace Philips { namespace Workflow {

	//! Represents a persons name
	//! Names are encoded using using value representations based on component groups using 5 components.
	class PersonName
	{
	public:
		//! Represents the components groups
		enum class Group
		{
				Alphabetic	= 0
			,	Ideographic = 1
			,	Phonetic	= 2
		};

		//! Represent a peron's name
		struct Component
		{
			std::string familyName;			    //!< Family name of the person; 
			std::string givenName;				//!< Given name of the person
			std::string middleName;			    //!< Middle name of the person
			std::string namePrefix;				//!< Prefix for name
			std::string nameSuffix;				//!< Suffix for name

			bool operator ==(const Component& other) const;
			bool operator !=(const Component& other) const;
		};

		//! Return default formatted name for Alphabetic group.
		//! [Prefix] [Given name] [Middle Name] [Family Name] [Suffix]
		std::string toString() const;
		
		bool operator ==(const PersonName& other) const;
		bool operator !=(const PersonName& other) const;

		std::map<Group, Component> components;
	};
}}
