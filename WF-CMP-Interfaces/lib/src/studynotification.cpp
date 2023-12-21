// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "studynotification.h"
#include "minitomodelconverter.h"

namespace Philips {namespace Workflow {

	StudyNotification::StudyNotification(Database::Study study):_study(study)
	{
		_study.eventProcedureStepCreated = [&](const Database::ProcedureStep& procedureStep) {procedureAdded(MiniToModelConverter::getProcedure(procedureStep)); };
		_study.eventProcedureStepRemoved = [&](const std::filesystem::path &path) {procedureRemoved(IRepository::Model(path.u8string())); };
		_study.eventChanged = [&]() {studyChanged(MiniToModelConverter::getStudy(_study)); };
		
	}

	StudyNotification::~StudyNotification()
	{
	}
}}
