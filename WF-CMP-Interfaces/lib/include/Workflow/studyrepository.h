// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "ireadonlystudyrepository.h"
#include "istudyrepositorynotification.h"

namespace Philips { namespace Workflow {

    //! Represents a localRepository in workflow
    class StudyRepository : public IStudyRepository, public IStudyRepositoryNotification
    {
    public:
        StudyRepository();
        ~StudyRepository();

        //! Get  studies from the repository
         std::vector<Study> studies() override;

        //! Get  procedures from the repository
         std::vector<Procedure> procedures(const Study& study) override;

        //! Get  series from the repository
         std::vector<Series> series(const Procedure& procedure) override;

    	//! Get series from the repository corresponding to the given study
		 std::vector<Series> series(const Study& study) override;

         //! Is Study Protected
         bool isProtected(const Study& study) override;

		 //! Add a new study to the repository
		 //! \remarks the study is updated with the database key
		 bool add(Study& study) override;

		 //! Update an existing study in the repository
		 bool update(const Study& study) override;

		 //! Remove a study from the repository
		 bool remove(const Study& study) override;

		 //! Add a new procedure to the repository
		 //! \remarks the procedure is updated with the database key
		 bool add(Procedure& procedure) override;

		 //! Update an existing proecedure in the repository
		 bool update(const Procedure& procedure) override;

		 //! Remove a procedure from the repository
		 bool remove(const Procedure& procedure) override;

		 //! Add a new series to the repository
		 //! \remarks the series is updated with the database key
		 bool add(Series& series) override;

		 //! Update an existing series in the repository
		 bool update(const Series& series) override;

		 //! Remove a series from the repository
		 bool remove(const Series& series) override;

    	//! Subscribes series events
		 std::unique_ptr<IProcedureNotification> subscribe(const Procedure&) override;

		 //! Subscribes procedure events
		 std::unique_ptr<IStudyNotification> subscribe(const Study&) override;

		 //! Subscribes series events
		 std::unique_ptr<ISeriesNotification> subscribe(const Series&) override;

    private:
        struct Private;
        std::unique_ptr<Private> _private;
    };
}}
