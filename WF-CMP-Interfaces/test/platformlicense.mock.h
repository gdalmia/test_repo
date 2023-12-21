// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "gmock/gmock.h"
#include "ILicense/ILicense.h"

using namespace IPlatform;
//TICS -INT#002 -CON#007
//INT#002 - MOCK methods need to be public
//CON#007 - false TICS warning for bool being implicitly converted for featureAvailable mock method
class PlatformILicenseMock : public IPlatform::ILicense
{
public:
	PlatformILicenseMock()
	{
	}
	~PlatformILicenseMock() = default;
	MOCK_METHOD(bool, featureAvailable, (const wchar_t*), (override));
};

typedef testing::NiceMock<PlatformILicenseMock> PlatformLicenseMock;
//TICS +INT#002 +CON#007