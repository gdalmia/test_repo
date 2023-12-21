// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "bannerviewmodel.h"
namespace Philips {namespace Workflow {namespace Control {

	BannerViewModel::BannerViewModel():_currentMode(Modes::None)
	{
		_modePriority.insert(std::pair<Modes, bool>(Modes::Demo, false));
		_modePriority.insert(std::pair<Modes, bool>(Modes::XrayConnectionLost, false));
		_modePriority.insert(std::pair<Modes, bool>(Modes::Remote, false));
		_modePriority.insert(std::pair<Modes, bool>(Modes::Emergency, false));
		_modePriority.insert(std::pair<Modes, bool>(Modes::Service, false));
		_modePriority.insert(std::pair<Modes, bool>(Modes::Administrator, false));
		_modePriority.insert(std::pair<Modes, bool>(Modes::None, true));
	}
	
	void BannerViewModel::setMode(const Modes& mode,const bool enable)
	{
		switch (mode)
		{
		case Modes::None:				setModeNone();						break;
		case Modes::Demo:				setDemoMode(enable);				break;
		case Modes::XrayConnectionLost: setXrayConnectionLostMode(enable); 	break;
		case Modes::Remote:				setRemoteMode(enable); 				break;
		case Modes::Emergency:			setEmergencyMode(enable);			break;
		case Modes::Service:			setServiceMode(enable) ;			break;
		case Modes::Administrator:		setAdministratorMode(enable);		break;
		}
	}

	Modes BannerViewModel::mode() const
	{
		return _currentMode;
	}

	void BannerViewModel::setModeNone()
	{
		_modePriority.at(Modes::None) = true;
		changed(getNextMode());
	}

	void BannerViewModel::setDemoMode(const bool enable)
	{
		_modePriority.at(Modes::Demo) = enable;
		changed(getNextMode());
	}

	void BannerViewModel::setXrayConnectionLostMode(const bool enable)
	{
		_modePriority.at(Modes::XrayConnectionLost) = enable;
		changed(getNextMode());
	}

	void BannerViewModel::setRemoteMode(const bool enable)
	{
		_modePriority.at(Modes::Remote) = enable;
		changed(getNextMode());
	}

	void BannerViewModel::setEmergencyMode(const bool enable)
	{
		_modePriority.at(Modes::Emergency) = enable;
		changed(getNextMode());
	}

	void BannerViewModel::setServiceMode(const bool enable)
	{
		_modePriority.at(Modes::Service) = enable;
		changed(getNextMode());
	}

	void BannerViewModel::setAdministratorMode(const bool enable)
	{
		_modePriority.at(Modes::Administrator) = enable;
		changed(getNextMode());
	}

	const Modes BannerViewModel::getNextMode()
	{
		for (auto it = _modePriority.begin(); it != _modePriority.end(); ++it)
		{
			if (it->second == true)
				return _currentMode = it->first;
		}
		return _currentMode = Modes::None;
	}

	BannerViewModel& BannerViewModel::instance() 
	{
		static BannerViewModel viewModel;
		return viewModel;
	}

}}}