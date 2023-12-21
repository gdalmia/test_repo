// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/pictorial.h"

namespace Philips { namespace Workflow {
	
	Pictorial::Pictorial()
	: image(Sense::Image())
	, seriesNumber("")
	, title("")
	, creationTime(DateTime())
	, icon(Sense::Image())
	, attribute("")
	, tag("")
	, favorite(false)
	{
	}
	
}}