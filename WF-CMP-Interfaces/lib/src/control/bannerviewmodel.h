// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include <workflow.h>
#include <Workflow/control/banner.h>

namespace Philips {namespace Workflow { namespace Control {

	class BannerViewModel
	{
	public:
		static BannerViewModel& instance();

		void setMode(const Modes& mode, const bool enableMode);
		Modes mode() const;
		Event<Modes> changed;
		
	private:
		BannerViewModel();

		void setModeNone();
		void setDemoMode(const bool enable);
		void setXrayConnectionLostMode(const bool enable);
		void setRemoteMode(const bool enable);
		void setEmergencyMode(const bool enable);
		void setServiceMode(const bool enable);
		void setAdministratorMode(const bool enable);
		
		const Modes getNextMode();
		
		Modes _currentMode;
		std::map<Modes, bool> _modePriority;
	};


}}}