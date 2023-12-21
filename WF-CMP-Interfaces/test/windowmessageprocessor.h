// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include <functional>

namespace Philips { namespace Workflow  {

	namespace WindowMessageProcessor
	{
		bool processMessageWhile(std::function<bool()> condition, int timeoutInMs);
	};

}}