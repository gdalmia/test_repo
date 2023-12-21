// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>
#include "Workflow/control/testpeer/toucheditboxpeer.h"
#include "src/Resource.h"
#include "automateduitest.h"

using namespace Philips::Workflow::Control;

struct TouchEditBoxTestParams
{
	TouchEditBox::ClearButtonSize	size;
	bool							warning;
};

class TouchEditBoxTest : public AutomatedUiTest, public testing::WithParamInterface<TouchEditBoxTestParams>
{
public:
	TouchEditBoxTest() :
		AutomatedUiTest(Rect(Point(), Size(1280, 980))),
		keyboard(canvas()),
		touchEditBox(canvas(), keyboard),
		peer(touchEditBox)
	{
		keyboard.hide();
	}

	Sense::Image clearButtonImage(TouchEditBox::ClearButtonSize buttonSize, bool warning)
	{
		Sense::Image image;

		if (warning)
		{
			switch(buttonSize)
			{
				case TouchEditBox::ClearButtonSize::VerySmall :
					image = Philips::Workflow::Resource::cross_circle_orange_16;
					break;
				case TouchEditBox::ClearButtonSize::Small:
					image = Philips::Workflow::Resource::cross_circle_orange_22;
					break;
				case TouchEditBox::ClearButtonSize::Large:
					image = Philips::Workflow::Resource::cross_circle_orange_32;
					break;
			}
		}
		else
		{
			switch (buttonSize)
			{
			case TouchEditBox::ClearButtonSize::VerySmall:
				image = Philips::Workflow::Resource::cross_circle_grey_16;
				break;
			case TouchEditBox::ClearButtonSize::Small:
				image = Philips::Workflow::Resource::cross_circle_grey_22;
				break;
			case TouchEditBox::ClearButtonSize::Large:
				image = Philips::Workflow::Resource::cross_circle_grey_32;
				break;
			}
		}
		return image;
	}

protected:
	VirtualKeyboard	keyboard;
	TouchEditBox	touchEditBox;
	TestPeer::TouchEditBoxPeer	peer;
};

INSTANTIATE_TEST_SUITE_P(WhenEnableWarningIsTrue, TouchEditBoxTest, ::testing::Values(
	 TouchEditBoxTestParams{ TouchEditBox::ClearButtonSize::VerySmall, true },
	 TouchEditBoxTestParams{ TouchEditBox::ClearButtonSize::VerySmall, false },
	 TouchEditBoxTestParams{ TouchEditBox::ClearButtonSize::Small, true },
	 TouchEditBoxTestParams{ TouchEditBox::ClearButtonSize::Small, false },
	 TouchEditBoxTestParams{ TouchEditBox::ClearButtonSize::Large, true },	
	 TouchEditBoxTestParams{ TouchEditBox::ClearButtonSize::Large, false }
	));

TEST_P(TouchEditBoxTest, When_Warning_Enabled_Verify_ClearButtonImage)
{
	touchEditBox.enableWarning = GetParam().warning;
	touchEditBox.clearButtonSize = GetParam().size;
	ASSERT_EQ(this->peer.getClearButtonImage(), clearButtonImage(GetParam().size, GetParam().warning));
}

class TouchEditBoxTests : public AutomatedUiTest
{
public:
	TouchEditBoxTests() :
		AutomatedUiTest(Rect(Point(), Size(1280, 980))),
		keyboard(canvas()),
		touchEditBox(canvas(), keyboard),
		peer(touchEditBox)
	{
		keyboard.hide();
	}
	
protected:
	VirtualKeyboard	keyboard;
	TouchEditBox	touchEditBox;
	TestPeer::TouchEditBoxPeer	peer;				
};

TEST_F(TouchEditBoxTests, Given_EnterText_When_ClearButtonClicked_Then_Verify_EventIsTriggered)
{
	bool buttonclicked = false;
	touchEditBox.clearButtonClicked.subscribe([&buttonclicked]() { buttonclicked = true; });
	peer.enterText();
	peer.clear();

	ASSERT_TRUE(buttonclicked);
}

