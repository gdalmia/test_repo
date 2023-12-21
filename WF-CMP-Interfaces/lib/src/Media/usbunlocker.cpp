// Copyright 2017 Koninklijke Philips N.V.
#include "usbunlocker.h"
#include <Trace.h>
#include <iostream>
#include <string>
#include <comdef.h>

namespace Philips { namespace Workflow { namespace Media {
    class WMIServices
    {
    public:
        WMIServices():_initialized(false),_wbemlocator(NULL),_wbemservice(NULL)
        {
            HRESULT hres        = NULL;

            hres = CoInitializeEx(0, COINIT_MULTITHREADED);
            if (FAILED(hres))
            {
                return;
            }

            hres = CoCreateInstance(
                CLSID_WbemLocator,
                0,
                CLSCTX_INPROC_SERVER,
                IID_IWbemLocator,
                (LPVOID *) &_wbemlocator);
            if (FAILED( hres ))
            {
                CoUninitialize();
                return;
            }

            hres = _wbemlocator->ConnectServer(_bstr_t(L"ROOT\\CIMV2\\Security\\MicrosoftVolumeEncryption"),NULL,NULL,0,NULL,0,0,&_wbemservice);
            if (FAILED(hres))
            {
                _wbemlocator->Release();
                CoUninitialize();
                return;
            }

            hres = CoSetProxyBlanket(_wbemservice,RPC_C_AUTHN_WINNT,RPC_C_AUTHZ_NONE,NULL,RPC_C_AUTHN_LEVEL_CALL,RPC_C_IMP_LEVEL_DELEGATE,NULL,EOAC_NONE);
            if (FAILED(hres))
            {

                _wbemservice->Release();
                _wbemlocator->Release();
                CoUninitialize();
                return;
            }

			_initialized = true;
        }

        bool isInitialized() const
        {
            return _initialized;
        }

        ~WMIServices()
        {
            if(_initialized)
            {
                _wbemservice->Release();
                _wbemlocator->Release();
                CoUninitialize();
            }
        }

        IWbemServices* getWMIServices()
        {
            return _wbemservice;
        }

    private:
        bool _initialized;
        IWbemLocator *_wbemlocator;
        IWbemServices *_wbemservice;
    };

    bool UsbUnlocker::getLockStatus(const std::wstring& driveLetter)
    {
		bool isLocked = false;
        TraceScope() << "Drive: " << driveLetter;
        WMIServices wmiservices;
        VARIANT vtProp = { 0 }, vtProp1 = { 0 };
        VariantInit(&vtProp);
        VariantInit(&vtProp1);
        IWbemClassObject* pclsObj[1];
        if (getWMIService(driveLetter, wmiservices, vtProp, vtProp1, pclsObj))
        {
            std::wstring wstrQuery = L"Win32_EncryptableVolume.DeviceID='\\\\?\\";
            std::wstring tmp = vtProp.bstrVal;
            wstrQuery += tmp.substr(4) + L"\'";

            HRESULT result = wmiservices.getWMIServices()->ExecMethod(	bstr_t(wstrQuery.c_str()), bstr_t(L"GetLockStatus"),
                                                                        0,
                                                                        NULL,
                                                                        NULL,
                                                                        pclsObj,
                                                                        NULL);
            if (SUCCEEDED(result))
            {
                pclsObj[0]->Get(_bstr_t(L"LockStatus"), 0, &vtProp, 0, 0);
				isLocked = (vtProp.intVal == 1);
            }
            else
            {
                TraceError() << "UsbUnlocker::getLockStatus() query to get lock status is failed." << " Last error: " << GetLastError();
            }

            pclsObj[0]->Release();
        }
        else
        {
            TraceError() << "UsbUnlocker::getLockStatus() failed to get WMI service for drive: " << driveLetter;
        }

        VariantClear(&vtProp);
        VariantClear(&vtProp1);

        return isLocked;
    }

	bool UsbUnlocker::unLock(const std::wstring& driveLetter, const std::wstring& password)
    {
		bool isUnLocked = false;
        TraceScope() << "Drive: " << driveLetter;
        WMIServices wmiservices;
        VARIANT vtProp = { 0 }, vtProp1 = { 0 };
        VariantInit(&vtProp);
        VariantInit(&vtProp1);
        IWbemClassObject* pclsObj[1];
        if (getWMIService(driveLetter, wmiservices, vtProp, vtProp1, pclsObj))
        {
            IWbemClassObject* pInParamsDefinition = nullptr;
            IWbemClassObject* newIns = nullptr;

            if(SUCCEEDED(wmiservices.getWMIServices()->GetObject(_bstr_t(L"Win32_EncryptableVolume"), 0, NULL, &newIns, NULL)) &&
               SUCCEEDED(newIns->GetMethod(L"UnlockWithPassphrase", 0, &pInParamsDefinition,NULL)))
            {
                VARIANT var;
                VariantInit(&var);
                var.vt = VT_BSTR;
                var.bstrVal=SysAllocString(_bstr_t(password.c_str()));
                if(SUCCEEDED(pInParamsDefinition->Put(L"PassPhrase",0,&var,CIM_STRING)))
                {
					queryAndCheckLockStatus(vtProp, wmiservices, pInParamsDefinition, pclsObj, isUnLocked);
                }

                // Clean up
                pInParamsDefinition->Release();
                newIns->Release();
                VariantClear(&var);
            }
            pclsObj[0]->Release();
        }
        else
        {
            TraceError() << "UsbUnlocker::unLock() failed to get WMI service for drive: " << driveLetter;
        }

        VariantClear(&vtProp);
        VariantClear(&vtProp1);

        return isUnLocked;
    }

	void UsbUnlocker::queryAndCheckLockStatus(VARIANT &vtProp, Philips::Workflow::Media::WMIServices &wmiservices, IWbemClassObject * pInParamsDefinition, IWbemClassObject * pclsObj[1], bool &isUnLocked)
	{
		std::wstring wstrQuery = L"Win32_EncryptableVolume.DeviceID='\\\\?\\";
		std::wstring tmp = vtProp.bstrVal;
		wstrQuery += tmp.substr(4) + L"\'";

		if (SUCCEEDED(wmiservices.getWMIServices()->ExecMethod(bstr_t(wstrQuery.c_str()), bstr_t(L"UnlockWithPassphrase"),
			0, NULL, pInParamsDefinition, pclsObj, NULL)))
		{
			if (SUCCEEDED(wmiservices.getWMIServices()->ExecMethod(bstr_t(wstrQuery.c_str()), bstr_t(L"GetLockStatus"),
				0, NULL, NULL, pclsObj, NULL)))
			{
				pclsObj[0]->Get(_bstr_t(L"LockStatus"), 0, &vtProp, 0, 0);
				if (0 == vtProp.intVal)
				{
					isUnLocked = true;
				}
			}
			else
			{
				TraceError() << "UsbUnlocker::unLock() - execute to get lock status failed." << " Last error: " << GetLastError();
			}
		}
		else
		{
			TraceError() << "UsbUnlocker::unLock() - execute unlock with passphrase failed." << " Last error: " << GetLastError();
		}
	}

    bool UsbUnlocker::getWMIService(const std::wstring& driveLetter, WMIServices& wmiservices, VARIANT& vtProp, VARIANT& vtProp1, IWbemClassObject* pclsObj[1])
    {
        if (!wmiservices.isInitialized())
        {
            TraceError() << "UsbUnlocker::getWMIService() - WMI services are not initialized.";
            return false;
        }

        wchar_t query[] = L"Select * from Win32_EncryptableVolume where  DriveLetter = 'X:'";
        query[60] = driveLetter[0];

        IEnumWbemClassObject* pEnumerator = nullptr;
        if (FAILED(wmiservices.getWMIServices()->ExecQuery(bstr_t("WQL"), bstr_t(query), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator)))
        {
            TraceError() << "UsbUnlocker::getWMIService() - Execute query failed. Query: " << query << L". Last error: " << GetLastError();
            return false;
        }
		bool deviceFound = queryAndGetDeviceFoundStatus(pEnumerator, pclsObj, vtProp, vtProp1, driveLetter);
        pEnumerator->Release();

        return deviceFound;
    }

	bool UsbUnlocker::queryAndGetDeviceFoundStatus(IEnumWbemClassObject * pEnumerator, IWbemClassObject ** pclsObj, VARIANT & vtProp, VARIANT & vtProp1, const std::wstring & driveLetter)
	{
		HRESULT result = WBEM_S_NO_ERROR;
		bool isDeviceFound = false;
		while (WBEM_S_NO_ERROR == result)
		{
			ULONG uReturned;
			result = pEnumerator->Next(WBEM_INFINITE, 1, pclsObj, &uReturned);
			if (0 == uReturned)
			{
				continue; // If nothing returned
			}
			if (SUCCEEDED(pclsObj[0]->Get(_bstr_t(L"DeviceID"), 0, &vtProp, 0, 0)) &&
				SUCCEEDED(pclsObj[0]->Get(_bstr_t(L"DriveLetter"), 0, &vtProp1, 0, 0)))
			{
				std::wstring wstrQuery = L"Win32_EncryptableVolume.DeviceID='\\\\?\\";
				std::wstring tmp = vtProp.bstrVal;
				tmp = tmp.substr(4);
				wstrQuery += tmp.substr(4) + L"\'";

				if (lstrcmpW(vtProp1.bstrVal, driveLetter.c_str()) == 0)
				{
					isDeviceFound = true;
					break;
				}
			}
			// Clean up
			for (ULONG n = 0; n < uReturned; n++)
			{
				pclsObj[n]->Release();
			}
			VariantClear(&vtProp);
			VariantClear(&vtProp1);
		}
		return isDeviceFound;
	}

}}}