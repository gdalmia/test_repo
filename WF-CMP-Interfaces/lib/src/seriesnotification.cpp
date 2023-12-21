// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "seriesnotification.h"
#include "minitomodelconverter.h"

namespace Philips {namespace Workflow {

	SeriesNotification::SeriesNotification(Database::Series databaseSeries) :_series(databaseSeries)
	{
		_series.eventChanged = [this]()
		{
			seriesChanged(MiniToModelConverter::getSeries(_series));
		};
	}

	SeriesNotification::~SeriesNotification()
	{
	}
}}