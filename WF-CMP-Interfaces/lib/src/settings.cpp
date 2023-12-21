// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "Workflow/settings.h"
#include <string>

namespace Philips { namespace Workflow {

	class Settings::Singleton
	{
		static constexpr int	 maximumLength = 256;

	public:
		Settings::Singleton(
			const std::filesystem::path& source
			, bool test
			, const std::string dateTimeFormat = "dd/MM/yyyy"
			, const HeightUnit heightUnit = HeightUnit::Cm
			, const WeightUnit weightUnit = WeightUnit::Kilogram
			, const std::string dateFormat = "dd/MM/yyyy"
			, const std::string timeFormat = "hh:mm:ss tt"
			, const bool showIdeographic = false
			)
			: _path(source)
			, _test(test)
			, _dateTimeFormat(dateTimeFormat)
			, _dateFormat(dateFormat)
			, _timeFormat(timeFormat)
			, _heightUnit(heightUnit)
			, _weightUnit(weightUnit)
			, _language(Language::US)
			, _showIdeographic(showIdeographic)
		{
			if (!exists(_path))
			{
				create_directories(_path);
			}
		}
		
		static std::shared_ptr<Singleton> instance(
			  std::filesystem::path source = std::filesystem::path()
			, bool test = false)
		{
			auto result(_instance.lock());

			if (!result)
			{
				if (source.empty())
				{
					char buffer[maximumLength] = { '\0' };
					size_t length = 0;
					if (0 != getenv_s(&length, buffer, maximumLength, "ProgramData"))
					{
						throw std::runtime_error("Program Data path cannot be determined.");
					}
					source = std::experimental::filesystem::path(buffer) / "Philips";
				}

				result = std::make_shared<Singleton>(source / "DataHandler", test);
				_instance = result;
			}

			return result;
		}

		const std::filesystem::path& path() const
		{
			return _path;
		}

		bool test() const
		{
			return _test;
		}
		
		const std::string& dateTimeFormat() const
		{
			return _dateTimeFormat;
		}
		
		void setDateTimeFormat(const std::string& format)
		{
			_dateTimeFormat = format;
		}

		const std::string& dateFormat() const
		{
			return _dateFormat;
		}

		void setDateFormat(const std::string& format)
		{
			_dateFormat = format;
		}

		const std::string& timeFormat() const
		{
			return _timeFormat;
		}

		void setTimeFormat(const std::string& format)
		{
			_timeFormat = format;
		}

		
		void setHeightUnit(const HeightUnit& unit)
		{
			_heightUnit = unit;
		}

		HeightUnit getHeightUnit() const
		{
			return _heightUnit;
		}
		
		void setWeightUnit(const WeightUnit& unit)
		{
			_weightUnit = unit;
		}

		WeightUnit getWeightUnit() const
		{
			return _weightUnit;
		}

		void setLanguage(const Language& language)
		{
			_language = language;
		}

		Settings::Language language() const
		{
			return _language;
		}

		void setShowIdeographic( const bool showIdeographic)
		{
			_showIdeographic = showIdeographic;
		}

		bool showIdeographic() const
		{
			return _showIdeographic;
		}
	private:
		std::filesystem::path           _path          ;
		bool                            _test          ;
		std::string                     _dateTimeFormat;
		std::string                     _dateFormat    ;
		std::string                     _timeFormat    ;
		HeightUnit	                    _heightUnit    ;
		WeightUnit	                    _weightUnit    ;
		Language                        _language      ;
		static std::weak_ptr<Singleton> _instance      ;
		bool							_showIdeographic;
	};

	std::weak_ptr<Settings::Singleton> Settings::Singleton::_instance;

	Settings::Settings() : _singleton(Singleton::instance())
	{
	}

	Settings::Settings(const std::filesystem::path& source, bool test)
		: _singleton(Singleton::instance(source, test))
	{
	}

	const std::experimental::filesystem::path& Settings::path() const
	{
		return _singleton->path();
	}

	bool Settings::test() const
	{
		return _singleton->test();
	}

	void Settings::setDateTimeFormat(const std::string& format) const
	{ 
		_singleton->setDateTimeFormat(format);
	}

	const std::string& Settings::dateTimeFormat() const
	{
		return _singleton->dateTimeFormat();
	}

	void Settings::setDateFormat(const std::string& format) const
	{
		_singleton->setDateFormat(format);
	}

	const std::string& Settings::dateFormat() const
	{
		return _singleton->dateFormat();
	}

	void Settings::setTimeFormat(const std::string& format) const
	{
		_singleton->setTimeFormat(format);
	}

	const std::string& Settings::timeFormat() const
	{
		return _singleton->timeFormat();
	}

	void Settings::setHeightUnit(const HeightUnit & unit)
	{
		_singleton->setHeightUnit(unit);
	}

	Settings::HeightUnit Settings::getHeightUnit() const
	{
		return _singleton->getHeightUnit();
	}

	void Settings::setWeightUnit(const WeightUnit & unit)
	{
		_singleton->setWeightUnit(unit);
	}

	Settings::WeightUnit Settings::getWeightUnit() const
	{
		return _singleton->getWeightUnit();
	}

	void Settings::setLanguage(const Language& language) const
	{
		_singleton->setLanguage(language);
	}

	Settings::Language Settings::language() const
	{
		return _singleton->language();
	}

	void Settings::setShowIdeographic(const bool showIdeographic) const
	{
		_singleton->setShowIdeographic(showIdeographic);
	}

	bool  Settings::showIdeographic() const
	{
		return _singleton->showIdeographic();
	}
}}
