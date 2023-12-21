// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "ireadonlystudyrepository.h"

namespace Philips { namespace Workflow {
	
	//! Represents a study repository in workflow
	class IStudyRepository: public IReadOnlyStudyRepository
	{
	public:
		IStudyRepository() = default;
		virtual ~IStudyRepository() = default;

		//! Add a new study to the repository
		virtual bool add(Study& study) = 0;

		//! Update an existing study in the repository
		virtual bool update(const Study& study) = 0;

		//! Remove a study from the repository
		virtual bool remove(const Study& study) = 0;

		//! Add a new procedure to the repository
		virtual bool add(Procedure& procedure) = 0;

		//! Update an existing proecedure in the repository
		virtual bool update(const Procedure& procedure) = 0;

		//! Remove a procedure from the repository
		virtual bool remove(const Procedure& procedure) = 0;

		//! Add a new series to the repository
		virtual bool add(Series& series) = 0;

		//! Update an existing series in the repository
		virtual bool update(const Series& series) = 0;

		//! Remove a series from the repository
		virtual bool remove(const Series& series) = 0;


		IStudyRepository(const IStudyRepository&) = delete;
		IStudyRepository(const IStudyRepository&&) = delete;
		IStudyRepository& operator=(const IStudyRepository&) = delete;
		IStudyRepository& operator=(IStudyRepository&& source) = delete;
	};
}}
