// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "workflow.h"

using namespace Philips::Workflow;
namespace Philips{namespace Workflow{namespace Test
{
	Study newStudy();
	Procedure newProcedure(Procedure::Type type);
	Series newSeries();
}}}