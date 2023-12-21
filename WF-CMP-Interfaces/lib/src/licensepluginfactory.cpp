// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/licensepluginfactory.h"
#include "Workflow/settings.h"
#include "ILicense/ILicense.h"
#include "Workflow/systemsettings.h"
#include <Trace.h>
#include <Log.h>
#include <Windows.h>

namespace Philips { namespace Workflow {

	const Log::Event LicenceNotAvailable = Log::Event(1, "DHW", Log::Category::ServiceAppError, L"License component not available");

	class LicensePluginFactory::Private
	{
	public:

		Private()
			: _handle(nullptr)
			, _createLicense(nullptr)
			, _destroyLicense(nullptr)
			, _exists(false)
		{
			const SystemSettings settings;
			auto licensePath = settings.licensePluginPath();
			_exists = std::filesystem::exists(licensePath);

			if (!_exists)
			{
				TraceWarning() << "License plugin do not exist in the given path-> " << licensePath;
				return;
			}

			loadLibrary(licensePath);
		}

		void loadLibrary(const std::filesystem::path& path)
		{
			std::string error;
			_handle = ::LoadLibrary((path).wstring().c_str());
			if (_handle == nullptr)
			{
				error = "Failed to load License plugin. Error-> " + GetLastError();
				TraceError() << error;
				LogMessage(LicenceNotAvailable);
				throw std::runtime_error(error);
			}

			_createLicense = reinterpret_cast<_licFac>(::GetProcAddress(_handle, "createLicense"));
			if (_createLicense == nullptr)
			{
				error = "Not able to get the address for the createLicense exported function from license plugin. Error-> " + GetLastError();
				TraceError() << error;
				throw std::runtime_error(error);
			}

			_destroyLicense = reinterpret_cast<_destroyLic>(::GetProcAddress(_handle, "destroyLicense"));
			if (_destroyLicense == nullptr)
			{
				error = "Not able to get the address for the destroyLicense exported functions from license plugin. Error-> " + GetLastError();
				TraceError() << error;
				throw std::runtime_error(error);
			}
		}

		std::unique_ptr<IPlatform::ILicense, std::function<void(IPlatform::ILicense*)>> create()
		{
			TraceScope();

			if (!_exists) return nullptr;

			return std::unique_ptr<IPlatform::ILicense, std::function<void(IPlatform::ILicense*)>>(_createLicense(), [this]
			(IPlatform::ILicense* license)
			{
				_destroyLicense(license);
			});
		}

		~Private()
		{
			if (nullptr != _handle)
			{
				::FreeLibrary(_handle);
			}
		}

	private:
		HINSTANCE _handle;
		typedef IPlatform::ILicense*(__cdecl *_licFac)();
		_licFac _createLicense;
		typedef void(__cdecl *_destroyLic)(IPlatform::ILicense*);
		_destroyLic _destroyLicense;
		bool _exists;
	};


	LicensePluginFactory::LicensePluginFactory()
		:	pInstance(std::make_unique<Private>())
	{
	}

	 std::unique_ptr<IPlatform::ILicense, std::function<void(IPlatform::ILicense*)>> LicensePluginFactory::create()
	{
		 return pInstance->create();
	}

	LicensePluginFactory::~LicensePluginFactory()
	{
	}
}}