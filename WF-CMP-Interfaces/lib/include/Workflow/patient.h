// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "datetime.h"
#include "personname.h"
#include "optional.h"

//TICS -INT#002: Domain model needs data member in public

namespace Philips { namespace Workflow {
	//! Represents a patient in workflow
	class Patient
	{
	public:
		//! Gender of a patient
		enum class Gender
		{
			Unknown = 0,
			Male = 1,
			Female = 2,
			Other = 3
		};

		//! Patient Type
		enum class Type
		{
			Unknown			= 0,
			Neonate			= 1,
			Infant			= 2,
			Child			= 3,
			SmallAdult		= 4,
			NormalAdult		= 5,
			LargeAdult		= 6,
			VeryLargeAdult	= 7
		};

		Patient();											//!< Creates a patient

		bool operator ==(const Patient& other) const;		
		bool operator !=(const Patient& other) const;

		std::string id;										//!< Identifier for the patient
		PersonName name;									//!< Name of the person. List to support Chinese names
		Optional<DateTime> birthDate;						//!< Date of birth of the patient
		Gender gender;										//!< Gender of the patient
		Type type;											//!< Type of Patient
	};
}}
