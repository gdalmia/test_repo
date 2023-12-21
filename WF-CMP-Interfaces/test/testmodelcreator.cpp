// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "testmodelcreator.h"
	
Study Philips::Workflow::Test::newStudy()
{
	Study study("Key");
	study.id = "ID";
	study.accessionNumber = "Acc no";
	study.alerts.push_back("Alert");
	study.allergies.push_back("Allergy");
	study.dateTime = DateTime::now();
	study.description = "Desc";
	study.height = 100;
	study.instanceUid = "UID";
	study.patient.id = "Patient ID";
	study.patient.birthDate = DateTime::now();
	study.patient.gender = Patient::Gender::Male;
	PersonName name;
	name.components.insert({ PersonName::Group::Alphabetic, PersonName::Component{"family",
		"Name", "Middle", "Prefix", "Suffix"} });
	study.patient.name = name;
	study.comments = { "Under observation" };
	study.weight = 100;
	study.requestingService = "Service";
	Philips::Workflow::PersonName pName;
	pName.components.insert({ PersonName::Group::Alphabetic, PersonName::Component{"pfamily",
		"pName", "pMiddle", "pPrefix", "pSuffix"} });
	study.referringPhysician = pName;
	return study;
}

Procedure Philips::Workflow::Test::newProcedure(Procedure::Type type)
{
	Procedure procedure;
	procedure.description = "Desc";
	procedure.endTime = DateTime::now();
	procedure.type = type;
	PersonName pName;
	pName.components.insert({ PersonName::Group::Alphabetic, PersonName::Component{"pfamily",
		"pName", "pMiddle", "pPrefix", "pSuffix"} });
	procedure.performingPhysician = pName;
	PersonName oName;
	oName.components.insert({ PersonName::Group::Alphabetic, PersonName::Component{"ofamily",
		"oName", "oMiddle", "oPrefix", "oSuffix"} });
	procedure.performingOperator = oName;
	procedure.phase = Procedure::Phase::Acquisition;
	procedure.startTime = DateTime::now();
	procedure.state = Procedure::State::Completed;

	return procedure;
}

Series Philips::Workflow::Test::newSeries()
{
	Series series;
	series.modality = Series::Modality::XA;
	series.acquisitionTime = DateTime::now();
	series.clinicalDataType = "XA";
	series.clinicalDataType = "TestVariant";
	series.creationTime = DateTime::now();
	series.description = "Series description";
	series.number = 1;
	series.status = Series::ArchiveStatus::InProgress;
	return  series;
}

