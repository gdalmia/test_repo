// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
//TICS -INT#002 : Members are used in TestCases, so made protected

#include "Workflow/control/hyperlinklabel.h"
#include "automateduitest.h"

using namespace Philips::Workflow::Control;

namespace Test
{
	class HyperlinkLabelTest : public AutomatedUiTest
	{
	public:
		HyperlinkLabelTest() :
			AutomatedUiTest(Rect(Point(), Size(1280, 980))),
			hyperlinkLabel(static_cast<Control&>(canvas())),
			peer(hyperlinkLabel)
		{
			hyperlinkLabel.size = Size(200, 40);
			hyperlinkLabel.position = Point(20, 20);
			hyperlinkLabel.text = L"Test";
		}

		~HyperlinkLabelTest()
		{
		}

	protected:
		HyperlinkLabel                              hyperlinkLabel;
		Automation::Peer<Label>						peer;
	};

	TEST_F(HyperlinkLabelTest, Given_HyperlinkLabel_When_Clicked_Then_VerifyeventClickedIsFired)
	{
		bool eventReceived = false;
		hyperlinkLabel.eventClicked += [&eventReceived]() { eventReceived = true; };
		peer.mouseClick();
		ASSERT_EQ(eventReceived, true);
	}

	TEST_F(HyperlinkLabelTest, Given_HyperlinkLabel_When_linkFontIsSet_Then_VerifyControlFont)
	{
		hyperlinkLabel.linkFont = Font(L"CentraleSansBold", 20, false, false, false, false, Font::Antialias::High);
		ASSERT_EQ(hyperlinkLabel.font->underline, true);
		ASSERT_EQ(hyperlinkLabel.font->name, L"CentraleSansBold");
		ASSERT_EQ(hyperlinkLabel.font->size, 20);
	}
}