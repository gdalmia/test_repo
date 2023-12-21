// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "gmock/gmock.h"
#include "Workflow/ilicensefactory.h"
#include "platformlicense.mock.h"

using namespace Philips::Workflow;

class ILicenseFactoryMock : public ILicenseFactory
{
public:
	ILicenseFactoryMock(PlatformLicenseMock* license, bool value)
	{
		if (value)
		{
			platformLicense = std::unique_ptr<IPlatform::ILicense, std::function<void(IPlatform::ILicense*)>>(license, [this](IPlatform::ILicense* ) { });
		}
		else
		{
			platformLicense = nullptr;
		}
	}
	~ILicenseFactoryMock() = default;

	std::unique_ptr<IPlatform::ILicense, std::function<void(IPlatform::ILicense*)>> create() override
	{
		return std::move(platformLicense);
	}

private:
	std::unique_ptr<IPlatform::ILicense, std::function<void(IPlatform::ILicense*)>> platformLicense;
};

typedef testing::NiceMock<ILicenseFactoryMock> LicenseFactoryMock;