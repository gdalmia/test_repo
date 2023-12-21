// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include <filesystem>
#include <memory>

namespace std { namespace filesystem = experimental::filesystem; }

namespace Philips { namespace Workflow {

	//! Provides system level settings in Workflow
	class SystemSettings
	{
	public:
		SystemSettings();
		~SystemSettings() = default;

		//! Define the system type
		enum class SystemType
		{
			Integrated = 0, //!< Integrated system
			Mobile,			//!< Mobile system
			None			//!< System type is unknown
		};

		//! Define the lab integration type for the system
		enum class IntegrationType
		{
			Philips = 0,	//!< Integrated with Philips lab
			Competitor,		//!< Integrated with competitor lab
			None			//!< Integration type is unknown
		};

		//! Set the system type 
		void setSystemType(SystemType) const;

		//! Get the system type
		SystemType systemType() const;

		//! Set the integration type for the system
		void setIntegrationType(IntegrationType) const;

		//! Get the integration type for the system
		IntegrationType integrationType() const;

		//! Set the license plugin path
		void setLicesnePluginPath(const std::filesystem::path&) const;

		//! Get the license plugin path
		std::filesystem::path licensePluginPath() const;
		
	private:
		class Singleton;
		std::shared_ptr<Singleton> _singleton;
	};

}}
