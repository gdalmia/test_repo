// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "Workflow/personname.h"

namespace Philips { namespace Workflow {
	
	bool PersonName::Component::operator==(const Component& other) const
	{
		return !(*this != other);
	}

	bool PersonName::Component::operator!=(const Component& other) const
	{
		return  familyName	!= other.familyName
			||	givenName	!= other.givenName
			||	middleName	!= other.middleName
			||	namePrefix	!= other.namePrefix
			||	nameSuffix	!= other.nameSuffix;
	}

	std::string PersonName::toString() const
	{
		if (components.empty()) return std::string();

		const auto& component = components.find(Group::Alphabetic);
		if (component == components.end()) return std::string();

		auto name = component->second;
		auto result = name.namePrefix;

		if (!result.empty() && !name.givenName.empty())		result += " ";	
		result += name.givenName; 
		if (!result.empty() && !name.middleName.empty())	result += " ";	
		result += name.middleName; 
		if (!result.empty() && !name.familyName.empty())	result += " ";	
		result += name.familyName; 
		if (!result.empty() && !name.nameSuffix.empty())	result += " ";	
		result += name.nameSuffix; 

		return result;
	}

	bool PersonName::operator==(const PersonName& other) const
	{
		return !(*this != other);
	}

	bool PersonName::operator!=(const PersonName& other) const
	{
		return components != other.components;
	}
}}
