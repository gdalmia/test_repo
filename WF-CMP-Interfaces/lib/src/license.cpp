// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/license.h"
#include "ILicense/ILicense.h"
#include <Workflow/binding.h>
#include <Trace.h>

namespace Philips { namespace Workflow {

	class License::Private {

	public:
		Private(ILicenseFactory& licenseFactory)
			:_license(licenseFactory.create())
		{
		}

		bool available(const std::string& featureName) const
		{
			if (nullptr == _license) return false;

			return _license->featureAvailable(Philips::Workflow::convert(featureName).c_str());
		}

	private:
		std::unique_ptr<IPlatform::ILicense, std::function<void(IPlatform::ILicense*)>> _license;
	};

	License::License(ILicenseFactory& licenseFactory)
		: _pInstance(std::make_unique<Private>(licenseFactory))
	{		
	}

	bool License::available(const std::string& featureName) const
	{
		return _pInstance->available(featureName);
	}

	License::~License(){}	
}}