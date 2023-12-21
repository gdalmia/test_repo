// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "workflow.h"

using namespace Philips::Workflow;

namespace Philips{namespace Workflow{namespace Test
{
	bool verifyPersonInfo(const PersonName& actual, const PersonName& expected);
	bool verifyCollection(const std::vector<std::string> &expected, const std::vector<std::string> &actual);
	bool verifyStudyInfo(Study& actual, Study& expected);
	bool verifyProcedureInfo(Procedure& actual, Procedure& expected);
	bool verifySeriesInfo(Series& actual, Series& expected);
	bool verifyAlphabeticPersonInfo(const std::pair<PersonName::Group, PersonName::Component>&  actual, const std::pair<PersonName::Group, PersonName::Component>& expected);
}}}