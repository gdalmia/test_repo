// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

namespace Philips { namespace Workflow { namespace Binding {

	//! \ingroup Binding
	//! Indicates the direction of the data flow in the binding.
	enum class Direction
	{
		OneWay = 0			//!< Updates the only target when source changes. Not the other way.
	,	TwoWay = 1			//!< Causes changes to either the source or the target to automatically update the other.
	};
	
}}}
