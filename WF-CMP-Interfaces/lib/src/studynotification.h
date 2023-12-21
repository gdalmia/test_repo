// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "workflow.h"
#include <database/database.h>

namespace Philips { namespace Workflow {

	class StudyNotification : public IStudyNotification
	{
	public:
		explicit StudyNotification(Database::Study);
		~StudyNotification();
	private:
		Database::Study _study;	
	};
}}
