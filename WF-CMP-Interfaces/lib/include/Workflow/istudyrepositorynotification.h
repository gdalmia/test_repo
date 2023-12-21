// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "irepository.h"
#include "iprocedurenotification.h"
#include "iseriesnotification.h"
#include "istudynotification.h"
#include "study.h"
#include "procedure.h"
#include "Workflow/event.h"

namespace Philips { namespace Workflow {
	
	//! Represents notification for study repository 
	class IStudyRepositoryNotification
	{
	public:
		IStudyRepositoryNotification() = default;
		virtual ~IStudyRepositoryNotification() = default;
		
		Event<Study> studyAdded;     //!< Notifies the Study Addition in the StudyRepository. //TICS !INT#002: Event needs to be public so that it can be subscribed
		
		Event<IRepository::Model> studyRemoved;   //!< Notified the Study Removal in the Study Repository. //TICS !INT#002: Event needs to be public so that it can be subscribed

		virtual std::unique_ptr<IStudyNotification> subscribe(const Study& study) = 0 ;
		virtual std::unique_ptr<IProcedureNotification> subscribe(const Procedure& procedure) = 0;
		virtual std::unique_ptr<ISeriesNotification> subscribe(const Series& series) = 0;

		IStudyRepositoryNotification(const IStudyRepositoryNotification&) = delete;
		IStudyRepositoryNotification(const IStudyRepositoryNotification&&) = delete;
		IStudyRepositoryNotification& operator=(const IStudyRepositoryNotification&) = delete;
		IStudyRepositoryNotification& operator=(IStudyRepositoryNotification&& source) = delete;
	};
}}