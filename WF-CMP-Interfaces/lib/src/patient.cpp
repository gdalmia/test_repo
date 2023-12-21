// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/patient.h"

namespace Philips { namespace Workflow {
	
	Patient::Patient()
		: id("")
		, birthDate()
		, gender(Gender::Unknown)
		, type(Type::Unknown)
	{
	}

	bool Patient::operator==(const Patient& other) const
	{
		return !(*this != other);
	}

	bool Patient::operator!=(const Patient& other) const
	{
		return id				 != other.id
			|| birthDate		 != other.birthDate
			|| gender			 != other.gender
			|| name				 != other.name;
	}

}}