// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/study.h"

namespace Philips { namespace Workflow {
	
	Study::Study()
		: Model()
		, id("")
		, dateTime(DateTime())
		, description("")
		, weight(0.0)
		, height(0.0)
		, alerts({})
		, allergies({})
		, accessionNumber("")
		, requestingService("")
		, comments({})
		, importStatus(ImportStatus::None)
	{
	}

	Study::Study(const std::string& key)
		: Model(key)
		, id("")
		, dateTime(DateTime())
		, description("")
		, weight(0.0)
		, height(0.0)
		, alerts({})
		, allergies({})
		, accessionNumber("")
		, requestingService("")
		, comments({})
		, importStatus(ImportStatus::None)
	{
	}

	bool Study::operator==(const Study& other) const
	{
		return !(*this != other);
	}

	bool Study::operator!=(const Study& other) const
	{
		if (this->importStatus != Study::ImportStatus::None || other.importStatus != Study::ImportStatus::None) return this->id != other.id;
		return (!instanceUid.empty() || !other.instanceUid.empty())
				? (instanceUid != other.instanceUid)
				: (id != other.id);
	}
	
}}