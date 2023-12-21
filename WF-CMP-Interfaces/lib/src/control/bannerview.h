// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "Ui\bannerintegrated.ui.h"
#include "Ui\bannercart.ui.h"
#include "Ui\bannertsm.ui.h"
#include "bannerviewModel.h"

namespace Philips {namespace Workflow {namespace Control {

	template<class T>
	class BannerView : public T
	{
	public:
		BannerView(Sense::Control& parent, BannerViewModel& model , const UiStyle& uiStyle);
		~BannerView();

	private:
		void setMode(const Modes& mode);
		void setDefaultColor();
		void setConnectionLostColor();
		void setModeNone();
		void setDemoMode();
		void setXrayConnectionLostMode();
		void setRemoteMode();
		void setEmergencyMode();
		void setServiceMode();
		void setAdministratorMode();

		UiStyle							_uiStyle;
		BannerViewModel&				_model;
		Event<Modes>::Subscription 		_token;
	};
	typedef BannerView<Philips::Workflow::Control::Ui::BannerIntegrated>		demoBanner;
	typedef BannerView<Philips::Workflow::Control::Ui::BannerCart>				demoBannerCart;
	typedef BannerView<Philips::Workflow::Control::Ui::BannerTsm>				demoBannerTSM;

}}}
