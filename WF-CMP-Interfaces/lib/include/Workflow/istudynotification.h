// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "Workflow/event.h"
#include "procedure.h"
#include "settings.h"

namespace Philips { namespace Workflow {

	//! Represents notification for study 
	class IStudyNotification
	{
	public:
		IStudyNotification() = default;
		virtual ~IStudyNotification() = default;
		
		Event<Study> studyChanged;			 //!< Notified the Procedure Changed in the Study Repository. //TICS !INT#002: Event needs to be public so that it can be subscribed

		Event<Procedure> procedureAdded;		 //!< Notifies the Procedure Addition in the StudyRepository. //TICS !INT#002: Event needs to be public so that it can be subscribed

		Event<IRepository::Model> procedureRemoved;	 //!< Notified the Procedure Removal in the Study Repository. //TICS !INT#002: Event needs to be public so that it can be subscribed

		IStudyNotification(const IStudyNotification&) = delete;
		IStudyNotification(const IStudyNotification&&) = delete;
		IStudyNotification& operator=(const IStudyNotification&) = delete;
		IStudyNotification& operator=(IStudyNotification&& source) = delete;
	};
}}
