// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include "property.h"

namespace Philips { namespace Workflow { namespace Binding {

	//! The Command represents an action which can be invoked
	class Command
	{
		typedef std::function<void()> Execute;
		
	public:
		//! Create a command
		Command();

		//! Executes the command
		void operator()() const;

		Execute execute;					//!< Defines the method to be called when the command is invoked.
		Property<bool> canExecute;			//!< Defines a property that determines whether the command can execute.

		~Command() = default;
		Command(const Command&) = delete;
		Command(Command&&) = delete;
		Command& operator=(const Command&) = delete;
		Command& operator=(Command&&) = delete;
	};
}}}
