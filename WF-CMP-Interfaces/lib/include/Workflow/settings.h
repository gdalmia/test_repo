// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include <filesystem>

namespace std { namespace filesystem = experimental::filesystem; }

namespace Philips { namespace Workflow {

	//! Provides settings in Workflow
	class Settings
	{
	public:
		enum class HeightUnit { Cm, Inch };
		enum class WeightUnit { Kilogram, Pound };
		enum class Language   { US, ES, FI, FR, HU, IT, NL, NO, PL, PT, DK, DE, CZ, SE, TR };

		Settings();
		Settings(const std::filesystem::path& source, bool test);
		virtual ~Settings() = default;
		
		//! Base path where all workflow level configurations will be stored
		virtual const std::filesystem::path& path() const;

		//! Indicates whether the workflow is in test mode or not
		bool test() const;

		//! Sets the format in which DateTime has to be displayed
		//! The accepted characters are:
		//!	y: Year, M: month, d: day, H: hour(24 hour format),
		//! h: hour(12 hour format), m: minutes, s: seconds, t: AM or PM
		void setDateTimeFormat(const std::string& format) const;

		//! Returns the format in which DateTime has to be displayed
		const std::string& dateTimeFormat() const;

		//! Sets the format in which Date has to be displayed
		void setDateFormat(const std::string& format) const;

		//! Returns the format in which Date has to be displayed
		const std::string& dateFormat() const;

		//! Sets the format in which Time has to be displayed
		void setTimeFormat(const std::string& format) const;

		//! Returns the format in which Time has to be displayed
		const std::string& timeFormat() const;

		// !set the height unit of the system
		void setHeightUnit(const HeightUnit& unit);

		//! get the height unit of the system
		HeightUnit getHeightUnit() const;

		//! set the weight unit of the system
		void setWeightUnit(const WeightUnit& unit);

		//! get the weight unit of the system
		WeightUnit getWeightUnit() const;

		//! set the language of the system
		void setLanguage(const Language& language) const;

		//! Returns the language of the system
		Language language() const;

		//! Set the Show Ideographic 
		void  setShowIdeographic(const bool showIdeographic) const;

		//! get the Show Ideographic 
		bool  showIdeographic() const;
	private:
		class Singleton;
		std::shared_ptr<Singleton> _singleton;
	};

}}
