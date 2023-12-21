// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "datetime.h"
#include "patient.h"
#include "irepository.h"

#include <string>
#include <vector>

//TICS -INT#002: Domain model needs data member in public

namespace Philips { namespace Workflow {
	//! Represents a study in workflow
	class Study: public IRepository::Model
	{
	public:

		enum class ImportStatus
		{
			None				= 0,
			InProgress			= 1,
			Completed			= 2,
			Failed				= 3
		};

		explicit Study();									//!< Creates a study
		explicit Study(const std::string& key);				//!< Creates a study with given key

		bool operator ==(const Study& other) const;
		bool operator !=(const Study& other) const;

		Patient patient;									//!< Patient to which study belongs to

		std::string id;							            //!< Identifier for the study
		DateTime dateTime;						            //!< Date Time of the study
		std::string description;					        //!< Study description
		double weight;							            //!< Weight of the patient
		double height;							            //!< Height of the patient
		std::vector<std::string> alerts;			        //!< Alerts for the patient
		std::vector<std::string> allergies;		            //!< Allergies of the patient
		std::string accessionNumber;				        //!< A RIS generated number that identifies the study
		std::string requestingService;			            //!< Requesting service
		PersonName referringPhysician;				        //!< Requesting physician
		std::vector<std::string> comments;		            //!< Comments
		std::string instanceUid;							//!< Study Instance UID
		ImportStatus importStatus;							//!< Import status of the study
	};
}}
