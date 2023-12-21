// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "datetime.h"
#include "study.h"
#include "irepository.h"

#include <string>

//TICS -INT#002: Domain model needs data member in public

namespace Philips { namespace Workflow
{
    //! Represents a Procedure in workflow
    class Procedure : public IRepository::Model
    {
    public:

		enum class State
        {
            Scheduled			= 0,
            Selected			= 1,
            InProgress			= 2,
            Suspended			= 3,
            Completed			= 4
        };

        enum class Type
        {
            Coronary    = 0,
            Peripheral  = 1,
			Unknown		= 2
        };

        enum class Phase
        {
            Acquisition = 0,
            Review      = 1
        };

		enum class ProcedureSource
		{
			Local = 0,
			Wlm = 1,
			Cwis = 2
		};


        explicit Procedure();                                   //!< Creates a Procedure
        explicit Procedure(const std::string& key);             //!< Creates a Procedure with given key
        explicit Procedure(const Study& study);                 //!< Creates a Procedure with given Study

    	//! Converts the given string to Procedure Type
    	static Type parseType(const std::string& type);

    	//! Converts the given Procedure Type to string
		static std::string toString(const Type type);

        bool operator ==(const Procedure& other) const;
        bool operator !=(const Procedure& other) const;

        Study                   parent;                                     //!< Study to which procedure belongs to.

        std::string             id;                                         //!< Identifier for the procedure
        std::string             description;                                //!< Procedure Description
        DateTime                startTime;                                  //!< Start Time of the Procedure 
        DateTime                endTime;                                    //!< End time of the Procedure 
        State                   state;                                      //!< State of the Procedure 
        Type                    type;                                       //!< Procedure Type
        Phase                   phase;                                      //!< Procedure Phase.
        PersonName              performingPhysician;                        //!< Performing physician
        PersonName              performingOperator;                         //!< Operator Name
		ProcedureSource         procedureSource;                            //!< Source of the procedure
    };
}}