// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "minitomodelconverter.h"

namespace Philips { namespace Workflow { 
	
	const std::wstring MiniToModelConverter::Peripheral = L"Peripheral";
	const std::wstring MiniToModelConverter::Coronary = L"Coronary";
	const std::wstring MiniToModelConverter::AcquisitionPhase = L"Acquisition";
	const std::wstring MiniToModelConverter::ReviewPhase = L"Review";
	const std::wstring MiniToModelConverter::LocalSource = L"Local";
	const std::wstring MiniToModelConverter::WlmSource   = L"Wlm";
	const std::wstring MiniToModelConverter::CwisSource  = L"Cwis";

	Series MiniToModelConverter::getSeries(const Database::Series& dbSeries) {
		if (!dbSeries) return Series();

		Series wfSeries(dbSeries.path().u8string());
		const auto seriesInfo = dbSeries.read();
		const auto dicomMiniInfo = dbSeries.read<Mini::Models::Dicom::Series>();

		auto details = getDetails(dbSeries);
		wfSeries.status = static_cast<Series::ArchiveStatus>(static_cast<std::uint8_t>(details.archiveStatus));
		wfSeries.description = convert(seriesInfo.description);
		wfSeries.creationTime = DateTime(seriesInfo.creationTime);
		wfSeries.acquisitionTime = DateTime(seriesInfo.acquisitionTime);
		wfSeries.number = seriesInfo.number;
		wfSeries.clinicalDataType = convert(seriesInfo.clinicaldatatype);
		wfSeries.clinicalDataTypeVariant = convert(seriesInfo.clinicaldatatypevariant);
		wfSeries.flagged = seriesInfo.flagged;
		wfSeries.modality = getModality(dicomMiniInfo.modality);
		wfSeries.instanceUid = convert(dicomMiniInfo.instanceUID);
		wfSeries.parent = getProcedure(dbSeries.procedureStep());
		wfSeries.importStatus = static_cast<Series::ImportStatus>(static_cast<std::uint8_t>(details.importStatus));
		return wfSeries;
	}



	Procedure::State MiniToModelConverter::getState(const enum Mini::Models::Database::ProcedureStep::State& dbState)
	{
		auto state = Workflow::Procedure::State::InProgress;
		switch (dbState)
		{
		case Mini::Models::Database::ProcedureStep::State::Scheduled:   state = Procedure::State::Scheduled;  break;
		case Mini::Models::Database::ProcedureStep::State::Suspended:   state = Procedure::State::Suspended;  break;
		case Mini::Models::Database::ProcedureStep::State::Completed:   state = Procedure::State::Completed;  break;
		default:    break;
		}
		return state;
	}

	Philips::Workflow::SeriesDetails MiniToModelConverter::getDetails(const Database::Series & dbSeries)
	{
		Philips::Workflow::SeriesDetails details;
		Mini::FileSource(dbSeries.path() / L"apposdh.mini") >> details;
		return details;
	}

	Philips::Workflow::StudyDetails MiniToModelConverter::getStudyDetails(const Database::Study & dbStudy)
	{
		Philips::Workflow::StudyDetails details;
		Mini::FileSource(dbStudy.path() / L"apposdh.mini") >> details;
		return details;
	}

	Procedure::Phase MiniToModelConverter::getPhase(const Philips::Workflow::ProcedureStep& procedure)
	{
		const std::wstring phase = procedure.phase;
		if (phase == AcquisitionPhase)	return Procedure::Phase::Acquisition;
		if (phase == ReviewPhase)		return Procedure::Phase::Review;

		return Procedure::Phase::Acquisition;
	}

	Procedure::ProcedureSource MiniToModelConverter::getSource(const Philips::Workflow::ProcedureStep& procedure)
	{
		const std::wstring phase = procedure.source;
		if (phase == WlmSource)			return Procedure::ProcedureSource::Wlm;
		if (phase == CwisSource)		return Procedure::ProcedureSource::Cwis;

		return Procedure::ProcedureSource::Local;
	}

	Procedure::Type MiniToModelConverter::getType(std::wstring databaseProcedureType)
	{
		if (databaseProcedureType.empty()) return Procedure::Type::Unknown;
		if (databaseProcedureType == L"Coronary") return Procedure::Type::Coronary;
		if (databaseProcedureType == L"Peripheral") return Procedure::Type::Peripheral;
		
		return Procedure::Type::Unknown;
	}

	Study MiniToModelConverter::getStudy(const Database::Study& study)
	{
		if (!study) return Study();

		Study studyDetails(study.path().u8string());
		const auto studyInfo = study.read();
		studyDetails.id = convert(studyInfo.studyId);
		studyDetails.dateTime = DateTime(studyInfo.studyTime);
		studyDetails.description = convert(studyInfo.description);
		studyDetails.weight = studyInfo.weight;
		studyDetails.height = studyInfo.height;
		const auto studyDicom = study.read<Mini::Models::Dicom::Study>();
		studyDetails.accessionNumber = convert(studyDicom.accessionNumber);
		studyDetails.requestingService = convert(studyDicom.requestingService);
		studyDetails.instanceUid = convert(studyDicom.instanceUID);

		const auto& details = getStudyDetails(study);
		studyDetails.importStatus = static_cast<Study::ImportStatus>(static_cast<std::uint8_t>(details.importStatus));
		studyDetails.referringPhysician.components = getName(studyDicom.referringPhysician);
		
		for (const auto& alert : studyInfo.alerts)
			studyDetails.alerts.push_back(convert(alert));

		for (const auto& allergy : studyInfo.allergies)
			studyDetails.allergies.push_back(convert(allergy));

			studyDetails.comments.push_back(convert(studyInfo.patientComments));

		studyDetails.patient = getPatient(study.patient());
		return studyDetails;
	}

	Procedure MiniToModelConverter::getProcedure(const Database::ProcedureStep& dbProcedure)
	{
		if (!dbProcedure) return Procedure();

		Procedure wfProcedure(dbProcedure.path().u8string());
		const auto procedureInfo = dbProcedure.read();
		const auto dicomMiniInfo = dbProcedure.read<Mini::Models::Dicom::ProcedureStep>();
		Philips::Workflow::ProcedureStep apposDhInfo;
		Mini::FileSource(dbProcedure.path() / L"apposdh.mini") >> apposDhInfo;

		wfProcedure.id = convert(dicomMiniInfo.procedureStepId);
		wfProcedure.description = convert(procedureInfo.description);
		wfProcedure.startTime = DateTime(procedureInfo.startTime);
		wfProcedure.endTime = DateTime(procedureInfo.endTime);
		wfProcedure.state = getState(procedureInfo.state);
		wfProcedure.phase = getPhase(apposDhInfo);
		wfProcedure.procedureSource = getSource(apposDhInfo);
		wfProcedure.type = getType(procedureInfo.procedureType);
		wfProcedure.performingPhysician.components = getName(dicomMiniInfo.performingPhysician);
		wfProcedure.performingOperator.components = getName(dicomMiniInfo.performingOperatorName);
		
		
		wfProcedure.parent = getStudy(dbProcedure.study());
		return wfProcedure;
	}

	Patient::Type MiniToModelConverter::getPatientType(const enum Database::Patient::Info::Type& patientType)
	{
		Patient::Type patType = Patient::Type::Unknown;
		switch (patientType)
		{
		case Mini::Models::Database::Patient::Type::Neonate: patType = Patient::Type::Neonate; break;
		case Mini::Models::Database::Patient::Type::Infant: patType = Patient::Type::Infant; break;
		case Mini::Models::Database::Patient::Type::Child: patType = Patient::Type::Child; break;
		case Mini::Models::Database::Patient::Type::SmallAdult: patType = Patient::Type::SmallAdult; break;
		case Mini::Models::Database::Patient::Type::NormalAdult: patType = Patient::Type::NormalAdult; break;
		case Mini::Models::Database::Patient::Type::LargeAdult: patType = Patient::Type::LargeAdult; break;
		case Mini::Models::Database::Patient::Type::VerylargeAdult: patType = Patient::Type::VeryLargeAdult; break;
		case Mini::Models::Database::Patient::Type::Unknown: patType = Patient::Type::Unknown; break;
		}
		return patType;
	}

	template <typename personName>
	bool MiniToModelConverter::isComponentPresent(const personName& name)
	{
		return !(name.familyName.empty() && name.givenName.empty() && name.middleName.empty() &&
			name.namePrefix.empty() && name.nameSuffix.empty());
	}
	
	template <typename patientInfo>
	std::map<PersonName::Group, PersonName::Component> MiniToModelConverter::getName( patientInfo info)
	{
		std::map<PersonName::Group, PersonName::Component> name;
		if(isComponentPresent(info.personNameAlfabetic))
		{
			name.insert({ PersonName::Group::Alphabetic,
			PersonName::Component{ convert(info.personNameAlfabetic.familyName),
				convert(info.personNameAlfabetic.givenName),
				convert(info.personNameAlfabetic.middleName),
				convert(info.personNameAlfabetic.namePrefix),
				convert(info.personNameAlfabetic.nameSuffix)
			} });
		}

		if (isComponentPresent(info.personNameIdeografic))
		{
			name.insert({ PersonName::Group::Ideographic,
				PersonName::Component{ convert(info.personNameIdeografic.familyName),
					convert(info.personNameIdeografic.givenName),
					convert(info.personNameIdeografic.middleName),
					convert(info.personNameIdeografic.namePrefix),
					convert(info.personNameIdeografic.nameSuffix)
				} });
		}
		if (isComponentPresent(info.personNamePhonetic))
		{
			name.insert({ PersonName::Group::Phonetic,
				PersonName::Component{ convert(info.personNamePhonetic.familyName),
					convert(info.personNamePhonetic.givenName),
					convert(info.personNamePhonetic.middleName),
					convert(info.personNamePhonetic.namePrefix),
					convert(info.personNamePhonetic.nameSuffix)
				} });
		}
		return name;
	}
	
	Patient MiniToModelConverter::getPatient(const Database::Patient& patient)
	{
		if (!patient) return Patient();

		Patient patientDetails;
		const auto patientInfo = patient.read();
		patientDetails.id = convert(patientInfo.patientId);
		if(!patientInfo.birthDate.empty())
			patientDetails.birthDate = DateTime(patientInfo.birthDate);
		patientDetails.gender = getGender(patientInfo.gender);
		patientDetails.type = getPatientType(patientInfo.type);
		const auto patientDicom = patient.read<Mini::Models::Dicom::Patient>();

		patientDetails.name.components = getName(patientDicom.patientInfo);
		return patientDetails;
	}

	Patient::Gender MiniToModelConverter::getGender(const enum Database::Patient::Info::Gender& genderInfo)
	{
		Patient::Gender gender = Patient::Gender::Unknown;
		switch (genderInfo)
		{
		case Mini::Models::Database::Patient::Gender::Unknown:  gender = Patient::Gender::Unknown;  break;
		case Mini::Models::Database::Patient::Gender::Male:     gender = Patient::Gender::Male;     break;
		case Mini::Models::Database::Patient::Gender::Female:   gender = Patient::Gender::Female;   break;
		case Mini::Models::Database::Patient::Gender::Other:    gender = Patient::Gender::Other;    break;
		default:    break;
		}
		return gender;
	}

	Workflow::Series::Modality MiniToModelConverter::getModality(const enum Mini::Models::Dicom::Series::Modality& dbModality)
	{
		auto modality = Workflow::Series::Modality::XA;
		switch (dbModality)
		{
		case Mini::Models::Dicom::Series::Modality::IVUS:   modality = Workflow::Series::Modality::IVUS;    break;
		case Mini::Models::Dicom::Series::Modality::US:     modality = Workflow::Series::Modality::US;      break;
		case Mini::Models::Dicom::Series::Modality::MR:     modality = Workflow::Series::Modality::MR;      break;
		case Mini::Models::Dicom::Series::Modality::CT:     modality = Workflow::Series::Modality::CT;      break;
		default:    break;
		}
		return modality;
	}
}}