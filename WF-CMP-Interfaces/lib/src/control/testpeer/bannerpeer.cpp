// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/control/testpeer/bannerpeer.h"
#include "../bannerview.h"

namespace Philips {namespace Workflow {namespace Control {namespace TestPeer {

	namespace
	{
		template<typename TView>
		TView &view(const std::unique_ptr<Sense::Control> &control)
		{
			return *dynamic_cast<TView*>(control.get());
		}
	}
	
	BannerPeer::BannerPeer(Banner &control, const Control::UiStyle& style):_bannerControl(control)
	{
		if (style == UiStyle::Integrated)
		{
			_button = std::make_unique<Peer<ButtonCustom>>(view<Philips::Workflow::Control::Ui::BannerIntegrated>(control._bannerView).buttonCustom);
			_seperator = std::make_unique<Peer<Separator>>(view<Philips::Workflow::Control::Ui::BannerIntegrated>(control._bannerView).separator);
		}
		else if (style == UiStyle::Cart)
		{
			_button = std::make_unique<Peer<ButtonCustom>>(view<Philips::Workflow::Control::Ui::BannerCart>(control._bannerView).buttonCustom);
			_seperator = std::make_unique<Peer<Separator>>(view<Philips::Workflow::Control::Ui::BannerCart>(control._bannerView).separator);
		}
		else
		{
			_button = std::make_unique<Peer<ButtonCustom>>(view<Philips::Workflow::Control::Ui::BannerTsm>(control._bannerView).buttonCustom);
			_seperator = std::make_unique<Peer<Separator>>(view<Philips::Workflow::Control::Ui::BannerTsm>(control._bannerView).separator);
		}

	}

	std::wstring BannerPeer::getBannerMode() const
	{
		if(_button)
			return 	_button->control.text;
		return L"";
	}

	bool BannerPeer::isBannerVisible() const
	{
		return _button->control.isVisible();
	}

	Color BannerPeer::getButtonColor() const
	{
		return _button->control.colorButtonUp;
	}

	Color BannerPeer::getSeperatorColor() const
	{
		return _seperator->control.color1;
	}

	Modes BannerPeer::bannerMode() const
	{
		return _bannerControl.mode();
	}
	
}}}}