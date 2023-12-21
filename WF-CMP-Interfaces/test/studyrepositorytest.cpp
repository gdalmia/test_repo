// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "workflow.h"
#include "src\studydetails.mini.h"
#include "src\procedurestep.mini.h"
#include "testmodelcreator.h"
#include <gtest\gtest.h>
#include <database\database.h>
#include <database\datetime.h>
#include <models\database.mini.h>
#include <models\dicom.mini.h>
#include <mini\filesource.h>
#include <mini\filesink.h>
#include <fstream>


using namespace Philips::Workflow;
using namespace Philips::Workflow::Test;

namespace Test
{
    class Repository
    {
    public:
        Repository() :
            _settings(std::experimental::filesystem::temp_directory_path(), false),
            _database(_settings.test())
        {
        }
        ~Repository() = default;

		template < typename source, typename fileType>
		void dbFileWrite(source& sourceObject, fileType& iniFile) const
		{
			SscfTs1::EventLoop waitEventLoop;
			sourceObject.eventFileCreated = [&](const std::filesystem::path::string_type &) {waitEventLoop.stop(); };
			sourceObject.write(iniFile);
			EXPECT_EQ(waitEventLoop.run(2000), 0);
			sourceObject.eventFileCreated = nullptr;
		}

        // Move below code to IReadOnlyStudyRepository implementation, once addstudy is implemented 
        Database::Patient createPatient(bool ideographicName = true, bool phoneticName = true) const
        {
			SscfTs1::EventLoop waitEventLoop;
            Database::PatientList	patientList(_database.openPatientList());
			patientList.eventPatientCreated = [&](const Database::Patient&) {waitEventLoop.stop(); };
            auto patient = patientList.createPatient();
            if (!patient) return Database::Patient();
			EXPECT_EQ(waitEventLoop.run(2000), 0);

            auto patientInfo = patient.read();
            patientInfo.familyName = L"TestPatientFamilyName";
            patientInfo.middleName = L"TestPatientMiddleName";
            patientInfo.givenName = L"TestPatientGivenName";
            patientInfo.patientId = Database::DateTime(std::chrono::system_clock::now()).toString(L"yyyyMMddhhmmss");
            patientInfo.birthDate = std::chrono::system_clock::now();
            patientInfo.gender = Mini::Models::Database::Patient::Gender::Male;
			patientInfo.type = Mini::Models::Database::Patient::Type::Infant;

			auto patientDicom = patient.read<Mini::Models::Dicom::Patient>();

			patientDicom.patientInfo.personNameAlfabetic.familyName = L"TestPatientFamilyName";
			patientDicom.patientInfo.personNameAlfabetic.middleName = L"TestPatientMiddleName";
			patientDicom.patientInfo.personNameAlfabetic.givenName = L"TestPatientGivenName";

			if(ideographicName)
			{
				patientDicom.patientInfo.personNameIdeografic.familyName = L"TestPatientIdeographicFamilyName";
				patientDicom.patientInfo.personNameIdeografic.middleName = L"TestPatientIdeographicMiddleName";
				patientDicom.patientInfo.personNameIdeografic.givenName = L"TestPatientIdeographicGivenName";
			}

			if(phoneticName)
			{
				patientDicom.patientInfo.personNamePhonetic.familyName = L"TestPatientPhoneticFamilyName";
				patientDicom.patientInfo.personNamePhonetic.middleName = L"TestPatientPhoneticMiddleName";
				patientDicom.patientInfo.personNamePhonetic.givenName = L"TestPatientPhoneticGivenName";
			}			
			dbFileWrite(patient, patientInfo);
			dbFileWrite(patient, patientDicom);
            patient.unhide();
			patientList.eventPatientCreated = nullptr;
            return patient;
        }

        // Move below code to IReadOnlyStudyRepository implementation, once addstudy is implemented 
        Database::Study createStudy(Database::Patient& patient, const Database::ProcedureStep::Info::State& procedureState = Database::ProcedureStep::Info::State::Completed, std::wstring procedureType = L"Peripheral") const
        {
			SscfTs1::EventLoop waitEventLoop;
			patient.eventStudyCreated = [&](const Database::Study&){ waitEventLoop.stop(); };
		    auto study = patient.createStudy();
            if (!study) return Database::Study();
			EXPECT_EQ(waitEventLoop.run(2000), 0);

            auto studyInfo = study.read();
            studyInfo.studyId = L"StudyID";
            studyInfo.studyTime = std::chrono::system_clock::now();;

            auto studyDicom = study.read<Mini::Models::Dicom::Study>();
            studyInfo.weight = 200.00;
            studyInfo.height = 190.00;
            studyInfo.description = L"Test study description";
            studyDicom.accessionNumber = L"TestAccessionNumber";
            studyDicom.requestingService = L"TestRequestingService";
            studyDicom.referringPhysician.personNameAlfabetic.familyName = L"TestReferringPhysician";
            studyDicom.referringPhysician.personNameIdeografic.familyName = L"TestReferringIdeograficPhysician";
            studyDicom.referringPhysician.personNamePhonetic.familyName = L"TestReferringPhoneticPhysician";
            studyDicom.instanceUID = L"TestStudyUID";
            studyInfo.alerts = { L"TestAllert1", L"TestAllert2" };
            studyInfo.allergies = { L"TestAllergy1", L"TestAllergy2" };
			dbFileWrite(study, studyInfo);
			dbFileWrite(study, studyDicom);
            study.unhide();
            
            const auto procedure = createProcedure(study, procedureState, procedureType);
			patient.eventStudyCreated = nullptr;
            if (!procedure)  return study;
            return study;
        }

        Database::ProcedureStep createProcedure(Database::Study& study, const Database::ProcedureStep::Info::State& procedureState = Database::ProcedureStep::Info::State::Completed, std::wstring procedureType = L"Peripheral") const
        {
			SscfTs1::EventLoop waitEventLoop;
			study.eventProcedureStepCreated = [&](const Database::ProcedureStep&) {waitEventLoop.stop();};
            auto procedure = study.createProcedureStep();
            if (!procedure) return Database::ProcedureStep();
			EXPECT_EQ(waitEventLoop.run(2000), 0);
            auto procedureInfo = procedure.read();
            auto dicomMiniInfo = procedure.read<Mini::Models::Dicom::ProcedureStep>();
			procedureInfo.procedureType = procedureType;
            procedureInfo.state = procedureState;
            procedureInfo.startTime = std::chrono::system_clock::now();
            procedureInfo.endTime = std::chrono::system_clock::now();
            procedureInfo.description = L"Test Procedure Description";
            dicomMiniInfo.procedureStepId = Database::DateTime(std::chrono::system_clock::now()).toString(L"yyyyMMddhhmmss");
			dicomMiniInfo.performingOperatorName.personNameAlfabetic.familyName = L"Test Operator";
			dicomMiniInfo.performingOperatorName.personNameIdeografic.familyName = L"TestIdeograficOperator";
			dicomMiniInfo.performingOperatorName.personNamePhonetic.familyName = L"TestPhoneticOperator";
			dbFileWrite(procedure, procedureInfo);
			dbFileWrite(procedure, dicomMiniInfo);
            procedure.unhide();

			study.eventProcedureStepCreated = nullptr;
            return procedure;
        }

        Database::Series createSeries(Database::ProcedureStep& procedure, const Mini::Models::Dicom::Series::Modality& modality = Mini::Models::Dicom::Series::Modality::XA) const
        {
			SscfTs1::EventLoop waitEventLoop;
			procedure.eventSeriesCreated = [&](const Database::Series&){ waitEventLoop.stop();};
            auto series = procedure.createSeries();
            if (!series) return Database::Series();
			EXPECT_EQ(waitEventLoop.run(2000), 0);

            auto seriesInfo = series.read();
            auto dicomMiniInfo = series.read<Mini::Models::Dicom::Series>();
            seriesInfo.acquisitionTime = std::chrono::system_clock::now();;
            seriesInfo.creationTime = std::chrono::system_clock::now();;
            seriesInfo.description = L"Test Series Description";
            seriesInfo.number = 1;
            seriesInfo.clinicaldatatype = L"Test Clinical DataType";
			seriesInfo.clinicaldatatypevariant = L"Test Clinical DataType Variant";
			dicomMiniInfo.modality = modality;
            dicomMiniInfo.instanceUID = Database::DateTime(std::chrono::system_clock::now()).toString(L"yyyyMMddhhmmss");
			dbFileWrite(series, seriesInfo);
			dbFileWrite(series, dicomMiniInfo);
            series.unhide();
			procedure.eventSeriesCreated = nullptr;
            return series;
        }

        bool removeStudy(const Database::Study& study) const
        {
            return _database.openStudyList().removeStudy(study);
        }

        bool removePatient(const Database::Patient& patient) const
        {
            return _database.openPatientList().removePatient(patient);
        }

        void clearDatabase() const
        {
            auto patientList = _database.openPatientList();
            for (const auto& patient : patientList.patients())
                patientList.removePatient(patient);
        }

		Database::Database& getDatabase()
		{
			return _database;
		}
		
    private:
        Settings                                    _settings;
        mutable Database::Database                  _database;
    };


    //TICS -INT#027 -OAL#011 -OLC#009 -INT#002
    // Suppressed as warning is thrown for all Testcases related to override specifier, "new" inside smart pointers 
    // and members are used in TestCases, so made protected
    class StudyRepositoryTest : public testing::Test
    {
    public:
        StudyRepositoryTest() :
            repository(),
            database(Settings().test())
        {
        }

        ~StudyRepositoryTest() = default;

        void SetUp() override
        {
            repository.clearDatabase();
        }

        void TearDown() override
        {
            repository.clearDatabase();
        }

    	bool verifyCollection(const std::vector<std::wstring> &expected, const std::vector<std::string> &actual) const
        {
			std::vector<std::wstring> vectorCompare;
			for (auto const& item : actual)
				vectorCompare.push_back(convert(item));
			return expected == vectorCompare;
        }

    	bool verifyStudyDicom(const Database::Study& databaseStudy, const Study& workflowStudy) const
        {
			const auto info = databaseStudy.read<Mini::Models::Dicom::Study>();
			if (workflowStudy.accessionNumber != convert(info.accessionNumber)) return false;
			if (workflowStudy.requestingService != convert(info.requestingService))   return false;
			if (workflowStudy.instanceUid != convert(info.instanceUID)) return false;
			if (!verifyDicomPersonInfo(info.referringPhysician, workflowStudy.referringPhysician))   return false;
			return true;
        }

    	bool verifyStudyMini(const Database::Study& databaseStudy, const Study& workflowStudy) const
        {
			const auto studyInfo = databaseStudy.read();
			if (workflowStudy.id != convert(studyInfo.studyId)) return false;
			if (workflowStudy.dateTime != DateTime(studyInfo.studyTime))    return false;
			if (workflowStudy.description != convert(studyInfo.description)) return false;
			if (std::to_wstring(workflowStudy.weight) != std::to_wstring(studyInfo.weight)) return false;
			if (std::to_wstring(workflowStudy.height) != std::to_wstring(studyInfo.height)) return false;
			if (!verifyCollection(studyInfo.alerts, workflowStudy.alerts)) return false;
			if (!verifyCollection(studyInfo.allergies, workflowStudy.allergies)) return false;
			for (const auto& comment : workflowStudy.comments)
			{
				if(convert(comment) != (studyInfo.patientComments)) return false;
			}
			if (workflowStudy.importStatus != Study::ImportStatus::None) return false;
			return true;
		}

        bool verifyStudyInfo(Database::Study& study, Study& workflowStudy) const
        {
	        return verifyPatientInfo(study.patient(), workflowStudy.patient)
        		&& verifyPatientDicomInfo(study.patient(), workflowStudy.patient)
        		&& verifyStudyMini(study, workflowStudy)
        		&& verifyStudyDicom(study, workflowStudy);
        }

        bool verifyProcedureInfo(Database::ProcedureStep& dbProcedure, Procedure& wfProcedure)
        {
            auto procedureInfo = dbProcedure.read();
            auto dicomMiniInfo = dbProcedure.read<Mini::Models::Dicom::ProcedureStep>();
            auto dbStudy = dbProcedure.study();

            if (wfProcedure.id != convert(dicomMiniInfo.procedureStepId))           return false;
            if (wfProcedure.description != convert(procedureInfo.description))      return false;
            if (wfProcedure.startTime != DateTime(procedureInfo.startTime))         return false;
            if (wfProcedure.endTime != DateTime(procedureInfo.endTime))             return false;
            if (wfProcedure.state != Procedure::State::Completed)                   return false;
            if (wfProcedure.phase != Procedure::Phase::Acquisition)                 return false;
            if (wfProcedure.type != Procedure::Type::Peripheral)                            return false;

			verifyPersonInfo(procedureInfo.performingPhysician, wfProcedure.performingPhysician);
			verifyPersonInfo(procedureInfo.performingOperatorName, wfProcedure.performingOperator);
            // Verify parent details are filled properly.
            
            if (!verifyStudyInfo(dbStudy, wfProcedure.parent))    return false;
            return true;
        }

        bool verifySeriesInfo(Database::Series& dbSeries, Series& wfSeries)
        {
            auto seriesInfo = dbSeries.read();
            auto dicomMiniInfo = dbSeries.read<Mini::Models::Dicom::Series>();

			auto result =  (wfSeries.number						== seriesInfo.number)
						&& (wfSeries.modality					== Series::Modality::XA)
						&& (wfSeries.acquisitionTime			== DateTime(seriesInfo.acquisitionTime))
						&& (wfSeries.creationTime				== DateTime(seriesInfo.creationTime))
						&& (wfSeries.description				== convert(seriesInfo.description))
						&& (wfSeries.clinicalDataType			== convert(seriesInfo.clinicaldatatype))
						&& (wfSeries.clinicalDataTypeVariant	== convert(seriesInfo.clinicaldatatypevariant))
						&& (wfSeries.instanceUid				== convert(dicomMiniInfo.instanceUID))
						&& (wfSeries.status						== Series::ArchiveStatus::NotSubmitted)
						&& (wfSeries.importStatus == Series::ImportStatus::None);

            // Verify parent details are filled properly.
            auto dbProcedure = dbSeries.procedureStep();
            return result && verifyProcedureInfo(dbProcedure, wfSeries.parent);
        }

        Database::Patient::Info::Gender getGender(const Patient::Gender& gender) const
        {
            Database::Patient::Info::Gender genderInfo = Mini::Models::Database::Patient::Gender::Unknown;;
            switch (gender)
            {
            case  Patient::Gender::Unknown:     genderInfo = Mini::Models::Database::Patient::Gender::Unknown;  break;
            case Patient::Gender::Male:         genderInfo = Mini::Models::Database::Patient::Gender::Male;     break;
            case Patient::Gender::Female:       genderInfo = Mini::Models::Database::Patient::Gender::Female;   break;
            case Patient::Gender::Other:        genderInfo = Mini::Models::Database::Patient::Gender::Other;    break;
            default:    break;
            }
            return genderInfo;
        }

		Database::Patient::Info::Type getPatientType(const Patient::Type& patType) const
		{
			Database::Patient::Info::Type databasePatient = Mini::Models::Database::Patient::Type::Unknown;
			switch (patType)
			{
			case Patient::Type::Neonate:		databasePatient = Mini::Models::Database::Patient::Type::Neonate; break;
			case Patient::Type::Infant:			databasePatient = Mini::Models::Database::Patient::Type::Infant; break;
			case Patient::Type::Child:			databasePatient = Mini::Models::Database::Patient::Type::Child; break;
			case Patient::Type::SmallAdult:		databasePatient = Mini::Models::Database::Patient::Type::SmallAdult; break;
			case Patient::Type::NormalAdult:	databasePatient = Mini::Models::Database::Patient::Type::NormalAdult; break;
			case Patient::Type::LargeAdult:		databasePatient = Mini::Models::Database::Patient::Type::LargeAdult; break;
			case Patient::Type::VeryLargeAdult:	databasePatient = Mini::Models::Database::Patient::Type::VerylargeAdult; break;
			case Patient::Type::Unknown:		databasePatient = Mini::Models::Database::Patient::Type::Unknown; break;
			}
			return databasePatient;
		}

	private:

		bool verifyDicomPatientName(std::map<PersonName::Group, PersonName::Component>::const_iterator component, const Mini::Models::Dicom::Patient::PersonName & dicomPatient) const
		{
			if (dicomPatient.familyName != convert(component->second.familyName)) return false;
			if (dicomPatient.givenName  != convert(component->second.givenName)) return false;
			if (dicomPatient.middleName != convert(component->second.middleName)) return false;
			if (dicomPatient.namePrefix != convert(component->second.namePrefix)) return false;
			if (dicomPatient.nameSuffix != convert(component->second.nameSuffix)) return false;
			return true;
		}

		bool verifyPatientDicomInfo(const Database::Patient& dbPatient, const Patient& wfPatient) const
		{
			bool result = true;
			const auto patientDicom = dbPatient.read<Mini::Models::Dicom::Patient>();

			const auto& component = wfPatient.name.components.find(PersonName::Group::Alphabetic);
			if (component != wfPatient.name.components.end()) 
				result &= verifyDicomPatientName(component, patientDicom.patientInfo.personNameAlfabetic);

			const auto& ideographicComponent = wfPatient.name.components.find(PersonName::Group::Ideographic);
			if (ideographicComponent != wfPatient.name.components.end()) 
				result &= verifyDicomPatientName(ideographicComponent, patientDicom.patientInfo.personNameIdeografic);

			const auto& phoneticComponent = wfPatient.name.components.find(PersonName::Group::Phonetic);
			if (phoneticComponent != wfPatient.name.components.end()) 
				result &= verifyDicomPatientName(phoneticComponent, patientDicom.patientInfo.personNamePhonetic);

			return result;
		}
    	
        bool verifyPatientInfo(const Database::Patient& dbPatient, const Patient& wfPatient) const
        {
            auto patientInfo = dbPatient.read();
            {
                if (getGender(wfPatient.gender) != Mini::Models::Database::Patient::Gender::Male) return false;
                if (getPatientType(wfPatient.type) != Mini::Models::Database::Patient::Type::Infant) return false;
                if (wfPatient.id != convert(patientInfo.patientId))        return false;
                if (wfPatient.birthDate.value() != DateTime(patientInfo.birthDate))return false;

				if (wfPatient.name.components.at(PersonName::Group::Alphabetic).familyName != convert(patientInfo.familyName)) return false;
				if (wfPatient.name.components.at(PersonName::Group::Alphabetic).givenName != convert(patientInfo.givenName)) return false;
				if (wfPatient.name.components.at(PersonName::Group::Alphabetic).middleName != convert(patientInfo.middleName)) return false;
				if (wfPatient.name.components.at(PersonName::Group::Alphabetic).namePrefix != convert(patientInfo.namePrefix)) return false;
				if (wfPatient.name.components.at(PersonName::Group::Alphabetic).nameSuffix != convert(patientInfo.nameSuffix)) return false;
            	
				
            }
            return true;
        }

        bool verifyPersonInfo(const Mini::Models::Database::Study::PersonInfo& dbPersonInfo, const PersonName& wfPersonInfo) const
        {
            for (auto const& iterator : wfPersonInfo.components)
            {
                if (iterator.second.givenName != convert(dbPersonInfo.givenName))   return false;
                if (iterator.second.familyName != convert(dbPersonInfo.familyName)) return false;
                if (iterator.second.middleName != convert(dbPersonInfo.middleName)) return false;
                if (iterator.second.namePrefix != convert(dbPersonInfo.namePrefix)) return false;
                if (iterator.second.nameSuffix != convert(dbPersonInfo.nameSuffix)) return false;
            }
            return true;
        }

		bool verifyDicomPersonInfo(const Mini::Models::Dicom::Study::PersonInfo& dbPersonInfo, const PersonName& wfPersonInfo) const
		{
			bool result = false;
			const auto& component = wfPersonInfo.components.find(PersonName::Group::Alphabetic);
			if (component != wfPersonInfo.components.end()) result = verifyDicomPatientName(component, dbPersonInfo.personNameAlfabetic);

			const auto& ideographicComponent = wfPersonInfo.components.find(PersonName::Group::Ideographic);
			if (ideographicComponent != wfPersonInfo.components.end()) result &= verifyDicomPatientName(ideographicComponent, dbPersonInfo.personNameIdeografic);

			const auto& phoneticComponent = wfPersonInfo.components.find(PersonName::Group::Phonetic);
			if (phoneticComponent != wfPersonInfo.components.end()) result &= verifyDicomPatientName(phoneticComponent, dbPersonInfo.personNamePhonetic);
			return true;
		}
    	
    protected:
        Repository                                  repository;
        mutable Database::Database                  database;
        StudyRepository                             studyrepo;
    };
	
    TEST_F(StudyRepositoryTest, Given_EmptyDB_When_OnePatientIsCreated_Then_VerifyStudy)
    {
        Database::Patient patient = repository.createPatient();
        Database::Study study = repository.createStudy(patient);
        auto studies = studyrepo.studies();

        ASSERT_EQ(studies.size(), 1);
        ASSERT_EQ(verifyStudyInfo(study, studies.at(0)), true);
    }

    TEST_F(StudyRepositoryTest, Given_AStudyInDatabase_When_PatientIsRemoved_Then_ReturnsEmptyStudies)
    {
        Database::Patient patient = repository.createPatient();
        Database::Study study = repository.createStudy(patient);
        auto studies = studyrepo.studies();

        ASSERT_EQ(studies.size(), 1);
        ASSERT_EQ(verifyStudyInfo(study, studies.at(0)), true);
        repository.removePatient(patient);
        studies = studyrepo.studies();
        ASSERT_EQ(studies.size(), 0);
    }

    TEST_F(StudyRepositoryTest, Given_AStudyInDatabase_When_StudiesCreatedUnderDifferentPatients_Then_ReturnStudies)
    {
        Database::Patient patient = repository.createPatient();
        Database::Study study = repository.createStudy(patient);
        Database::Patient patient2 = repository.createPatient();
        Database::Study study2 = repository.createStudy(patient2);
        auto studies = studyrepo.studies();

        ASSERT_EQ(studies.size(), 2);
        ASSERT_EQ(verifyStudyInfo(study, studies.at(0)), true);
        ASSERT_EQ(verifyStudyInfo(study2, studies.at(1)), true);
    }

    TEST_F(StudyRepositoryTest, Given_AMultipleStudiesInDatabase_When_DeleteOneStudy_Then_ReturnStudies)
    {
        Database::Patient patient = repository.createPatient();
        Database::Study study = repository.createStudy(patient);
        Database::Study study2 = repository.createStudy(patient);

        auto studies = studyrepo.studies();
        ASSERT_EQ(studies.size(), 2);
        repository.removeStudy(study2);
        studies = studyrepo.studies();
        ASSERT_EQ(studies.size(), 1);
        ASSERT_EQ(verifyStudyInfo(study, studies.at(0)), true);
    }

    TEST_F(StudyRepositoryTest, Given_AStudyInDatabase_When_DicomMiniIsRemoved_Then_ReturnsEmptyDicomInfo)
    {
        Database::Patient patient = repository.createPatient();
        Database::Study study = repository.createStudy(patient);
        ASSERT_EQ(study.remove(L"dicom.mini"), true);
        ASSERT_EQ(verifyStudyInfo(study, studyrepo.studies().at(0)), true);
    }

    TEST_F(StudyRepositoryTest, Given_EmptyDatabase_When_NewStudyIsAdded_Then_VerfyStudyCreatedEventIsCalled)
    {
        SscfTs1::EventLoop waitEventLoop;
        Study addedStudy;
        studyrepo.studyAdded.subscribe([&](const Study& study)
        {
            addedStudy = study;
            waitEventLoop.stop();
        });

		Database::PatientList	patientList(repository.getDatabase().openPatientList());
		Database::Study study;
		auto patient = patientList.createPatient();
		if (patient) study = patient.createStudy();


        ASSERT_EQ(waitEventLoop.run(2000), 0) << "Timeout. Failed to receive the Event.";      // Wait for the event.
        ASSERT_EQ(addedStudy.id, convert(study.read().studyId));
    }

    TEST_F(StudyRepositoryTest, Given_AStudyInDatabase_When_StudyIsRemoved_Then_VerfyStudyRemovedEventIsCalled)
    {
        SscfTs1::EventLoop waitEventLoop;
        Study addedStudy;
        studyrepo.studyAdded.subscribe([&](const Study& study)
        {
            addedStudy = study;
            waitEventLoop.stop();
        });

		Database::PatientList	patientList(repository.getDatabase().openPatientList());
		Database::Study study;
		auto patient = patientList.createPatient();
		if (patient) study = patient.createStudy();
        ASSERT_EQ(waitEventLoop.run(2000), 0) << "Timeout. Failed to receive the Event.";

        auto studies = studyrepo.studies();
        ASSERT_EQ(studies.size(), 1);
        auto wfStudy = studies[0];
        IRepository::Model modelRemoved;
        studyrepo.studyRemoved.subscribe([&](const IRepository::Model& model)
        {
            modelRemoved = model;
            waitEventLoop.stop();
        });
        repository.removeStudy(study);
        
        ASSERT_EQ(waitEventLoop.run(2000), 0) << "Timeout. Failed to receive the Event.";
        ASSERT_EQ(IRepository::Model(wfStudy), modelRemoved);
    }

    TEST_F(StudyRepositoryTest, Given_EmptyDB_When_SingleProcedureIsCreated_Then_VerifyReturnedProcedure)
    {
        Database::Patient patient = repository.createPatient();
        Database::Study study = repository.createStudy(patient);
        Database::ProcedureStep procedure = *study.procedureSteps().begin();

        auto studies = studyrepo.studies();
        ASSERT_EQ(studies.size(), 1);

        auto procedures = studyrepo.procedures(studies[0]);
        ASSERT_EQ(procedures.size(), 1);
        ASSERT_EQ(verifyProcedureInfo(procedure, procedures[0]), true);
    }

    TEST_F(StudyRepositoryTest, Given_EmptyDB_When_MultipleProceduresAreCreated_Then_VerifyReturnedProcedures)
    {
        Database::Patient patient = repository.createPatient();
        Database::Study study = repository.createStudy(patient);
        Database::ProcedureStep procedure = repository.createProcedure(study);
        auto studies = studyrepo.studies();

        ASSERT_EQ(studies.size(), 1);
        ASSERT_EQ(studyrepo.procedures(studies[0]).size(), 2);
    }

    TEST_F(StudyRepositoryTest, Given_AProcedureInDatabase_When_DicomMiniIsRemoved_Then_ReturnsEmptyDicomInfo)
    {
        Database::Patient patient = repository.createPatient();
        Database::Study study = repository.createStudy(patient);
        Database::ProcedureStep procedure = *study.procedureSteps().begin();

        ASSERT_EQ(procedure.remove(L"dicom.mini"), true);

        auto studies = studyrepo.studies();
        ASSERT_EQ(studies.size(), 1);

        auto procedures = studyrepo.procedures(studies[0]);
        ASSERT_EQ(procedures.size(), 1);
        ASSERT_TRUE(procedures[0].id.empty());
    }

    TEST_F(StudyRepositoryTest, Given_ProceduresInDatabase_When_InvalidStudyIsInput_Then_ReturnsEmptyProcedures)
    {
        Database::Patient patient1 = repository.createPatient();
        Database::Study study1 = repository.createStudy(patient1);
        auto procedures = studyrepo.procedures(Study("testKey"));
        ASSERT_EQ(procedures.size(), 0);
    }

	TEST_F(StudyRepositoryTest, Given_EmptyDatabase_When_ProcedureIsCreated_Then_VerifyProcedureType)
	{
		auto dbPatient = repository.createPatient();
		auto dbStudy = repository.createStudy(dbPatient, Database::ProcedureStep::Info::State::Completed, L"Coronary");
		auto dbProcedure = *dbStudy.procedureSteps().begin();
		auto dbSeries = repository.createSeries(dbProcedure);

		auto studies = studyrepo.studies();
		ASSERT_EQ(studies.size(), 1);

		auto procedures = studyrepo.procedures(studies[0]);
		ASSERT_EQ(procedures.size(), 1);
		ASSERT_EQ(procedures[0].type, Procedure::Type::Coronary);
	}

	TEST_F(StudyRepositoryTest, Given_EmptyDatabase_When_ProcedureIsCreatedWithEmptyProcedureType_Then_VerifyProcedureType)
	{
		auto dbPatient = repository.createPatient();
		auto dbStudy = repository.createStudy(dbPatient, Database::ProcedureStep::Info::State::Completed, L"");
		auto dbProcedure = *dbStudy.procedureSteps().begin();
		auto dbSeries = repository.createSeries(dbProcedure);

		auto studies = studyrepo.studies();
		ASSERT_EQ(studies.size(), 1);

		auto procedures = studyrepo.procedures(studies[0]);
		ASSERT_EQ(procedures.size(), 1);
		ASSERT_EQ(procedures[0].type, Procedure::Type::Unknown);
	}

    TEST_F(StudyRepositoryTest, Given_EmptyDB_When_SingleSeriesIsCreated_Then_VerifyReturnedSeries)
    {
        auto dbPatient = repository.createPatient();
        auto dbStudy = repository.createStudy(dbPatient);
        auto dbProcedure = *dbStudy.procedureSteps().begin();
        auto dbSeries = repository.createSeries(dbProcedure);

        auto studies = studyrepo.studies();
        ASSERT_EQ(studies.size(), 1);
        auto procedures = studyrepo.procedures(studies[0]);
        ASSERT_EQ(procedures.size(), 1);

        auto series = studyrepo.series(procedures[0]);
        ASSERT_EQ(series.size(), 1);
        ASSERT_EQ(verifySeriesInfo(dbSeries, series[0]), true);
    }

    TEST_F(StudyRepositoryTest, Given_EmptyDB_When_MultipleSeriesAreCreated_Then_VerifyReturnedSeries)
    {
        auto dbPatient = repository.createPatient();
        auto dbStudy = repository.createStudy(dbPatient);
        auto dbProcedure = *dbStudy.procedureSteps().begin();
        auto dbSeries1 = repository.createSeries(dbProcedure);
        auto dbSeries2 = repository.createSeries(dbProcedure);
        auto dbSeries3 = repository.createSeries(dbProcedure);

        auto studies = studyrepo.studies();
        ASSERT_EQ(studies.size(), 1);
        auto procedures = studyrepo.procedures(studies[0]);
        ASSERT_EQ(procedures.size(), 1);

        auto series = studyrepo.series(procedures[0]);
        ASSERT_EQ(series.size(), 3);
    }

	TEST_F(StudyRepositoryTest, Given_SingleSeriesIsDatabase_When_SeriesIsCalledWithParentStudy_Then_VerifyReturnedSeries)
	{
		auto dbPatient = repository.createPatient();
		const auto dbStudy = repository.createStudy(dbPatient);
		auto dbProcedure = *dbStudy.procedureSteps().begin();
		auto dbSeries = repository.createSeries(dbProcedure);

		auto studies = studyrepo.studies();
		ASSERT_EQ(studies.size(), 1);
		auto procedures = studyrepo.procedures(studies[0]);
		ASSERT_EQ(procedures.size(), 1);

		auto series = studyrepo.series(studies[0]);
		ASSERT_EQ(series.size(), 1);
		ASSERT_TRUE(verifyProcedureInfo(dbProcedure, series[0].parent));
		ASSERT_TRUE(verifySeriesInfo(dbSeries, series[0]));
	}

	TEST_F(StudyRepositoryTest, Given_MultipleSeriesInDatabaseUnderMultipleProceduresUnderTheSameStudy_When_SeriesCalledWithParentStudy_Then_VerifyReturnedSeries)
	{
		Database::Patient patient = repository.createPatient();
		Database::Study study = repository.createStudy(patient);
		Database::ProcedureStep procedure1 = repository.createProcedure(study);
		Database::ProcedureStep procedure2 = repository.createProcedure(study);
		auto dbSeries1 = repository.createSeries(procedure1);
		auto dbSeries2 = repository.createSeries(procedure1);
		auto dbSeries3 = repository.createSeries(procedure2);

		auto studies = studyrepo.studies();
		ASSERT_EQ(studies.size(), 1);
		auto procedures = studyrepo.procedures(studies[0]);
		ASSERT_EQ(procedures.size(), 3);

		auto series = studyrepo.series(studies[0]);
		ASSERT_EQ(series.size(), 3);

		ASSERT_TRUE(verifyProcedureInfo(procedure1, series[0].parent));
		ASSERT_TRUE(verifySeriesInfo(dbSeries1, series[0]));
		ASSERT_TRUE(verifyProcedureInfo(procedure1, series[1].parent));
		ASSERT_TRUE(verifySeriesInfo(dbSeries2, series[1]));
		ASSERT_TRUE(verifyProcedureInfo(procedure2, series[2].parent));
		ASSERT_TRUE(verifySeriesInfo(dbSeries3, series[2]));
	}

	TEST_F(StudyRepositoryTest, Given_TwoStudiesInDatabaseAndFirstStudyHasOneSeries_When_SeriesIsCalledWithSecondStudy_Then_EmptySeriesListReturned)
	{
		auto dbPatient = repository.createPatient();
		const auto dbStudy = repository.createStudy(dbPatient);
		auto dummyStudy = repository.createStudy(dbPatient);
		auto dbProcedure = *dbStudy.procedureSteps().begin();
		auto dbSeries = repository.createSeries(dbProcedure);

		auto studies = studyrepo.studies();
		ASSERT_EQ(studies.size(), 2);
		auto procedures = studyrepo.procedures(studies[0]);
		ASSERT_EQ(procedures.size(), 1);

		auto series = studyrepo.series(studies[1]);
		ASSERT_EQ(series.size(), 0);
	}

	TEST_F(StudyRepositoryTest, Given_SeriesInDatabase_When_SeriesIsCalledWithInvalidStudy_Then_ReturnsEmptySeriesList)
	{
		auto dbPatient = repository.createPatient();
		auto dbStudy = repository.createStudy(dbPatient);
		auto dbProcedure = *dbStudy.procedureSteps().begin();
		auto dbSeries1 = repository.createSeries(dbProcedure);

		auto series = studyrepo.series(Study("TestKey"));
		ASSERT_EQ(series.size(), 0);
	}

	TEST_F(StudyRepositoryTest, Given_Study_When_StudyIsAddedInStudyRepository_Then_Verify_StudiesInRepository)
	{
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		auto addedStudy = studyrepo.studies();
		ASSERT_EQ(addedStudy.size(), 1);
		ASSERT_EQ(study, addedStudy[0]);
	}

	TEST_F(StudyRepositoryTest, Given_Procedure_When_ProcedureIsAddedInStudyRepository_Then_Verify_ProcedureInRepository)
	{
		auto procedure = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
	    procedure.parent = study;
		ASSERT_TRUE(studyrepo.add(procedure));
		auto addedProcedure = studyrepo.procedures(studyrepo.studies().at(0));
		ASSERT_EQ(addedProcedure.size(), 1);
		ASSERT_EQ(procedure, addedProcedure[0]);
	}

	TEST_F(StudyRepositoryTest, Given_Procedure_When_InvalidProcedureIsAddedInStudyRepository_Then_Verify_ProcedureInRepository)
	{
		auto procedure = newProcedure(Procedure::Type::Peripheral);
		auto study = newStudy();
		procedure.parent = study;
		ASSERT_FALSE(studyrepo.add(procedure));
	}

	TEST_F(StudyRepositoryTest, Given_Study_When_StudyInRepositoryIsModified_Then_Verify_UpdateStudyRepository)
	{
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));

		study.height = 0;
		study.alerts.clear();
		study.alerts.emplace_back("updated alert");
		study.allergies.clear();
		study.allergies.emplace_back("updated allergy");
		study.patient.gender = Patient::Gender::Other;
		study.patient.type = Patient::Type::LargeAdult;
		study.comments.clear();
		study.comments.emplace_back("updated comments");
		ASSERT_TRUE(studyrepo.update(study));

		study = studyrepo.studies().at(0);
		ASSERT_EQ(study.height, 0);
		ASSERT_EQ(int(study.alerts.size()), 1);
		ASSERT_EQ(study.alerts[0], "updated alert");
		ASSERT_EQ(int(study.allergies.size()), 1);
		ASSERT_EQ(study.allergies[0], "updated allergy");
		ASSERT_EQ(int(study.comments.size()), 1);
		ASSERT_EQ(study.comments[0], "updated comments");
		ASSERT_EQ(study.patient.gender, Patient::Gender::Other);
		ASSERT_EQ(study.patient.type , Patient::Type::LargeAdult);
	}

	TEST_F(StudyRepositoryTest, Given_Study_When_InvalidStudyInRepositoryIsModified_Then_Verify_UpdateStudyRepository)
	{
		auto study = newStudy();
		study.height = 0;
		ASSERT_FALSE(studyrepo.update(study));
	}

	TEST_F(StudyRepositoryTest, Given_Procedure_When_ProcedureInRepositoryIsModified_Then_Verify_UpdateProcedureRepository)
	{
		auto procedure = newProcedure(Procedure::Type::Peripheral);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure.parent = study;
		ASSERT_TRUE(studyrepo.add(procedure));
		procedure.description = "Updated Description";
		ASSERT_TRUE(studyrepo.update(procedure));
		ASSERT_EQ(studyrepo.procedures(study).at(0).description, "Updated Description");
	}

	TEST_F(StudyRepositoryTest, Given_Procedure_When_InvalidProcedureInRepositoryIsModified_Then_Verify_UpdateProcedureRepository)
	{
		auto procedure = newProcedure(Procedure::Type::Peripheral);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure.parent = study;
		procedure.description = "Updated Description";
		ASSERT_FALSE(studyrepo.update(procedure));
	}


	TEST_F(StudyRepositoryTest, Given_Procedure_When_InvalidProcedureOperatorIsModified_Then_Verify_UpdatedOperator)
	{
		auto procedure = newProcedure(Procedure::Type::Peripheral);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure.parent = study;
		PersonName pName;
		pName.components.insert({ PersonName::Group::Alphabetic, PersonName::Component{"Updatedfamily",
			"pName", "pMiddle", "pPrefix", "pSuffix"} });
		procedure.performingOperator = pName;
		ASSERT_FALSE(studyrepo.update(procedure));
	}

	TEST_F(StudyRepositoryTest, Given_Procedure_When_OperatorInRepositoryIsModified_Then_Verify_UpdateOperatorInRepository)
	{
		auto procedure = newProcedure(Procedure::Type::Peripheral);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure.parent = study;
		ASSERT_TRUE(studyrepo.add(procedure));
		PersonName pName;
		pName.components.insert({ PersonName::Group::Alphabetic, PersonName::Component{"Updatedfamily",
			"UpdatedName", "UpdatedMiddle", "UpdatedPrefix", "UpdatedSuffix"} });
		procedure.performingOperator = pName;
		ASSERT_TRUE(studyrepo.update(procedure));
		ASSERT_EQ(studyrepo.procedures(study).at(0).performingOperator.toString(), pName.toString());		
	}

	TEST_F(StudyRepositoryTest, Given_Procedure_When_PerformingPhysicianInRepositoryIsModified_Then_Verify_UpdatedPhysicianInRepository)
	{
		auto procedure = newProcedure(Procedure::Type::Peripheral);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure.parent = study;
		ASSERT_TRUE(studyrepo.add(procedure));
		PersonName pName;
		pName.components.insert({ PersonName::Group::Alphabetic, PersonName::Component{"Updatedfamily",
			"UpdatedMiddle", "UpdatedMiddle", "UpdatedPrefix", "pSuffix"} });
		procedure.performingPhysician = pName;
		ASSERT_TRUE(studyrepo.update(procedure));
		ASSERT_EQ(studyrepo.procedures(study).at(0).performingPhysician.toString(), pName.toString());
	}

    TEST_F(StudyRepositoryTest, Given_SeriesInDatabase_When_InvalidProcedureIsInput_Then_ReturnsEmptySeries)
    {
        auto dbPatient = repository.createPatient();
        auto dbStudy = repository.createStudy(dbPatient);
        auto dbProcedure = *dbStudy.procedureSteps().begin();
        auto dbSeries1 = repository.createSeries(dbProcedure);

        auto series = studyrepo.series(Procedure("TestKey"));
        ASSERT_EQ(series.size(), 0);
    }

    TEST_F(StudyRepositoryTest, Given_ASeriesInDatabase_When_DicomMiniIsRemoved_Then_ReturnsEmptyDicomInfo)
    {
        auto dbPatient = repository.createPatient();
        auto dbStudy = repository.createStudy(dbPatient);
        auto dbProcedure = *dbStudy.procedureSteps().begin();
        auto dbSeries = repository.createSeries(dbProcedure);

        ASSERT_EQ(dbSeries.remove(L"dicom.mini"), true);

        auto studies = studyrepo.studies();
        ASSERT_EQ(studies.size(), 1);
        auto procedures = studyrepo.procedures(studies[0]);
        ASSERT_EQ(procedures.size(), 1);

        auto series = studyrepo.series(procedures[0]);
        ASSERT_EQ(series.size(), 1);
        ASSERT_EQ(series[0].modality, Series::Modality::XA);
        ASSERT_TRUE(series[0].instanceUid.empty());
    }

	TEST_F(StudyRepositoryTest, Given_Patient_When_PatientTypeIsNotProvided_Then_Verify_DefaultPatientTypeInRepository)
	{
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		ASSERT_EQ(studyrepo.studies().at(0).patient.type, Patient::Type::Unknown);
	}

	TEST_F(StudyRepositoryTest, Given_Patient_When_PatientTypeIsModified_Then_Verify_UpdatedPatientTypeInRepository)
	{
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		study.patient.type = Patient::Type::Child;
		ASSERT_TRUE(studyrepo.update(study));
		ASSERT_EQ(studyrepo.studies().at(0).patient.type, Patient::Type::Child);
	}

	typedef std::pair<std::string, Procedure::ProcedureSource> ProcedureSourceResultParam;

	//TICS -INT#027 -OAL#011 -OLC#009 -INT#002
	// Suppressed as warning is thrown for all Testcases related to override specifier, "new" inside smart pointers 
	// and members are used in TestCases, so made protected
	class When_ProcedureSourceAvailableInDb : public testing::TestWithParam<ProcedureSourceResultParam>
	{
	public:
		void SetUp() override
		{
			repository.clearDatabase();
		}

		void TearDown() override
		{
			repository.clearDatabase();
		}

	protected:
		Repository          repository;
		StudyRepository     studyrepo;

	};
	INSTANTIATE_TEST_CASE_P(test, When_ProcedureSourceAvailableInDb, ::testing::Values(
		ProcedureSourceResultParam("Local", Procedure::ProcedureSource::Local),
		ProcedureSourceResultParam("Wlm", Procedure::ProcedureSource::Wlm),
		ProcedureSourceResultParam("Cwis", Procedure::ProcedureSource::Cwis)
	));

	TEST_P(When_ProcedureSourceAvailableInDb, Then_VerifyReturnedProcedureSource)
	{
		const auto params = GetParam();
		const auto source = params.second;


		Study study;
		study.id = 1;
		ASSERT_TRUE(studyrepo.add(study));
		Procedure procedure;
		procedure.parent = study;
		procedure.procedureSource = source;
		ASSERT_TRUE(studyrepo.add(procedure));

		auto procedures = studyrepo.procedures(study);
		ASSERT_EQ(procedures.size(), 1);
		ASSERT_EQ(procedures[0].procedureSource, source) << "Expected->" << params.first;
	}

    typedef std::pair<std::string, Procedure::Phase> ProcedurePhaseResultParam;

    //TICS -INT#027 -OAL#011 -OLC#009 -INT#002
    // Suppressed as warning is thrown for all Testcases related to override specifier, "new" inside smart pointers 
    // and members are used in TestCases, so made protected
    class When_ProcedureAvailableInDb : public testing::TestWithParam<ProcedurePhaseResultParam>
    {
    public:
        void SetUp() override
        {
            repository.clearDatabase();
        }

        void TearDown() override
        {
            repository.clearDatabase();
        }

    protected:
        Repository          repository;
        StudyRepository     studyrepo;
        
    };

    INSTANTIATE_TEST_CASE_P(test, When_ProcedureAvailableInDb, ::testing::Values(
        ProcedurePhaseResultParam("Review", Procedure::Phase::Review),
        ProcedurePhaseResultParam("Acquisition", Procedure::Phase::Acquisition)
    ));

    TEST_P(When_ProcedureAvailableInDb, Then_VerifyReturnedProcedurePhase)
    {
        const auto params = GetParam();
        const auto phase = params.second;

		Study study;
		study.id = 1;
		ASSERT_TRUE(studyrepo.add(study));
		Procedure procedure;
		procedure.parent = study;
		procedure.phase = phase;
		ASSERT_TRUE(studyrepo.add(procedure));

        auto procedures = studyrepo.procedures(study);
        ASSERT_EQ(procedures.size(), 1);
        ASSERT_EQ(procedures[0].phase, phase) << "Expected->" << params.first;
    }


    typedef std::pair<Mini::Models::Database::ProcedureStep::State, Procedure::State> ProcedureStateResultParam;

    //TICS -INT#027 -OAL#011 -OLC#009 -INT#002
    // Suppressed as warning is thrown for all Testcases related to override specifier, "new" inside smart pointers 
    // and members are used in TestCases, so made protected
    class When_ProcedureWithStateAvailableInDb : public testing::TestWithParam<ProcedureStateResultParam>
    {
    public:
        void SetUp() override
        {
            repository.clearDatabase();
        }

        void TearDown() override
        {
            repository.clearDatabase();
        }

    protected:
        Repository          repository;
        StudyRepository     studyrepo;
    };

    INSTANTIATE_TEST_CASE_P(test, When_ProcedureWithStateAvailableInDb, ::testing::Values(
        ProcedureStateResultParam(Mini::Models::Database::ProcedureStep::State::Scheduled, Procedure::State::Scheduled),
        ProcedureStateResultParam(Mini::Models::Database::ProcedureStep::State::Suspended, Procedure::State::Suspended),
        ProcedureStateResultParam(Mini::Models::Database::ProcedureStep::State::InProgress, Procedure::State::InProgress),
        ProcedureStateResultParam(Mini::Models::Database::ProcedureStep::State::Completed, Procedure::State::Completed)
    ));

    TEST_P(When_ProcedureWithStateAvailableInDb, Then_VerifyReturnedProcedureState)
    {
        const auto params = GetParam();
        const auto dbState = params.first;
        const auto wfState = params.second;
        Database::Patient patient = repository.createPatient();
        Database::Study study = repository.createStudy(patient, dbState);
        Database::ProcedureStep procedure = *study.procedureSteps().begin();

        auto studies = studyrepo.studies();
        ASSERT_EQ(studies.size(), 1);
        auto procedures = studyrepo.procedures(studies[0]);
        ASSERT_EQ(procedures.size(), 1);
        ASSERT_EQ(procedures[0].state, wfState);
    }


    typedef std::pair<Mini::Models::Dicom::Series::Modality, Series::Modality> ModalityResultParam;

    //TICS -INT#027 -OAL#011 -OLC#009 -INT#002
    // Suppressed as warning is thrown for all Testcases related to override specifier, "new" inside smart pointers 
    // and members are used in TestCases, so made protected
    class When_SeriesWithModalityAvailableInDb : public testing::TestWithParam<ModalityResultParam>
    {
    public:
        void SetUp() override
        {
            repository.clearDatabase();
        }

        void TearDown() override
        {
            repository.clearDatabase();
        }

    protected:
        Repository          repository;
        StudyRepository     studyrepo;
    };

    INSTANTIATE_TEST_CASE_P(test, When_SeriesWithModalityAvailableInDb, ::testing::Values(
        ModalityResultParam(Mini::Models::Dicom::Series::Modality::XA, Series::Modality::XA),
        ModalityResultParam(Mini::Models::Dicom::Series::Modality::IVUS, Series::Modality::IVUS),
        ModalityResultParam(Mini::Models::Dicom::Series::Modality::US, Series::Modality::US),
        ModalityResultParam(Mini::Models::Dicom::Series::Modality::MR, Series::Modality::MR),
        ModalityResultParam(Mini::Models::Dicom::Series::Modality::CT, Series::Modality::CT)
    ));

    TEST_P(When_SeriesWithModalityAvailableInDb, Then_VerifyReturnedSeriesModality)
    {
        const auto params = GetParam();
        const auto dbModality = params.first;
        const auto wfModality = params.second;
        auto dbPatient = repository.createPatient();
        auto dbStudy = repository.createStudy(dbPatient);
        auto dbProcedure = *dbStudy.procedureSteps().begin();
        auto dbSeries = repository.createSeries(dbProcedure, dbModality);

        auto studies = studyrepo.studies();
        ASSERT_EQ(studies.size(), 1);
        auto procedures = studyrepo.procedures(studies[0]);
        ASSERT_EQ(procedures.size(), 1);

        auto series = studyrepo.series(procedures[0]);
        ASSERT_EQ(series.size(), 1);
        ASSERT_EQ(series[0].modality, wfModality);
    }

    typedef std::pair<std::wstring, Procedure::Type> ProcedureTypeResultParam;

    //TICS -INT#027 -OAL#011 -OLC#009 -INT#002
    // Suppressed as warning is thrown for all Testcases related to override specifier, "new" inside smart pointers 
    // and members are used in TestCases, so made protected
    class When_ProcedureWithTypeAvailableInDb : public testing::TestWithParam<ProcedureTypeResultParam>
    {
    public:
        void SetUp() override
        {
            repository.clearDatabase();
        }

        void TearDown() override
        {
            repository.clearDatabase();
        }

    protected:
        Repository          repository;
        StudyRepository     studyrepo;
    };

    INSTANTIATE_TEST_CASE_P(test, When_ProcedureWithTypeAvailableInDb, ::testing::Values(
        ProcedureTypeResultParam(L"Coronary", Procedure::Type::Coronary),
        ProcedureTypeResultParam(L"Peripheral", Procedure::Type::Peripheral)
    ));

    TEST_P(When_ProcedureWithTypeAvailableInDb, Then_VerifyReturnedProcedureType)
    {
        const auto params = GetParam();
        const auto dbType = params.first;
        const auto wfType = params.second;
        auto dbPatient = repository.createPatient();
        auto dbStudy = repository.createStudy(dbPatient,Database::ProcedureStep::Info::State::Completed , dbType);
        auto dbProcedure = *dbStudy.procedureSteps().begin();

        auto studies = studyrepo.studies();
        ASSERT_EQ(studies.size(), 1);
        auto procedures = studyrepo.procedures(studies[0]);
        ASSERT_EQ(procedures.size(), 1);
        ASSERT_EQ(procedures[0].type, wfType);
    }

	TEST_F(StudyRepositoryTest, Given_ProcedureInDatabase_When_ProcedureTypeNotAvailable_Then_ProcedureTypeIsUnknown)
	{
		auto procedure = newProcedure(Procedure::Type::Unknown);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure.parent = study;
		ASSERT_TRUE(studyrepo.add(procedure));
		auto addedProcedure = studyrepo.procedures(study);
		ASSERT_EQ(addedProcedure[0].type, Procedure::Type::Unknown);
	}

    TEST_F(StudyRepositoryTest, Given_AStudyInDatabase_When_PatientisProtected_Then_VerifyProtectState)
    {
        Database::Patient patient = repository.createPatient();
        Database::Study   dbStudy = repository.createStudy(patient);

        auto path = dbStudy.path();
        path /= L"protected";
        std::ofstream lock(path);
        lock << "study.is.locked";
        lock.close();

        auto studies = studyrepo.studies();
        ASSERT_EQ(studyrepo.isProtected(studies[0]) , true);
    }

	TEST_F(StudyRepositoryTest, Given_SingleStudyInDatabase_When_RemoveCalled_Then_StudyAndPatientAreRemoved)
	{
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));

		ASSERT_TRUE(studyrepo.remove(study));
		ASSERT_EQ(studyrepo.studies().size(), 0);
		ASSERT_EQ(repository.getDatabase().openPatientList().patients().size(),0);
	}

	TEST_F(StudyRepositoryTest, Given_MultipleStudiesInDatabase_When_OneStudyISRemoved_Then_TheStudyRemovedButPatientNotRemoved)
	{
		auto study = newStudy();
		auto study1 = newStudy();
		study1.id = "id1";
		ASSERT_TRUE(studyrepo.add(study));
		ASSERT_TRUE(studyrepo.add(study1));
		ASSERT_EQ(studyrepo.studies().size(), 2);

		ASSERT_TRUE(studyrepo.remove(study));
		ASSERT_EQ(studyrepo.studies().size(), 1);
		ASSERT_EQ(studyrepo.studies().at(0).id, "id1");
		ASSERT_EQ(repository.getDatabase().openPatientList().patients().size(), 1);
	}


	TEST_F(StudyRepositoryTest, Given_ProcedureInDatabase_When_RemoveCalled_Then_ProcedureIsRemoved)
	{
		auto procedure = newProcedure(Procedure::Type::Unknown);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure.parent = study;
		ASSERT_TRUE(studyrepo.add(procedure));
		ASSERT_TRUE(studyrepo.remove(procedure));
	}

	TEST_F(StudyRepositoryTest, Given_SeriesInDatabase_When_RemoveCalled_Then_SeriesIsRemoved)
	{
		auto dbPatient = repository.createPatient();
		auto dbStudy = repository.createStudy(dbPatient);
		auto dbProcedure = *dbStudy.procedureSteps().begin();
		auto dbSeries = repository.createSeries(dbProcedure);

		const Series series(dbSeries.path().u8string());
		ASSERT_TRUE(studyrepo.remove(series));
	}

	TEST_F(StudyRepositoryTest, When_InvalidStudyWithEmptyKeySubscribedForNotification_Then_StudyNotificationReturnNullPointer)
	{
		const auto study1 = Study();
		const std::unique_ptr<IStudyNotification> notification = studyrepo.subscribe(study1);
		ASSERT_TRUE(notification == nullptr);
	}

	TEST_F(StudyRepositoryTest, When_ValidStudySubscribedForNotification_Then_StudyNotificationReturnsNonNullPointer)
	{
		auto study1 = newStudy();
		studyrepo.add(study1);
		const std::unique_ptr<IStudyNotification> notification = studyrepo.subscribe(study1);
		ASSERT_TRUE(notification != nullptr);
	}

	TEST_F(StudyRepositoryTest, Given_StudyTwo_When_NotPresentInDatabaseAndSuscribedForNotification_Then_StudyNotificationReturnsNullPointer)
	{
		auto study1 = newStudy();
		studyrepo.add(study1);
		const auto study2 = newStudy();
		const std::unique_ptr<IStudyNotification> notification = studyrepo.subscribe(study2);
		ASSERT_TRUE(notification == nullptr);
	}

	TEST_F(StudyRepositoryTest, When_InvalidProcedureWithEmptyKeySubscribedForNotification_Then_ProcedureNotificationReturnNullPointer)
	{
		const auto procedure = Procedure();
		const std::unique_ptr<IProcedureNotification> notification = studyrepo.subscribe(procedure);
		ASSERT_TRUE(notification == nullptr);
	}

	TEST_F(StudyRepositoryTest, When_ValidProcdureSubscribedForNotification_Then_ProcedureNotificationReturnsNonNullPointer)
	{
		auto procedure1 = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		studyrepo.add(study);
		procedure1.parent = study;
		studyrepo.add(procedure1);
		const std::unique_ptr<IProcedureNotification> notification = studyrepo.subscribe(procedure1);
		ASSERT_TRUE(notification != nullptr);
	}
	
	TEST_F(StudyRepositoryTest, Given_ProcedureTwo_When_NotPresentInDatabaseAndSuscribedForNotification_Then_StudyNotificationReturnsNullPointer)
	{
		auto procedure1 = newProcedure(Procedure::Type::Coronary);
		const auto procedure2 = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		studyrepo.add(study);
		procedure1.parent = study;
		studyrepo.add(procedure1);
		const std::unique_ptr<IProcedureNotification> notification = studyrepo.subscribe(procedure2);
		ASSERT_TRUE(notification == nullptr);
	}

	TEST_F(StudyRepositoryTest, When_InvalidSeriesWithEmptyKeySubscribedForNotification_Then_SeriesNotificationReturnNullPointer)
	{
		const auto series = Series();
		const std::unique_ptr<ISeriesNotification> notification = studyrepo.subscribe(series);
		ASSERT_TRUE(notification == nullptr);
	}
	
	TEST_F(StudyRepositoryTest, When_ValidSeriesIsSubscribedForNotification_Then_SeriesNotificationReturnsNonNullPointer)
	{
		auto procedure1 = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		studyrepo.add(study);
		procedure1.parent = study;
		studyrepo.add(procedure1);
		auto series = Series("Key");
		series.parent = procedure1;
		studyrepo.add(series);
		const std::unique_ptr<ISeriesNotification> notification = studyrepo.subscribe(series);
		ASSERT_TRUE(notification != nullptr);
	}
	
	TEST_F(StudyRepositoryTest, Given_Series_When_NotPresentInDatabaseAndSuscribedForNotification_Then_SeriesNotificationReturnsNullPointer)
	{
		auto procedure = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		studyrepo.add(study);
		procedure.parent = study;
		studyrepo.add(procedure);
		auto series = Series("Key1");
		const std::unique_ptr<ISeriesNotification> notification = studyrepo.subscribe(series);
		ASSERT_TRUE(notification == nullptr);
	}

	TEST_F(StudyRepositoryTest, Given_Study_When_ImportStatusIsUpdated_Then_VerifyStudyImportStatus)
	{
		auto study = newStudy();
		study.importStatus = Study::ImportStatus::InProgress;
		ASSERT_TRUE(studyrepo.add(study));
		auto addedStudy = studyrepo.studies();
		ASSERT_EQ(addedStudy[0].importStatus, Study::ImportStatus::InProgress);
		study.importStatus = Study::ImportStatus::Completed;
		ASSERT_TRUE(studyrepo.update(study));
		addedStudy = studyrepo.studies();
		ASSERT_EQ(addedStudy.size(), 1);
		ASSERT_EQ(addedStudy[0].importStatus, Study::ImportStatus::Completed);
	}

	TEST_F(StudyRepositoryTest, Given_Series_When_ImportStatusIsUpdated_Then_VerifySeriesImportStatus)
	{
		auto procedure = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure.parent = study;
		ASSERT_TRUE(studyrepo.add(procedure));
		auto series = newSeries();
		series.importStatus = Series::ImportStatus::InProgress;
		series.parent = procedure;
		ASSERT_TRUE(studyrepo.add(series));
		auto addedSeries = studyrepo.series(study);
		ASSERT_EQ(addedSeries[0].importStatus, Series::ImportStatus::InProgress);
		series.importStatus = Series::ImportStatus::Completed;
		ASSERT_TRUE(studyrepo.update(series));
		addedSeries = studyrepo.series(study);
		ASSERT_EQ(addedSeries[0].importStatus, Series::ImportStatus::Completed);
	}

	TEST_F(StudyRepositoryTest, Given_AllComponentsInName_When_Studies_Then_VerifyAllComponentsReceived)
	{
		Database::Patient patient = repository.createPatient();
		Database::Study study = repository.createStudy(patient);
		auto studies = studyrepo.studies();
    	
		ASSERT_NE(studies.at(0).patient.name.components.find(PersonName::Group::Alphabetic), studies.at(0).patient.name.components.end());
		ASSERT_NE(studies.at(0).patient.name.components.find(PersonName::Group::Ideographic), studies.at(0).patient.name.components.end());
		ASSERT_NE(studies.at(0).patient.name.components.find(PersonName::Group::Phonetic), studies.at(0).patient.name.components.end());
	}
	
	TEST_F(StudyRepositoryTest, Given_OnlyAlphabeticName_When_Studies_Then_VerifyOnlyAlphabeticReceived)
	{
		Database::Patient patient = repository.createPatient(false, false);
		Database::Study study = repository.createStudy(patient);
		auto studies = studyrepo.studies();
    	
		ASSERT_NE(studies.at(0).patient.name.components.find(PersonName::Group::Alphabetic), studies.at(0).patient.name.components.end());
		ASSERT_EQ(studies.at(0).patient.name.components.find(PersonName::Group::Ideographic), studies.at(0).patient.name.components.end());
		ASSERT_EQ(studies.at(0).patient.name.components.find(PersonName::Group::Phonetic), studies.at(0).patient.name.components.end());
	}

	TEST_F(StudyRepositoryTest, Given_AlphabeticAndIdeographicName_When_Studies_Then_VerifyPhoneticNotReceived)
	{
		Database::Patient patient = repository.createPatient(true, false);
		Database::Study study = repository.createStudy(patient);
		auto studies = studyrepo.studies();
		ASSERT_NE(studies.at(0).patient.name.components.find(PersonName::Group::Alphabetic), studies.at(0).patient.name.components.end());
		ASSERT_NE(studies.at(0).patient.name.components.find(PersonName::Group::Ideographic), studies.at(0).patient.name.components.end());
		ASSERT_EQ(studies.at(0).patient.name.components.find(PersonName::Group::Phonetic), studies.at(0).patient.name.components.end());
	}
}