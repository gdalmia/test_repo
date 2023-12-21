// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include "ilicense.h"
#include "ilicensefactory.h"
#include <filesystem>

namespace std { namespace filesystem = experimental::filesystem; }

namespace Philips { namespace Workflow {

	class License : public ILicense
	{
	public:
		explicit License(ILicenseFactory& );
		~License() ;

		//! Gets feature available status
		bool available(const std::string& featureName) const override;

	private:
		class Private;
		std::unique_ptr<Private> _pInstance;
	};
}}