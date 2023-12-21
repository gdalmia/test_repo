// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "workflow.h"
#include <database/database.h>


namespace Philips {	namespace Workflow {

	class ProcedureNotification : public IProcedureNotification
	{
	public:
		explicit ProcedureNotification(Database::ProcedureStep databaseProcedure);
		~ProcedureNotification();
	private:
		Database::ProcedureStep _procedure;
	};
}}
