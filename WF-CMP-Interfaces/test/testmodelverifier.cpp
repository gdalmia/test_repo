// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "testmodelverifier.h"
bool Philips::Workflow::Test::verifyPersonInfo(const PersonName& actual, const PersonName& expected)
{
	for (auto const& iterator : expected.components)
	{
		for (auto const& iteratorexpected : actual.components)
		{
			verifyAlphabeticPersonInfo(iteratorexpected, iterator);
		}
	}
	return true;
}

bool Philips::Workflow::Test::verifyAlphabeticPersonInfo(const std::pair<PersonName::Group, PersonName::Component>&  actual, const std::pair<PersonName::Group, PersonName::Component>& expected)
{
	if ((actual.first == PersonName::Group::Alphabetic) && (expected.first == PersonName::Group::Alphabetic))
	{
		if (expected.second.givenName != actual.second.givenName)   return false;
		if (expected.second.familyName != actual.second.familyName) return false;
		if (expected.second.middleName != actual.second.middleName) return false;
		if (expected.second.namePrefix != actual.second.namePrefix) return false;
		if (expected.second.nameSuffix != actual.second.nameSuffix) return false;
	}
	return true;
}

bool Philips::Workflow::Test::verifyCollection(const std::vector<std::string> &expected, const std::vector<std::string> &actual)
{
	std::vector<std::string> vectorCompare;
	for (auto const& item : actual)
		vectorCompare.push_back(item);
	return expected == vectorCompare;
}

bool Philips::Workflow::Test::verifyStudyInfo(Study& actual, Study& expected)
{
	if (actual.height != expected.height) return false;
	if (actual.weight != expected.weight) return false;
	if (actual.description != expected.description) return false;
	if (actual.instanceUid != expected.instanceUid) return false;
	if (actual.accessionNumber != expected.accessionNumber) return false;
	if (actual.id != expected.id) return false;
	if (actual.requestingService != expected.requestingService) return false;
	if (!verifyCollection(actual.alerts, expected.alerts)) return false;
	if (!verifyCollection(actual.allergies, expected.allergies)) return false;
	if (!verifyCollection(actual.comments, expected.comments)) return false;
	if (actual.dateTime.toString("yyyy/mm/dd hh:mm:ss", true) != expected.dateTime.toString("yyyy/mm/dd hh:mm:ss", true)) return false;
	if (!verifyPersonInfo(actual.referringPhysician, expected.referringPhysician)) return false;
	return true;
}

bool  Philips::Workflow::Test::verifyProcedureInfo(Procedure& actual, Procedure& expected)
{
	if (actual.id != expected.id)          return false;
	if (actual.description != expected.description)      return false;
	if (actual.startTime.toString("yyyy/mm/dd hh:mm:ss", true) != expected.startTime.toString("yyyy/mm/dd hh:mm:ss", true))         return false;
	if (actual.endTime.toString("yyyy/mm/dd hh:mm:ss", true) != expected.endTime.toString("yyyy/mm/dd hh:mm:ss", true))             return false;
	if (actual.state != expected.state)                   return false;
	if (actual.phase != expected.phase)                 return false;
	if (actual.type != expected.type)                            return false;

	verifyPersonInfo(actual.performingPhysician, expected.performingPhysician);
	verifyPersonInfo(actual.performingOperator, expected.performingOperator);
	// Verify parent details are filled properly.

	if (!verifyStudyInfo(actual.parent, expected.parent))    return false;
	return true;
}

bool Philips::Workflow::Test::verifySeriesInfo(Series& actual, Series& expected)
{
	return (actual.number == expected.number)
		&& (actual.description == expected.description)
		&& (actual.instanceUid == expected.instanceUid)
		&& (actual.modality == expected.modality)
		&& (actual.clinicalDataType == expected.clinicalDataType)
		&& (actual.clinicalDataTypeVariant == expected.clinicalDataTypeVariant)
		&& (actual.status == expected.status)
		&& (actual.acquisitionTime.toString("yyyy/mm/dd hh:mm:ss", true) == expected.acquisitionTime.toString("yyyy/mm/dd hh:mm:ss", true))
		&& (actual.creationTime.toString("yyyy/mm/dd hh:mm:ss", true) == expected.creationTime.toString("yyyy/mm/dd hh:mm:ss", true))
		&& (verifyProcedureInfo(actual.parent, expected.parent));
}