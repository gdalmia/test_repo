// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "datetime.h"
#include "procedure.h"
#include "irepository.h"

#include <string>

//TICS -INT#002: Domain model needs data member in public

namespace Philips { namespace Workflow
{
    //! Represents a Series in workflow
    class Series : public IRepository::Model
    {
    public:
        enum class ArchiveStatus
        { 
			NotSubmitted			= 0,
            NotStarted				= 1, 
            InProgress				= 2, 
            CompletedSuccessfully	= 3,
            CompletedWithError      = 4
        };

		enum class ImportStatus
		{
			None					= 0,
			NotStarted				= 1,
			InProgress				= 2,
			Completed				= 3,
			Failed					= 4
		};

        enum class Modality
        {
            IVUS    = 0,
            XA      = 1,
            US      = 2,
            MR      = 3,
            CT      = 4
        };

        explicit Series();                              //!< Creates a Series
        explicit Series(const std::string& key);        //!< Creates a Series with given key
        explicit Series(const Procedure& procedure);    //!< Creates a Series with given Procedure

        bool operator ==(const Series& other) const;
        bool operator !=(const Series& other) const;

        Procedure           parent;                     //!< Procedure to which study belongs to.

        std::string         description;                //!< Series Description
        DateTime            creationTime;               //!< Creation time of the series 
        DateTime            acquisitionTime;            //!< AcquisitionTime of the Series 
        int32_t             number;                     //!< Sequence Number of the Series
        ArchiveStatus       status;                     //!< Archiving State of the Series
        Modality            modality;                   //!< Represents the modality.
        std::string         instanceUid;                //!< Series Instance UID.
        std::string         clinicalDataType;           //!< Clinical Data Type.
		std::string         clinicalDataTypeVariant;    //!< Clinical Data Type Variant.
		bool				flagged;					//!< Marked for auto archival
		ImportStatus        importStatus;				//!< Import status of the series
	};
}}