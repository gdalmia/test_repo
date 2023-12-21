// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "iapplication.h"

namespace Philips { namespace Workflow { namespace Test {

	#define SSCFTS1_DEFINE_INTERFACE
	#include "iapplication.hpp"
	#undef SSCFTS1_DEFINE_INTERFACE

	#define SSCFTS1_DEFINE_PROXY
	#include "iapplication.hpp"
	#undef SSCFTS1_DEFINE_PROXY

}}}
