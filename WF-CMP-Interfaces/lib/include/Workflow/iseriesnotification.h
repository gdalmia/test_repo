// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "Workflow/event.h"
#include "Workflow/series.h"

namespace Philips {	namespace Workflow {

	//! Represents a series notification in workflow
	class ISeriesNotification
	{
	public:
		ISeriesNotification() = default;
		virtual ~ISeriesNotification() = default;

		///! Series changed event
		Event<Series> seriesChanged;			//!< Notified the Series Changed in the Study Repository. //TICS !INT#002: Event needs to be public so that it can be subscribed

		ISeriesNotification(const ISeriesNotification&) = delete;
		ISeriesNotification(const ISeriesNotification&&) = delete;
		ISeriesNotification& operator=(const ISeriesNotification&) = delete;
		ISeriesNotification& operator=(ISeriesNotification&& source) = delete;
	};
}}
