// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include "workflow.h"
#include "studydetails.mini.h"
#include "procedurestep.mini.h"
#include "seriesdetails.mini.h"
#include <database/database.h>
#include <models/database.mini.h>
#include <models/dicom.mini.h>
#include <mini/filesource.h>

namespace Philips { namespace Workflow { 
	
	class MiniToModelConverter
	{
	public:
		static Series getSeries(const Database::Series& dbSeries);
		static Procedure getProcedure(const Database::ProcedureStep& dbProcedure);
		static Study getStudy(const Database::Study& study);
		static Patient getPatient(const Database::Patient& patient);
		static const std::wstring			Peripheral;
		static const std::wstring			Coronary;
		static const std::wstring			AcquisitionPhase;
		static const std::wstring			ReviewPhase;
		static const std::wstring			LocalSource;
		static const std::wstring			WlmSource;
		static const std::wstring			CwisSource;

	private:
		static Philips::Workflow::SeriesDetails getDetails(const Database::Series& dbSeries);
		static Philips::Workflow::StudyDetails getStudyDetails(const Database::Study & dbStudy);
		static Series::Modality getModality(const Mini::Models::Dicom::Series::Modality& dbModality);
		static Procedure::State getState(const Mini::Models::Database::ProcedureStep::State& dbState);
		static Procedure::Phase getPhase(const Philips::Workflow::ProcedureStep& procedure);
		static Procedure::Type getType(std::wstring databaseProcedureType);
		static Patient::Gender getGender(const Database::Patient::Info::Gender& genderInfo);
		static Procedure::ProcedureSource getSource(const Philips::Workflow::ProcedureStep& procedure);
		static Patient::Type getPatientType(const enum Database::Patient::Info::Type& patientType);
		template <typename patientInfo> static std::map<PersonName::Group, PersonName::Component> getName(patientInfo info);
		template <typename personName> static bool isComponentPresent(const personName& name);
	};
}}
