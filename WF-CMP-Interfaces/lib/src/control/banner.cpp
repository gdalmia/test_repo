// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "Workflow/control/Banner.h"
#include "bannerView.h"

namespace Philips {namespace Workflow {namespace Control {

	struct Banner::PrivateBanner
	{
	public:
		PrivateBanner()		= default;
		~PrivateBanner()	= default;

		BannerViewModel& viewModel()
		{
			return BannerViewModel::instance();
		}
	};

	Banner::Banner(Sense::Control& parent, const UiStyle& style): Control(parent),_bannerView(nullptr), _pImpl(std::make_unique<PrivateBanner>())
	{
		if (style == UiStyle::Integrated)
		{
			this->_bannerView = std::make_unique<Workflow::Control::demoBanner>(parent, _pImpl->viewModel(), style);
			this->_bannerView->visible = true;
		}
		else if (style == UiStyle::Cart)
		{
			this->_bannerView = std::make_unique<Workflow::Control::demoBannerCart>(parent, _pImpl->viewModel(), style);
			this->_bannerView->visible = true;
		}
		else
		{
			this->_bannerView = std::make_unique<Workflow::Control::demoBannerTSM>(parent, _pImpl->viewModel(), style);
			this->_bannerView->visible = true;
		}
	}

	void Banner::setMode(const Modes& mode, const bool enable) const
	{
		_pImpl->viewModel().setMode(mode , enable);
	}

	Modes Banner::mode() const
	{
		return _pImpl->viewModel().mode();
	}


	Banner::~Banner() = default;
}}}