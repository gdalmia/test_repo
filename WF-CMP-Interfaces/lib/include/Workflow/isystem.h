// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include <workflow.h>

namespace Philips { namespace Workflow {

	//! Represents the System in workflow
	class ISystem
	{
	public:
		ISystem() = default;
		virtual ~ISystem() = default;

		//! Returns whether the system is in demo mode or not. 
		virtual bool demoMode() const = 0;

		//! Event is raised when demo mode is changed.
		Workflow::Event<>  demoModeChanged;

		ISystem(const ISystem&) = delete;
		ISystem(const ISystem&&) = delete;
		ISystem& operator=(const ISystem&) = delete;
		ISystem& operator=(ISystem&& source) = delete;
	};
}}