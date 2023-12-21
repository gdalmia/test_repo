// Copyright 2017 Koninklijke Philips N.V.
#pragma once
#include <string>
#include <Wbemidl.h>


namespace Philips { namespace Workflow { namespace Media {
    class WMIServices;
	class UsbUnlocker
	{
	public:
		static bool getLockStatus(const std::wstring& driveLetter);
		static bool unLock(const std::wstring& driveLetter, const std::wstring& password);

	private:
		static bool getWMIService(const std::wstring& driveLetter, WMIServices& wmiservices, VARIANT& vtProp, VARIANT& vtProp1, IWbemClassObject* pclsObj[1]);
		static void queryAndCheckLockStatus(VARIANT & vtProp, Philips::Workflow::Media::WMIServices & wmiservices, IWbemClassObject * pInParamsDefinition, IWbemClassObject * pclsObj[1], bool & isUnLocked);
		static bool queryAndGetDeviceFoundStatus(IEnumWbemClassObject * pEnumerator, IWbemClassObject ** pclsObj, VARIANT & vtProp, VARIANT & vtProp1, const std::wstring & driveLetter);
	};
}}}