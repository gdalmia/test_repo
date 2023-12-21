// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "Workflow/control/banner.h"
#include <Automation.h>

namespace Philips {namespace Workflow {namespace Control {namespace TestPeer {

	class BannerPeer
	{
	public:
		BannerPeer(Banner &control, const Control::UiStyle& style);
		bool isBannerVisible()			const;
		std::wstring getBannerMode()	const;
		Color getButtonColor()			const;
		Color getSeperatorColor()		const;
		Modes bannerMode()			const;
	private:
		std::unique_ptr<Sense::Automation::Peer<Sense::DLS::ButtonCustom>>    _button;
		std::unique_ptr<Sense::Automation::Peer<Sense::DLS::Separator>>       _seperator;
		Banner&																  _bannerControl;
	};
	
}}}}