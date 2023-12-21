// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "Workflow/iseriesnotification.h"
#include <database/database.h>

namespace Philips {	namespace Workflow {

	class SeriesNotification : public ISeriesNotification
	{
	public:
		explicit SeriesNotification(Database::Series databaseSeries);
		~SeriesNotification();
	private:
		Database::Series _series;
	};
}}
