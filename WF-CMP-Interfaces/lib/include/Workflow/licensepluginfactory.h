// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "ilicensefactory.h"
#include <memory>
#include <filesystem>
#include <functional>

namespace std { namespace filesystem = experimental::filesystem; }

namespace Philips { namespace Workflow {

	class LicensePluginFactory : public ILicenseFactory
	{
	public:
		LicensePluginFactory();
		~LicensePluginFactory();

		//! Creates license from the dll
		std::unique_ptr<IPlatform::ILicense, std::function<void(IPlatform::ILicense*)>> create() override;

	private:
		class Private;
		std::unique_ptr<Private> pInstance;
	};
}}