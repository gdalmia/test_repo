// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "physician.h"
namespace Philips {namespace Workflow {

	Physician::PersonName::Component::Component() : Serializable("Component")
	{
	}

	Physician::Physician() : Serializable("Physician")
	{

	}

	Physician::PersonName::PersonName() : Serializable("PersonName")
	{
	}

}}