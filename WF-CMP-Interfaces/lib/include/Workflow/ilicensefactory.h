// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include <memory>
#include <functional>

namespace IPlatform { class ILicense; }
namespace Philips { namespace Workflow {

	class ILicenseFactory
	{
	public:
		ILicenseFactory()							 = default;
		virtual ~ILicenseFactory()					 = default;

		//! Creates license from the dll
		virtual std::unique_ptr<IPlatform::ILicense, std::function<void(IPlatform::ILicense*)>> create() = 0;
	};
}}