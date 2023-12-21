// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/series.h"

namespace Philips { namespace Workflow
{
    Series::Series()
        : Model()
        , parent(Procedure())
        , description("")
        , creationTime(DateTime())
        , acquisitionTime(DateTime())
        , number(0)
        , status(ArchiveStatus::NotSubmitted)
        , modality(Modality::XA)
        , instanceUid("")
        , clinicalDataType("")
		, clinicalDataTypeVariant("")
		, flagged(false)
		, importStatus(ImportStatus::None)
    {
    }

    Series::Series(const std::string& key)
        : Model(key)
        , parent(Procedure())
        , description("")
        , creationTime(DateTime())
        , acquisitionTime(DateTime())
        , number(0)
        , status(ArchiveStatus::NotSubmitted)
        , modality(Modality::XA)
        , instanceUid("")
        , clinicalDataType("")
		, clinicalDataTypeVariant("")
		, flagged(false)
		, importStatus(ImportStatus::None)
    {
    }

    Series::Series(const Procedure& procedure)
        : Model()
        , parent(procedure)
        , description("")
        , creationTime(DateTime())
        , acquisitionTime(DateTime())
        , number(0)
        , status(ArchiveStatus::NotSubmitted)
        , modality(Modality::XA)
        , instanceUid("")
        , clinicalDataType("")
		, clinicalDataTypeVariant("")
		, flagged(false)
		, importStatus(ImportStatus::None)
    {
    }

    bool Series::operator==(const Series& other) const
    {
        return !(*this != other);
    }

    bool Series::operator!=(const Series& other) const
    {
        return instanceUid != other.instanceUid;
    }
}}