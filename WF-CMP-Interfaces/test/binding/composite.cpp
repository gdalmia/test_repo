// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "composite.h"

namespace Test { namespace Binding {
	
	Composite::Composite() :id("default")
	{
	}

	Composite::Composite(std::string value) : id(std::move(value))
	{
	}

	bool Composite::operator!=(const Composite& other) const
	{
		return id != other.id;
	};

	bool Composite::operator==(const Composite& other) const
	{
		return id == other.id;
	};
}}
