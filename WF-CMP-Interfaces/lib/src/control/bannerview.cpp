// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "bannerview.h"
#include "../Localization.h"

namespace Philips {namespace Workflow {namespace Control {

	template<class T>
	BannerView<T>::BannerView(Sense::Control& parent, BannerViewModel& model, const UiStyle &uiStyle):
		T(parent),
		_uiStyle(uiStyle),
		_model(model)
	{
		
		setMode(Modes::None);
		_token = _model.changed.subscribe([this](Modes mode) { setMode(mode); });
	}

	template<class T>
	BannerView<T>::~BannerView()
	{
		_model.changed.unsubscribe(_token);
	}

	template<class T>
	void BannerView<T>::setMode(const Modes& mode)
	{
		switch (mode)
		{
		case Modes::None:				setModeNone();					break;
		case Modes::Demo:				setDemoMode();					break;
		case Modes::XrayConnectionLost:	setXrayConnectionLostMode();	break;
		case Modes::Remote:				setRemoteMode();				break;
		case Modes::Emergency:			setEmergencyMode();				break;
		case Modes::Service:			setServiceMode();				break;
		case Modes::Administrator:		setAdministratorMode();			break;
		}
	}
	
	template<class T>
	void BannerView<T>::setDefaultColor()
	{
		this->separator.color1					= Palette::S_Green45;
		this->buttonCustom.colorFontUp			= Palette::Green90;
		this->buttonCustom.colorFontDown		= Palette::Green90;
		this->buttonCustom.colorFontHover		= Palette::Green90;
		this->buttonCustom.colorFontDisabled	= Palette::Green90;
		this->buttonCustom.colorButtonUp		= Palette::S_Green45;
		this->buttonCustom.colorButtonDown		= Palette::S_Green45;
		this->buttonCustom.colorButtonHover		= Palette::S_Green45;
		this->buttonCustom.colorButtonDisabled	= Palette::S_Green45;
	}

	template<class T>
	void BannerView<T>::setConnectionLostColor()
	{
		this->separator.color1							=Palette::S_Yellow45;
		this->buttonCustom.colorFontUp 					=Palette::S_Yellow45;
		this->buttonCustom.colorFontDown 				=Palette::S_Yellow45;
		this->buttonCustom.colorFontHover 				=Palette::S_Yellow45;
		this->buttonCustom.colorFontDisabled 			=Palette::S_Yellow45;
		this->buttonCustom.colorButtonUp 				=Color::fromRgb(30, 25, 5);
		this->buttonCustom.colorButtonDown 				=Color::fromRgb(30, 25, 5);
		this->buttonCustom.colorButtonHover 			=Color::fromRgb(30, 25, 5);
		this->buttonCustom.colorButtonDisabled 			=Color::fromRgb(30, 25, 5);
	}

	template<class T>
	void BannerView<T>::setModeNone()
	{
		this->buttonCustom.visible = false;
		this->separator.visible = false;
	}
	
	template<class T>
	void BannerView<T>::setDemoMode()
	{
		this->buttonCustom.visible = true;
		this->separator.visible = true;
		this->buttonCustom.size = Size(178, 40);
		this->buttonCustom.text = Localization::demoMode;
		this->buttonCustom.position = _uiStyle == UiStyle::Integrated ? Point(867, 0) : Point(547, 0);
		setDefaultColor();
	}

	template <class T>
	void BannerView<T>::setXrayConnectionLostMode()
	{
		this->buttonCustom.visible = true;
		this->separator.visible = true;
		this->buttonCustom.size = Size(294, 36);
		this->buttonCustom.text = Localization::xrayConnectionLost;
		this->buttonCustom.position = _uiStyle == UiStyle::Integrated ? Point(813, 4) : Point(494, 4);
		setConnectionLostColor();
	}

	template <class T>
	void BannerView<T>::setRemoteMode()
	{
		this->buttonCustom.visible = true;
		this->separator.visible = true;
		this->buttonCustom.size = Size(200, 40);
		this->buttonCustom.text = Localization::remoteMode;
		this->buttonCustom.position = _uiStyle == UiStyle::Integrated ? Point(859, 0) : Point(539, 0);
		setDefaultColor();
	}

	template <class T>
	void BannerView<T>::setEmergencyMode()
	{
		this->buttonCustom.visible = true;
		this->separator.visible = true;
		this->buttonCustom.size = Size(242, 40);
		this->buttonCustom.text = Localization::emergencyMode;
		this->buttonCustom.position = _uiStyle == UiStyle::Integrated ? Point(840, 0) :  Point(520, 0);
		setDefaultColor();
	}

	template <class T>
	void BannerView<T>::setServiceMode()
	{
		this->buttonCustom.visible = true;
		this->separator.visible = true;
		this->buttonCustom.size = Size(201, 40);
		this->buttonCustom.text = Localization::serviceMode;
		this->buttonCustom.position = _uiStyle == UiStyle::Integrated ? Point(860, 0) :  Point(540, 0);
		setDefaultColor();
	}

	template <class T>
	void BannerView<T>::setAdministratorMode()
	{
		this->buttonCustom.visible = true;
		this->separator.visible = true;
		this->buttonCustom.text = Localization::administratorMode;
		this->buttonCustom.size = Size(288, 40);
		this->buttonCustom.position = _uiStyle == UiStyle::Integrated ? Point(817, 0) :  Point(497, 0);
		setDefaultColor();
	}

	template class BannerView<Philips::Workflow::Control::Ui::BannerIntegrated>;
	template class BannerView<Philips::Workflow::Control::Ui::BannerCart>;
	template class BannerView<Philips::Workflow::Control::Ui::BannerTsm>;
}}}