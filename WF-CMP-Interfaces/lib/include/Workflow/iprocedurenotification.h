// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "Workflow/event.h"
#include "series.h"

namespace Philips { namespace Workflow {
	
	class IProcedureNotification
	{
	public:

		IProcedureNotification() = default;
		virtual ~IProcedureNotification() = default;
		Event<Procedure> procedureChanged;			//!< Notified the Series Changed in the Study Repository. //TICS !INT#002: Event needs to be public so that it can be subscribed

		Event<Series> seriesAdded;		//!< Notified the Series Addition in the Study Repository. //TICS !INT#002: Event needs to be public so that it can be subscribed

		Event<IRepository::Model> seriesRemoved;      //!< Notified the Series Removal in the Study Repository. //TICS !INT#002: Event needs to be public so that it can be subscribed

		IProcedureNotification(const IProcedureNotification&) = delete;
		IProcedureNotification(const IProcedureNotification&&) = delete;
		IProcedureNotification& operator=(const IProcedureNotification&) = delete;
		IProcedureNotification& operator=(IProcedureNotification&& source) = delete;
	};
}}
