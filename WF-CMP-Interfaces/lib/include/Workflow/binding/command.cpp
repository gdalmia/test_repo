// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "command.h"

namespace Philips { namespace Workflow { namespace Binding {

	Command::Command()
	{
		canExecute =  true;
	}

	void Command::operator()() const
	{
		if (canExecute.value())
		{
			if (execute)
			{
				execute();
			}
		}
	}
}}}
