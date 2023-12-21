// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "irepository.h"
#include "study.h"
#include "procedure.h"
#include "series.h"
#include "Workflow/event.h"
#include <vector>

namespace Philips { namespace Workflow {
	
	//! Represents a read only patient repository 
	class IReadOnlyStudyRepository: protected IRepository
	{
	public:
		IReadOnlyStudyRepository() = default;
		virtual ~IReadOnlyStudyRepository() = default;

		//! Get all studies from the repository
		virtual std::vector<Study> studies() = 0;

        //! Get all procedures from the repository
		virtual std::vector<Procedure> procedures(const Study& study) = 0;

        //! Get all series from the repository
		virtual std::vector<Series> series(const Procedure& procedure) = 0;

		//! Get all series from the repository corresponding to the given study
		virtual std::vector<Series> series(const Study& study) = 0;

        //! Checks is Study Protected
        virtual bool isProtected(const Study& study) = 0;

		IReadOnlyStudyRepository(const IReadOnlyStudyRepository&) = delete;
		IReadOnlyStudyRepository(const IReadOnlyStudyRepository&&) = delete;
		IReadOnlyStudyRepository& operator=(const IReadOnlyStudyRepository&) = delete;
		IReadOnlyStudyRepository& operator=(IReadOnlyStudyRepository&& source) = delete;
	};

}}
