// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include "series.h"
#include "Workflow/ipreviewrun.h"

namespace Philips { namespace Workflow {

	//! Represents a preview source in workflow
	class IPreviewSource
	{
	public:
		IPreviewSource()          = default;
		virtual ~IPreviewSource() = default;

		//! Returns the preview run for the given series
		virtual std::unique_ptr<IPreviewRun> create(const Series& series) = 0;

		IPreviewSource(const IPreviewSource&)              = delete;
		IPreviewSource(const IPreviewSource&&)             = delete;
		IPreviewSource& operator=(const IPreviewSource&)   = delete;
		IPreviewSource& operator=(IPreviewSource&& source) = delete;

	};
	
}}