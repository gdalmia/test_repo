// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "automateduitest.h"
#include "Workflow/control/banner.h"
#include "src/Localization.h"
#include "Workflow/control/testpeer/bannerpeer.h"

using namespace Philips::Workflow;
using namespace Philips::Workflow::Control;

namespace Test
{
	
	class BannerTest : public AutomatedUiTest, public testing::WithParamInterface<UiStyle>
	{
	public:
		BannerTest() :AutomatedUiTest(Sense::Rect(Sense::Point(), Sense::Size(1920, 1080)))
			, style(GetParam())
		{
			banner = std::make_unique<Banner>(canvas(), style);
			peer = std::make_unique<TestPeer::BannerPeer>(*banner, style);
			resetModes();
		}

		~BannerTest() = default;

		void assertXrayConnectionLostColor()
		{
			ASSERT_EQ(peer->getSeperatorColor(), Palette::S_Yellow45);
			ASSERT_EQ(peer->getButtonColor(), Color::fromRgb(30, 25, 5));
		}
		
	protected:
		UiStyle style;
		std::unique_ptr<Banner> banner;
		std::unique_ptr<TestPeer::BannerPeer> peer;

	private:
		void resetModes()
		{
			std::vector<Modes> modes = { Modes::Administrator,Modes::Demo,Modes::Emergency,Modes::None,
			Modes::Remote,Modes::Service,Modes::XrayConnectionLost };
			for (auto &mode : modes)
				banner->setMode(mode, false);
		}
	};


	TEST_P(BannerTest, When_NoModeIsSet_Then_Banner_Is_InvisibleVisible)
	{
		ASSERT_FALSE(peer->isBannerVisible());
		ASSERT_EQ(Modes::None, peer->bannerMode());
	}
	
	TEST_P(BannerTest, When_ModeNoneIsSet_Then_Banner_Is_InvisibleVisible)
	{
		banner->setMode(Modes::None, true);
		ASSERT_FALSE(peer->isBannerVisible());
	}

	TEST_P(BannerTest, When_DemoModeIsSet_Then_Banner_Is_DemoBanner)
	{
		banner->setMode(Modes::Demo, true);
		ASSERT_TRUE(peer->isBannerVisible());
		ASSERT_EQ(Localization::demoMode, peer->getBannerMode());
	}

	TEST_P(BannerTest, When_XrayConnectionLostModeIsSet_Then_Banner_Is_XrayLost)
	{
		banner->setMode(Modes::XrayConnectionLost, true);
		ASSERT_TRUE(peer->isBannerVisible());
		ASSERT_EQ(Localization::xrayConnectionLost, peer->getBannerMode());
	}

	TEST_P(BannerTest, When_XrayConnectionLostModeIsSet_Then_BannerColor_Is_Changed)
	{
		banner->setMode(Modes::XrayConnectionLost, true);
		ASSERT_TRUE(peer->isBannerVisible());
		this->assertXrayConnectionLostColor();
	}
	
	TEST_P(BannerTest, When_RemoteModeIsSet_Then_Banner_Is_RemoteMode)
	{
		banner->setMode(Modes::Remote, true);
		ASSERT_TRUE(peer->isBannerVisible());
		ASSERT_EQ(Localization::remoteMode, peer->getBannerMode());
	}

	TEST_P(BannerTest, When_EmergecnyModeIsSet_Then_Banner_Is_EmergencyMode)
	{
		banner->setMode(Modes::Emergency, true);
		ASSERT_TRUE(peer->isBannerVisible());
		ASSERT_EQ(Localization::emergencyMode, peer->getBannerMode());
	}

	TEST_P(BannerTest, When_ServiceModeIsSet_Then_Banner_Is_ServiceMode)
	{
		banner->setMode(Modes::Service, true);
		ASSERT_TRUE(peer->isBannerVisible());
		ASSERT_EQ(Localization::serviceMode, peer->getBannerMode());
	}
	
	TEST_P(BannerTest, When_AdminModeIsSet_Then_Banner_Is_AdminBannerIsSet)
	{
		banner->setMode(Modes::Administrator, true);
		ASSERT_TRUE(peer->isBannerVisible());
		ASSERT_EQ(Localization::administratorMode, peer->getBannerMode());
	}

	INSTANTIATE_TEST_CASE_P(test, BannerTest, testing::Values(UiStyle::Integrated, UiStyle::Cart, UiStyle::Tsm));
}