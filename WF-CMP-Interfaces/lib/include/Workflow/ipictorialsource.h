// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include "pictorial.h"
#include "series.h"

namespace Philips { namespace Workflow {

	//! Represents a pictorial source in workflow
	class IPictorialSource
	{
	public:
		IPictorialSource() = default;
		virtual ~IPictorialSource() = default;

		//! Returns the pictorial for the given series
		virtual std::unique_ptr<Pictorial> pictorial(const Series& series) = 0;

		IPictorialSource(const IPictorialSource&) = delete;
		IPictorialSource(const IPictorialSource&&) = delete;
		IPictorialSource& operator=(const IPictorialSource&) = delete;
		IPictorialSource& operator=(IPictorialSource&& source) = delete;

	};
	
}}