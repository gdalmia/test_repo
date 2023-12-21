// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include <string>

namespace Test { namespace Binding {

	struct Composite
	{
		Composite();
		explicit Composite(std::string value);

		~Composite() = default;
		Composite(const Composite&) = default;
		Composite(Composite&&) = default;
		Composite& operator=(const Composite&) = default;
		Composite& operator=(Composite&&) = default;

		bool operator!=(const Composite& other) const;
		bool operator==(const Composite& other) const;

		std::string id;
	};
	
}}
