// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once 
#include <sscfts1.h>

namespace Philips { namespace Workflow { namespace Test {

	#define SSCFTS1_DECLARE_INTERFACE
	#include "iapplication.hpp"
	#undef SSCFTS1_DECLARE_INTERFACE

	#define SSCFTS1_DECLARE_PROXY
	#include "iapplication.hpp"
	#undef SSCFTS1_DECLARE_PROXY

}}}