// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "src/control/bannerviewmodel.h"
#include <gtest/gtest.h>

using namespace Philips::Workflow::Control;

class BannerViewModelTest : public testing::Test
{
public:

	void  subscribeEvent(Modes& newMode)
	{
		BannerViewModel::instance().changed.subscribe([&](Modes mode)
		{
			newMode = mode;
		});
	}

	void setAllModes()
	{
		BannerViewModel::instance().setMode(Modes::Demo, true);
		BannerViewModel::instance().setMode(Modes::XrayConnectionLost, true);
		BannerViewModel::instance().setMode(Modes::Remote, true);
		BannerViewModel::instance().setMode(Modes::Emergency, true);
		BannerViewModel::instance().setMode(Modes::Service, true);
		BannerViewModel::instance().setMode(Modes::Administrator, true);
	}

	void resetModesDescendingPriority()
	{
		Modes modeset;
		subscribeEvent(modeset);
		BannerViewModel::instance().setMode(Modes::Demo, false);
		ASSERT_EQ(Modes::XrayConnectionLost, modeset);
		BannerViewModel::instance().setMode(Modes::XrayConnectionLost, false);
		ASSERT_EQ(Modes::Remote, modeset);
		BannerViewModel::instance().setMode(Modes::Remote, false);
		ASSERT_EQ(Modes::Emergency, modeset);
		BannerViewModel::instance().setMode(Modes::Emergency, false);
		ASSERT_EQ(Modes::Service, modeset);
		BannerViewModel::instance().setMode(Modes::Service, false);
		ASSERT_EQ(Modes::Administrator, modeset);
		BannerViewModel::instance().setMode(Modes::Administrator, false);
		ASSERT_EQ(Modes::None, modeset);
	}
};


TEST_F(BannerViewModelTest, Given_AllModesSet_When_ModesResetOnPriority_Then_NextPriorityModeSet)
{
	setAllModes();
	resetModesDescendingPriority();
}

typedef std::tuple<Modes, Modes, Modes> TestParams;
class BannerViewModelModeTest : public testing::TestWithParam<TestParams>
{
public:
	void subscribeEvent(Modes& newMode)
	{
		BannerViewModel::instance().changed.subscribe([&](Modes mode)
		{
			newMode = mode;
		});
	}
};

TEST_P(BannerViewModelModeTest, When_CurrentModeIsNone_Then_OtherModesAreSet)
{
	const auto params = GetParam();
	const auto setCurrentMode = std::get<0>(params);
	const auto setMode = std::get<1>(params);
	const auto expectedMode = std::get<2>(params);

	Modes newMode;
	subscribeEvent(newMode);
	BannerViewModel::instance().setMode(setCurrentMode, true);
	subscribeEvent(newMode);
	BannerViewModel::instance().setMode(setMode, true);
	ASSERT_EQ(expectedMode, newMode);

	BannerViewModel::instance().setMode(setCurrentMode, false);
	BannerViewModel::instance().setMode(setMode, false);
}

std::vector< std::tuple< Modes, Modes, Modes>> getParameterFoDemoMode()
{
	std::vector< std::tuple< Modes, Modes, Modes>> parm;
	parm.push_back(std::make_tuple(Modes::Demo, Modes::Demo, Modes::Demo));
	parm.push_back(std::make_tuple(Modes::Demo, Modes::Remote, Modes::Demo));
	parm.push_back(std::make_tuple(Modes::Demo, Modes::Emergency, Modes::Demo));
	parm.push_back(std::make_tuple(Modes::Demo, Modes::Service, Modes::Demo));
	parm.push_back(std::make_tuple(Modes::Demo, Modes::Administrator, Modes::Demo));
	return parm;
}

std::vector< std::tuple< Modes, Modes, Modes>> getParameterForXrayMode()
{
	std::vector< std::tuple< Modes, Modes, Modes>> parm;
	parm.push_back(std::make_tuple(Modes::XrayConnectionLost, Modes::Demo, Modes::Demo));
	parm.push_back(std::make_tuple(Modes::XrayConnectionLost, Modes::XrayConnectionLost, Modes::XrayConnectionLost));
	parm.push_back(std::make_tuple(Modes::XrayConnectionLost, Modes::Remote, Modes::XrayConnectionLost));
	parm.push_back(std::make_tuple(Modes::XrayConnectionLost, Modes::Emergency, Modes::XrayConnectionLost));
	parm.push_back(std::make_tuple(Modes::XrayConnectionLost, Modes::Service, Modes::XrayConnectionLost));
	parm.push_back(std::make_tuple(Modes::XrayConnectionLost, Modes::Administrator, Modes::XrayConnectionLost));
	return parm;
}

std::vector< std::tuple< Modes, Modes, Modes>> getParameterForRemoteMode()
{
	std::vector< std::tuple< Modes, Modes, Modes>> parm;
	parm.push_back(std::make_tuple(Modes::Remote, Modes::Demo, Modes::Demo));
	parm.push_back(std::make_tuple(Modes::Remote, Modes::XrayConnectionLost, Modes::XrayConnectionLost));
	parm.push_back(std::make_tuple(Modes::Remote, Modes::Remote, Modes::Remote));
	parm.push_back(std::make_tuple(Modes::Remote, Modes::Emergency, Modes::Remote));
	parm.push_back(std::make_tuple(Modes::Remote, Modes::Service, Modes::Remote));
	parm.push_back(std::make_tuple(Modes::Remote, Modes::Administrator, Modes::Remote));
	return parm;
}


std::vector< std::tuple< Modes, Modes, Modes>> getParameterForEmergencyMode()
{
	std::vector< std::tuple< Modes, Modes, Modes>> parm;
	parm.push_back(std::make_tuple(Modes::Emergency, Modes::Demo, Modes::Demo));
	parm.push_back(std::make_tuple(Modes::Emergency, Modes::XrayConnectionLost, Modes::XrayConnectionLost));
	parm.push_back(std::make_tuple(Modes::Emergency, Modes::Remote, Modes::Remote));
	parm.push_back(std::make_tuple(Modes::Emergency, Modes::Emergency, Modes::Emergency));
	parm.push_back(std::make_tuple(Modes::Emergency, Modes::Service, Modes::Emergency));
	parm.push_back(std::make_tuple(Modes::Emergency, Modes::Administrator, Modes::Emergency));
	return parm;
}

std::vector< std::tuple< Modes, Modes, Modes>> getParameterForServiceMode()
{
	std::vector< std::tuple< Modes, Modes, Modes>> parm;
	parm.push_back(std::make_tuple(Modes::Service, Modes::Demo, Modes::Demo));
	parm.push_back(std::make_tuple(Modes::Service, Modes::XrayConnectionLost, Modes::XrayConnectionLost));
	parm.push_back(std::make_tuple(Modes::Service, Modes::Remote, Modes::Remote));
	parm.push_back(std::make_tuple(Modes::Service, Modes::Emergency, Modes::Emergency));
	parm.push_back(std::make_tuple(Modes::Service, Modes::Service, Modes::Service));
	parm.push_back(std::make_tuple(Modes::Service, Modes::Administrator, Modes::Service));
	return parm;
}

std::vector< std::tuple< Modes, Modes, Modes>> getParameterForAdministratorMode()
{
	std::vector< std::tuple< Modes, Modes, Modes>> parm;
	parm.push_back(std::make_tuple(Modes::Administrator, Modes::Demo, Modes::Demo));
	parm.push_back(std::make_tuple(Modes::Administrator, Modes::XrayConnectionLost, Modes::XrayConnectionLost));
	parm.push_back(std::make_tuple(Modes::Administrator, Modes::Remote, Modes::Remote));
	parm.push_back(std::make_tuple(Modes::Administrator, Modes::Emergency, Modes::Emergency));
	parm.push_back(std::make_tuple(Modes::Administrator, Modes::Service, Modes::Service));
	parm.push_back(std::make_tuple(Modes::Administrator, Modes::Administrator, Modes::Administrator));
	return parm;
}

INSTANTIATE_TEST_CASE_P(testDemo,			BannerViewModelModeTest, ::testing::ValuesIn(getParameterFoDemoMode()));
INSTANTIATE_TEST_CASE_P(testXray,			BannerViewModelModeTest, ::testing::ValuesIn(getParameterForXrayMode()));
INSTANTIATE_TEST_CASE_P(testRemote,			BannerViewModelModeTest, ::testing::ValuesIn(getParameterForRemoteMode()));
INSTANTIATE_TEST_CASE_P(testEmergency,		BannerViewModelModeTest, ::testing::ValuesIn(getParameterForEmergencyMode()));
INSTANTIATE_TEST_CASE_P(testService,		BannerViewModelModeTest, ::testing::ValuesIn(getParameterForServiceMode()));
INSTANTIATE_TEST_CASE_P(testAdministrator,	BannerViewModelModeTest, ::testing::ValuesIn(getParameterForAdministratorMode()));
