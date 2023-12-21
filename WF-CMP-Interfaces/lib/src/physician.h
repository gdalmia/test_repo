// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include<workflow.h>

namespace Philips { namespace Workflow {
	struct Physician :Net::Object::Serializable
	{
		struct PersonName : Net::Object::Serializable
		{
			struct Component : Net::Object::Serializable
			{
				Attribute<std::string>  familyName	= { this,"familyName" };
				Attribute<std::string>  givenName	= { this, "givenName" };
				Attribute<std::string>  middleName	= { this, "middleName" };
				Attribute<std::string>  namePrefix	= { this, "namePrefix" };
				Attribute<std::string>  nameSuffix	= { this, "nameSuffix" };
				Component();
			};
			Attribute<Component> alphabetic		= { this, "alphabetic" };
			Attribute<Component> ideographic	= { this, "ideographic" };
			Attribute<Component> phonetic		= { this, "phonetic" };
			Attribute<std::string> id			= { this , "id" };
			PersonName();
		};

		Physician();
		Attribute<std::vector<PersonName>> name = { this, "name" , "name1" };
	};
}}