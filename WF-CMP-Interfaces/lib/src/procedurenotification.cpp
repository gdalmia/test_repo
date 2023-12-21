// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "procedurenotification.h"
#include "minitomodelconverter.h"

namespace Philips {namespace Workflow {

	ProcedureNotification::ProcedureNotification(Database::ProcedureStep procedure): _procedure(procedure)
	{
		_procedure.eventSeriesCreated = [&](const Database::Series& series) {seriesAdded(MiniToModelConverter::getSeries(series)); };
		_procedure.eventSeriesRemoved = [&](const std::filesystem::path &path) {seriesRemoved(IRepository::Model(path.u8string())); };
		_procedure.eventChanged = [&]() {procedureChanged(MiniToModelConverter::getProcedure(_procedure)); };
	}

	ProcedureNotification::~ProcedureNotification()
	{
	}
}}