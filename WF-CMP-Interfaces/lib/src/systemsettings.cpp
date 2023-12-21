// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "Workflow/systemsettings.h"

namespace Philips { namespace Workflow {

	class SystemSettings::Singleton
	{
	public:
		SystemSettings::Singleton()
		: _systemType(SystemType::None)
		, _integrationType(IntegrationType::None)
		{
			
		}

		static std::shared_ptr<Singleton> instance()
		{
			auto result(_instance.lock());

			if (!result)
			{
				result = std::make_shared<Singleton>();
				_instance = result;
			}

			return result;
		}

		void setSystemType(const SystemType systemType)
		{
			_systemType = systemType;
		}

		SystemType systemType() const
		{
			return _systemType;
		}

		void setIntegrationType(const IntegrationType integrationType)
		{
			_integrationType = integrationType;
		}

		IntegrationType integrationType() const
		{
			return _integrationType;
		}

		void setLicesnePluginPath(const std::filesystem::path & path)
		{
			_licensePluginPath = path;
		}

		std::filesystem::path licensePluginPath() const
		{
			return _licensePluginPath;
		}

	private:
		SystemType						_systemType;
		IntegrationType					_integrationType;
		std::filesystem::path			_licensePluginPath;
		static std::weak_ptr<Singleton>	_instance;
	};

	std::weak_ptr<SystemSettings::Singleton> SystemSettings::Singleton::_instance;

	SystemSettings::SystemSettings() : _singleton(Singleton::instance())
	{
	}

	void SystemSettings::setSystemType(const SystemType systemType) const
	{
		_singleton->setSystemType(systemType);
	}

	SystemSettings::SystemType SystemSettings::systemType() const
	{
		return _singleton->systemType();
	}

	void SystemSettings::setIntegrationType(const IntegrationType integrationType) const
	{
		_singleton->setIntegrationType(integrationType);
	}

	SystemSettings::IntegrationType SystemSettings::integrationType() const
	{
		return _singleton->integrationType();
	}

	void SystemSettings::setLicesnePluginPath(const std::filesystem::path& path) const
	{
		_singleton->setLicesnePluginPath(path);
	}

	std::filesystem::path SystemSettings::licensePluginPath() const
	{
		return _singleton->licensePluginPath();
	}
		
}}
