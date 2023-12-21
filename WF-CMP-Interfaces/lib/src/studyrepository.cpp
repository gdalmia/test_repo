// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "workflow.h"
#include "studydetails.mini.h"
#include "procedurestep.mini.h"
#include "seriesdetails.mini.h"
#include "minitomodelconverter.h"
#include "procedurenotification.h"
#include "studynotification.h"
#include "seriesnotification.h"
#include <database/database.h>
#include <models/database.mini.h>
#include <models/dicom.mini.h>
#include <mini/filesource.h>
#include <mini/filesink.h>
#include <Trace.h>

namespace Philips {namespace Workflow {

    struct StudyRepository::Private
    {
        StudyRepository::Private():
            _settings(),
            database(_settings.test()),
            _studyList(database.openStudyList())
        {
        }
        
        Database::StudyList&    getStudyList() { return _studyList; }

		void					serialize(const Workflow::Patient&, Mini::Models::Database::Patient&) const;
		void					serialize(const Workflow::Patient& workflowPatient, Mini::Models::Dicom::Patient& dicomPatient) const;
		void					serialize(const Workflow::Study&,   Mini::Models::Database::Study&) const;
		void					serialize(const Workflow::Study&,   Mini::Models::Dicom::Study&) const;
		void					serialize(const Workflow::Procedure&, Mini::Models::Database::ProcedureStep&) const;
		void					serialize(const Workflow::Procedure&, Mini::Models::Dicom::ProcedureStep&) const;
		void					serialize(const Workflow::Series&, Mini::Models::Database::Series&) const;
		void					serialize(const Workflow::Series&, Mini::Models::Dicom::Series&) const;
		void					writeAppOsProcedure(const Database::ProcedureStep& dbProcedure, const Procedure& procedure) const;
		void					writeAppOsSeries(const Database::Series& dbSeries, const Workflow::Series& series) const;
		void					writeAppOsStudy(const Database::Study& dbStudy, const Workflow::Study & study) const;

	private: 
		Settings							_settings; 
	public:
		Database::Database					database;
	private:
		Mini::Models::Dicom::Series::Modality getModalityType(Series::Modality) const;
		Mini::Models::Database::Patient::Type getPatientType(const Workflow::Patient::Type& patientType) const;
		Database::StudyList                 _studyList;

		template<typename NameGroup, typename Person>
		void getPatientInfo(NameGroup& component ,Person& dicomPerson) const;
		template<typename Person>
		void toDicomPersonInfo(std::map<PersonName::Group, PersonName::Component>::const_iterator start, Person& dicomPatient) const;
		
    };

    StudyRepository::StudyRepository() :
        _private(std::make_unique<Private>())
    {
        const Workflow::Settings settings;
		_private->getStudyList().eventStudyCreated = [this](const Database::Study &study) { studyAdded(MiniToModelConverter::getStudy(study)); IStudyRepositoryNotification::studyAdded(MiniToModelConverter::getStudy(study)); };
		_private->getStudyList().eventStudyRemoved = [this](const std::filesystem::path &path) { studyRemoved(Model(path.u8string())); IStudyRepositoryNotification::studyRemoved(Model(path.u8string())); };
    }

    StudyRepository::~StudyRepository()
    {
        _private.reset(nullptr);
    }

	void StudyRepository::Private::serialize(const Workflow::Patient& workflowPatient, Mini::Models::Database::Patient& databasePatient) const
	{
		databasePatient.patientId = convert(workflowPatient.id);
		if (workflowPatient.birthDate)
			databasePatient.birthDate = workflowPatient.birthDate.value();
		else
			databasePatient.birthDate.clear();
		const auto& component = workflowPatient.name.components.find(PersonName::Group::Alphabetic);
		if (component != workflowPatient.name.components.end())
		{
			databasePatient.familyName = convert(component->second.familyName);
			databasePatient.givenName = convert(component->second.givenName);
			databasePatient.middleName = convert(component->second.middleName);
			databasePatient.namePrefix = convert(component->second.namePrefix);
			databasePatient.nameSuffix = convert(component->second.nameSuffix);
		}

		databasePatient.type = getPatientType(workflowPatient.type);

		switch (workflowPatient.gender)
		{
		case Patient::Gender::Unknown:  databasePatient.gender = Mini::Models::Database::Patient::Gender::Unknown;  break;
		case Patient::Gender::Male:     databasePatient.gender = Mini::Models::Database::Patient::Gender::Male;     break;
		case Patient::Gender::Female:   databasePatient.gender = Mini::Models::Database::Patient::Gender::Female;   break;
		case Patient::Gender::Other:    databasePatient.gender = Mini::Models::Database::Patient::Gender::Other;    break;
		default:						break;
		}
	}

	template<typename NameGroup, typename Person>
	void StudyRepository::Private::getPatientInfo(NameGroup& component, Person& dicomPerson) const
    {
		const auto& alphabeticComponent = component.find(PersonName::Group::Alphabetic);
		if (alphabeticComponent != component.end())
			toDicomPersonInfo(alphabeticComponent, dicomPerson.personNameAlfabetic);

		const auto& ideographicComponent = component.find(PersonName::Group::Ideographic);
		if (ideographicComponent != component.end())
			toDicomPersonInfo(ideographicComponent, dicomPerson.personNameIdeografic);

		const auto& PhoneticComponent = component.find(PersonName::Group::Phonetic);
		if (PhoneticComponent != component.end())
			toDicomPersonInfo(PhoneticComponent, dicomPerson.personNamePhonetic);
    }
	
	void StudyRepository::Private::serialize(const Workflow::Patient& workflowPatient, Mini::Models::Dicom::Patient& dicomPatient) const
	{

		getPatientInfo(workflowPatient.name.components,dicomPatient.patientInfo);
	}
	
	template<typename Person>
	void StudyRepository::Private::toDicomPersonInfo(std::map<PersonName::Group, PersonName::Component>::const_iterator component, Person& dicomPatient) const
    {
			dicomPatient.familyName = convert(component->second.familyName);
			dicomPatient.givenName = convert(component->second.givenName);
			dicomPatient.middleName = convert(component->second.middleName);
			dicomPatient.namePrefix = convert(component->second.namePrefix);
			dicomPatient.nameSuffix = convert(component->second.nameSuffix);
    }
	
	void   StudyRepository::Private::serialize(const Workflow::Study& workflowStudy, Mini::Models::Database::Study& databaseStudy) const
	{
		databaseStudy.studyId = convert(workflowStudy.id); 
		databaseStudy.studyTime = workflowStudy.dateTime;
		databaseStudy.weight = workflowStudy.weight;
		databaseStudy.height = workflowStudy.height;
		databaseStudy.description = convert(workflowStudy.description);
		databaseStudy.alerts.clear();
		for (const auto& comment : workflowStudy.comments)
		{
			databaseStudy.patientComments =  (convert(comment));
		}
		for (const auto& alert : workflowStudy.alerts)
		{
			databaseStudy.alerts.emplace_back(convert(alert));
		}
		databaseStudy.allergies.clear();
		for (const auto& allergy : workflowStudy.allergies)
		{
			databaseStudy.allergies.emplace_back(convert(allergy));
		}
	}

	void StudyRepository::Private::serialize(const Workflow::Study& workflowStudy, Mini::Models::Dicom::Study& dicomStudy) const
	{
		dicomStudy.accessionNumber = convert(workflowStudy.accessionNumber);
		dicomStudy.requestingService = convert(workflowStudy.requestingService);
		dicomStudy.instanceUID = convert(workflowStudy.instanceUid);
    	
		getPatientInfo(workflowStudy.referringPhysician.components, dicomStudy.referringPhysician);
	}

	void StudyRepository::Private::serialize(const Workflow::Procedure& workflowProcedure, Mini::Models::Database::ProcedureStep& databaseProcedure) const
	{
		switch (workflowProcedure.state)
		{
		case Procedure::State::Completed:	databaseProcedure.state = Mini::Models::Database::ProcedureStep::State::Completed; break;
		case Procedure::State::InProgress:	databaseProcedure.state = Mini::Models::Database::ProcedureStep::State::InProgress; break;
		case Procedure::State::Scheduled:	databaseProcedure.state = Mini::Models::Database::ProcedureStep::State::Scheduled; break;
		case Procedure::State::Suspended:	databaseProcedure.state = Mini::Models::Database::ProcedureStep::State::Suspended; break;
		case Procedure::State::Selected:	break;
		default:							break;
		}

		switch (workflowProcedure.type)
		{
		case Procedure::Type::Coronary:		databaseProcedure.procedureType = L"Coronary"; break;
		case Procedure::Type::Peripheral:	databaseProcedure.procedureType = L"Peripheral"; break;
		case Procedure::Type::Unknown:		databaseProcedure.procedureType = L""; break;
		default:							break;
		}

		databaseProcedure.startTime = workflowProcedure.startTime;
		databaseProcedure.endTime = workflowProcedure.endTime;
		databaseProcedure.description = convert(workflowProcedure.description);
	}

	void StudyRepository::Private::serialize(const Workflow::Procedure& workflowProcedure, Mini::Models::Dicom::ProcedureStep& dicomProcedure) const
	{
		dicomProcedure.procedureStepId = convert(workflowProcedure.id);
		getPatientInfo(workflowProcedure.performingPhysician.components, dicomProcedure.performingPhysician);
		getPatientInfo(workflowProcedure.performingOperator.components, dicomProcedure.performingOperatorName);
	}

    void StudyRepository::Private::serialize(const Workflow::Series& wfSeries, Mini::Models::Database::Series& databseSeries) const
    {
		databseSeries.acquisitionTime = wfSeries.acquisitionTime;
		databseSeries.creationTime = wfSeries.creationTime;
		databseSeries.description = convert(wfSeries.description);
		databseSeries.clinicaldatatype = convert(wfSeries.clinicalDataType);
		databseSeries.clinicaldatatypevariant = convert(wfSeries.clinicalDataTypeVariant);
		databseSeries.flagged = wfSeries.flagged;
		databseSeries.number = wfSeries.number;
    }

    void StudyRepository::Private::serialize(const Workflow::Series& wfSeries, Mini::Models::Dicom::Series& dicomSeries) const
    {
		dicomSeries.instanceUID = convert(wfSeries.instanceUid);
		dicomSeries.modality = getModalityType(wfSeries.modality);
    }

    void StudyRepository::Private::writeAppOsProcedure(const Database::ProcedureStep& dbProcedure, const Procedure& procedure) const
	{
		Philips::Workflow::ProcedureStep info;
		info.phase = (procedure.phase == Procedure::Phase::Review) ? MiniToModelConverter::ReviewPhase : MiniToModelConverter::AcquisitionPhase;
		info.source = MiniToModelConverter::LocalSource;
    	if(procedure.procedureSource != Procedure::ProcedureSource::Local)
		info.source = (procedure.procedureSource == Procedure::ProcedureSource::Wlm) ? MiniToModelConverter::WlmSource : MiniToModelConverter::CwisSource;
    	
		Mini::FileSink fileSink(dbProcedure.path() / L"apposdh.mini");
		fileSink << info;
		fileSink.flush();
		fileSink.close();
    }

	void StudyRepository::Private::writeAppOsSeries(const Database::Series& dbSeries, const Workflow::Series & series) const
	{
		Philips::Workflow::SeriesDetails details;
		details.archiveStatus = static_cast<uint8_t>(series.status);
		details.importStatus = static_cast<uint8_t>(series.importStatus);
		Mini::FileSink fileSink(dbSeries.path() / L"apposdh.mini");
		fileSink << details;
		fileSink.flush();
		fileSink.close();
	}

	void StudyRepository::Private::writeAppOsStudy(const Database::Study& dbStudy, const Workflow::Study & study) const
	{
		Philips::Workflow::StudyDetails details;
		details.importStatus = static_cast<uint8_t>(study.importStatus);
		Mini::FileSink fileSink(dbStudy.path() / L"apposdh.mini");
		fileSink << details;
		fileSink.flush();
		fileSink.close();
	}

	Mini::Models::Dicom::Series::Modality StudyRepository::Private::getModalityType(Series::Modality wfModality) const
	{
		Mini::Models::Dicom::Series::Modality dicomModality = {};
		switch (wfModality)
		{
		case Series::Modality::CT: dicomModality = Mini::Models::Dicom::Series::Modality::CT; break;
		case Series::Modality::IVUS: dicomModality = Mini::Models::Dicom::Series::Modality::IVUS; break;
		case Series::Modality::MR: dicomModality = Mini::Models::Dicom::Series::Modality::MR; break;
		case Series::Modality::US: dicomModality = Mini::Models::Dicom::Series::Modality::US; break;
		case Series::Modality::XA: dicomModality = Mini::Models::Dicom::Series::Modality::XA; break;
		default:break;
		}
		return dicomModality;
	}

	Mini::Models::Database::Patient::Type StudyRepository::Private::getPatientType(const Workflow::Patient::Type& patientType) const
	{
		Mini::Models::Database::Patient::Type patType = Mini::Models::Database::Patient::Type::Unknown;
		switch (patientType)
		{
		case Patient::Type::Neonate:			patType = Mini::Models::Database::Patient::Type::Neonate; break;
		case Patient::Type::Infant:				patType = Mini::Models::Database::Patient::Type::Infant; break;
		case Patient::Type::Child:				patType = Mini::Models::Database::Patient::Type::Child; break;
		case Patient::Type::SmallAdult:			patType = Mini::Models::Database::Patient::Type::SmallAdult; break;
		case Patient::Type::NormalAdult:		patType = Mini::Models::Database::Patient::Type::NormalAdult; break;
		case Patient::Type::LargeAdult:			patType = Mini::Models::Database::Patient::Type::LargeAdult; break;
		case Patient::Type::VeryLargeAdult:		patType = Mini::Models::Database::Patient::Type::VerylargeAdult; break;
		case Patient::Type::Unknown:			patType = Mini::Models::Database::Patient::Type::Unknown; break;
		}
		return patType;
	}

    std::vector<Study> StudyRepository::studies()
    {
        const Database::StudyList studyList = _private->database.openStudyList();
        std::vector<Study>       studies;
        for (const auto &study : studyList.studies())
            studies.push_back(MiniToModelConverter::getStudy(study));

        return studies;
    }

    std::vector<Procedure> StudyRepository::procedures(const Study& study)
    {
        std::vector<Procedure> procedures;
        const auto dbStudy = _private->database.openStudy(std::filesystem::path(static_cast<std::string>(study)));
        for (const auto& procedure : dbStudy.procedureSteps())
            procedures.emplace_back(MiniToModelConverter::getProcedure(procedure));

        return procedures;
    }

    std::vector<Series> StudyRepository::series(const Procedure& procedure)
    {
        std::vector<Series> series;
        const auto dbProcedure = _private->database.openProcedureStep(std::filesystem::path(static_cast<std::string>(procedure)));
        for (const auto& dbSeries : dbProcedure.series())
            series.emplace_back(MiniToModelConverter::getSeries(dbSeries));
        
        return series;
    }

    std::vector<Series> StudyRepository::series(const Study& study)
    {
		std::vector<Series> series;
		for (auto& procedure : this->procedures(study))
		{
			auto seriesList = this->series(procedure);
			series.insert(series.end(), seriesList.begin(), seriesList.end());
		}
		return series;
    }

    bool StudyRepository::isProtected(const Study& study)
    {
        const auto dbStudy = _private->database.openStudy(std::filesystem::path(static_cast<std::string>(study)));
        return std::filesystem::exists(dbStudy.path() / "protected");
    }

	bool StudyRepository::add(Study& study)
	{	
		//Patient Creation
		auto patientList = _private->database.openPatientList();
		auto patient = patientList.createPatient();	
		if (!patient)	return false;
		auto patientInfo = patient.read();
		_private->serialize(study.patient, patientInfo);

		auto patientDicom = patient.read<Mini::Models::Dicom::Patient>();
		_private->serialize(study.patient, patientDicom);
		patient.write(patientInfo);
		patient.write(patientDicom);
		
		//Database model study creation
		auto newStudy = patient.createStudy();
		if (!newStudy)	return false;
		auto studyInfo = newStudy.read(); 
		_private->serialize(study, studyInfo);
		_private->writeAppOsStudy(newStudy, study);

		//Dicom model study creation
		auto studyDicom = newStudy.read<Mini::Models::Dicom::Study>();
		_private->serialize(study, studyDicom);

		newStudy.write(studyInfo);
		newStudy.write(studyDicom);
		newStudy.unhide();

		setKey(study, newStudy.path().u8string());
		return true;
	}

	bool StudyRepository::update(const Study& study)
	{
		const auto studyKey = std::filesystem::path(static_cast<std::string>(study));
		auto databaseStudy = _private->database.openStudy(studyKey);
		if (!databaseStudy)	return false;

		auto databasePatient = databaseStudy.patient();
		auto patientInfo = databasePatient.read();
		_private->serialize(study.patient, patientInfo);
		auto dicomPatientInfo = databasePatient.read<Mini::Models::Dicom::Patient>();
		_private->serialize(study.patient, dicomPatientInfo);
    	
		auto studyInfo = databaseStudy.read();
		_private->serialize(study, studyInfo);
		auto dicomStudyInfo = databaseStudy.read<Mini::Models::Dicom::Study>();
		_private->serialize(study, dicomStudyInfo);	
		_private->writeAppOsStudy(databaseStudy, study);
		databasePatient.write(patientInfo);
		databasePatient.write(dicomPatientInfo);
		databaseStudy.write(studyInfo);
		databaseStudy.write(dicomStudyInfo);
		return true;
	}

	bool StudyRepository::remove(const Study& study)
	{
		const auto dbStudy = _private->database.openStudy(static_cast<std::string>(study));
		const auto patient = dbStudy.patient();
		const auto dbStudies = patient.studies();
		if (dbStudies.size() <= 1)
		{
			auto retValue = _private->database.removePatient(patient.path());
			if (retValue) TraceMessage() << "Patient is deleted";
			else TraceWarning() << "Patient deletion failed";
			return retValue;
		}
		else
		{
			auto retValue = _private->database.removeStudy(dbStudy.path());
			if (retValue) TraceMessage() << "Study is deleted";
			else TraceWarning() << "Study deletion failed";
			return retValue;
		}
	}

	bool StudyRepository::add(Procedure& procedure)
	{	
		const auto studyKey = std::filesystem::path(static_cast<std::string>(procedure.parent));
		auto study = _private->database.openStudy(studyKey);
		if (!study)	return false;
		auto procedureStep = study.createProcedureStep();
		if (!procedureStep)	return false;
		auto procedureInfo = procedureStep.read();
		_private->serialize(procedure, procedureInfo);
		
		auto dicomMiniInfo = procedureStep.read<Mini::Models::Dicom::ProcedureStep>();
		_private->serialize(procedure, dicomMiniInfo);

		_private->writeAppOsProcedure(procedureStep, procedure);
			
		procedureStep.write(procedureInfo);
		procedureStep.write(dicomMiniInfo);
		procedureStep.unhide();

		setKey(procedure, procedureStep.path().u8string());

		return true;
	}

	bool StudyRepository::update(const Procedure& procedure)
	{
		const auto procedureKey = std::filesystem::path(static_cast<std::string>(procedure));
		auto databaseProcedure = _private->database.openProcedureStep(procedureKey);
		if (!databaseProcedure)	return false;
		auto procedureInfo = databaseProcedure.read();
		_private->serialize(procedure, procedureInfo);

		auto dicomInfo = databaseProcedure.read<Mini::Models::Dicom::ProcedureStep>();
		_private->serialize(procedure, dicomInfo);

		_private->writeAppOsProcedure(databaseProcedure, procedure);

		databaseProcedure.write(procedureInfo);
		databaseProcedure.write(dicomInfo);

		return true;
	}

	bool StudyRepository::remove(const Procedure& procedure)
	{
		return _private->database.removeProcedureStep(static_cast<std::string>(procedure));
	}

	bool StudyRepository::add(Series& series)
	{
		const auto procedureKey = std::filesystem::path(static_cast<std::string>(series.parent));
		auto procedure = _private->database.openProcedureStep(procedureKey);
		if (!procedure)	return false;
		auto dbSeries = procedure.createSeries();
		if (!dbSeries)	return false;
		auto seriesInfo = dbSeries.read();
		_private->serialize(series, seriesInfo);

		auto dicomMiniInfo = dbSeries.read<Mini::Models::Dicom::Series>();
		_private->serialize(series, dicomMiniInfo);

		_private->writeAppOsSeries(dbSeries, series);
		dbSeries.write(seriesInfo);
		dbSeries.write(dicomMiniInfo);
		dbSeries.unhide();

		setKey(series, dbSeries.path().u8string());

		return true;
	}

	bool StudyRepository::update(const Series& series)
	{
		const auto seriesKey = std::filesystem::path(static_cast<std::string>(series));
		auto databaseSeries = _private->database.openSeries(seriesKey);
		if (!databaseSeries)	return false;
		auto seriesInfo = databaseSeries.read();
		_private->serialize(series, seriesInfo);

		auto dicomInfo = databaseSeries.read<Mini::Models::Dicom::Series>();
		_private->serialize(series, dicomInfo);

		_private->writeAppOsSeries(databaseSeries, series);
		databaseSeries.write(seriesInfo);
		databaseSeries.write(dicomInfo);

		return true;
	}

	bool StudyRepository::remove(const Series& series)
	{
		return _private->database.removeSeries(static_cast<std::string>(series));
	}

	std::unique_ptr<IProcedureNotification> StudyRepository::subscribe(const Procedure& procedure) 
	{
		auto databaseProcedure = _private->database.openProcedureStep(std::filesystem::path(static_cast<std::string>(procedure)));
		if (!databaseProcedure)
		{
			TraceMessage() << L"Procedure [key: " << static_cast<std::string>(procedure) << L"] is invalid";
			return nullptr;
		}
		return std::make_unique<ProcedureNotification>(databaseProcedure);
	}

    std::unique_ptr<IStudyNotification> StudyRepository::subscribe(const Study& study) 
    {
		auto databaseStudy = _private->database.openStudy(std::filesystem::path(static_cast<std::string>(study)));
		if (!databaseStudy)
		{
			TraceMessage() << L"Study [key: " << static_cast<std::string>(study) << L"] is invalid";
			return nullptr;
		}
		return std::make_unique<StudyNotification>(databaseStudy);
    }

    std::unique_ptr<ISeriesNotification> StudyRepository::subscribe(const Series& series)
    {
		auto databaseSeries = _private->database.openSeries(std::filesystem::path(static_cast<std::string>(series)));
		if (!databaseSeries)
		{
			TraceMessage() << L"Series [key: " << static_cast<std::string>(series) << L"] is invalid";
			return nullptr;
		}
		return std::make_unique<SeriesNotification>(databaseSeries);
    }
}}
