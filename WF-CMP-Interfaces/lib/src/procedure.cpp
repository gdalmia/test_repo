// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/procedure.h"

namespace Philips { namespace Workflow
{
    Procedure::Procedure()
        : Model()
        , parent(Study())
        , id("")
        , description("")
        , startTime(DateTime())
        , endTime(DateTime())
        , state(State::Scheduled)
        , type(Type::Unknown)
        , phase(Phase::Acquisition)
        , performingPhysician(PersonName())
		, performingOperator(PersonName())
		, procedureSource(ProcedureSource::Local)
    {
    }

    Procedure::Procedure(const std::string& key)
        : Model(key)
        , parent(Study())
        , id("")
        , description("")
        , startTime(DateTime())
        , endTime(DateTime())
        , state(State::Scheduled)
        , type(Type::Unknown)
        , phase(Phase::Acquisition)
        , performingPhysician(PersonName())
		, performingOperator(PersonName())
		, procedureSource(ProcedureSource::Local)
    {
    }

    Procedure::Procedure(const Study& study)
        : Model()
        , parent(study)
        , id("")
        , description("")
        , startTime(DateTime())
        , endTime(DateTime())
        , state(State::Scheduled)
        , type(Type::Unknown)
        , phase(Phase::Acquisition)
        , performingPhysician(PersonName())
		, performingOperator(PersonName())
		, procedureSource(ProcedureSource::Local)
    {
    }

	Procedure::Type Procedure::parseType(const std::string& type)
	{
		if (type == "Coronary")
			return Type::Coronary;
		if (type == "Peripheral")
			return Type::Peripheral;
		return Type::Unknown;
	}

	std::string Procedure::toString(const Type type)
	{
		switch (type)
		{
		case Type::Coronary:
			return "Coronary";
		case Type::Peripheral:
			return "Peripheral";
		default:
			return "Unknown";
		}
	}

    bool Procedure::operator==(const Procedure& other) const
    {
        return !(*this != other);
    }

    bool Procedure::operator!=(const Procedure& other) const
    {
        return (id != other.id);
    }
}}