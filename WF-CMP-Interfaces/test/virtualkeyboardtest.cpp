// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>
#include <workflow.h>
#include "Workflow/control/testpeer/virtualkeyboardpeer.h"
#include "Workflow/control/testpeer/toucheditboxpeer.h"
#include "automateduitest.h"

using namespace Philips::Workflow::Control;

//! Tests the behavior of TouchEditBox's Clear Button visibility
class TestTouchEditBox : public TouchEditBox
{
public:
	TestTouchEditBox(Control &parent
		, VirtualKeyboard &keyboard
		, const TouchEditBox::ClearButtonVisible showClear)
		: TouchEditBox(parent, keyboard, showClear)
	{}

	void resetClearButtonVisibility()
	{
		this->setClearButtonVisibility(true);
	}

	void hideClearButton()
	{
		this->setClearButtonVisibility(false);
	}
};

class TestPanel : public Control
{
public:

	TestPanel(Control &parent
		, Philips::Workflow::Control::VirtualKeyboard& keyboard
		, const TouchEditBox::ClearButtonVisible showClear = TouchEditBox::ClearButtonVisible::Auto)
		:
		Control(parent)
		, parentPanel(*this)
		, editBox(parentPanel, keyboard, showClear)
		, onFocusButton(parentPanel)
		, offFocusButton(parentPanel)
	{
		layoutUpdateBegin();
		size = Size(1026, 450);
		parentPanel.name = L"parentPanel";
		parentPanel.size = Size(1026, 450);
		parentPanel.layout = Layout::Fill;
		parentPanel.colorBorder = Palette::Transparent * 0.00;
		parentPanel.color = Palette::Gray90;
		//editBox.composerClass = EditBox;
		editBox.name = L"editBox";
		editBox.position = Point(256, 88);
		editBox.size = Size(360, 48);
		editBox.font = Font(L"CentraleSansBook", 20, false, false, false, false, Font::Antialias::High);
		editBox.maxLength = 52;
		editBox.multiline = true;
		onFocusButton.position = Point(112, 200);
		onFocusButton.size = Size(128, 48);
		onFocusButton.text = L"focus on";
		offFocusButton.position = Point(256, 200);
		offFocusButton.size = Size(128, 48);
		offFocusButton.text = L"focus off";
		layoutUpdateEnd();
		onFocusButton.eventClicked = [this]() {Control::keyboard.setFocus(&editBox, true); };
		focusOnBtnPeer.reset(new Automation::Peer<Button>(onFocusButton));
		focusOffBtnPeer.reset(new Automation::Peer<Button>(offFocusButton));
		editBoxPeer.reset(new TestPeer::TouchEditBoxPeer(editBox));
	}

	void setFocusOnEditBox()	{ focusOnBtnPeer->mouseClick();	 }
	void setFocusOffEditBox()	{ focusOffBtnPeer->mouseClick(); }

	public:    Panel									parentPanel;
	public:    TestTouchEditBox							editBox;
	private:   Button									onFocusButton;
	private:   Button									offFocusButton;
	public: std::unique_ptr<TestPeer::TouchEditBoxPeer>	editBoxPeer;
	private:
		std::unique_ptr<Automation::Peer<Button>>		focusOnBtnPeer;
		std::unique_ptr<Automation::Peer<Button>>		focusOffBtnPeer;
		TestPanel(const TestPanel&);
};

//! Tests the behavior of VirtualKeyboard class
class VirtualKeyboardTest : public AutomatedUiTest
{
public:
	VirtualKeyboardTest() : AutomatedUiTest(Rect(Point(), Size(1280, 1024)))
	{
		keyboard.reset(new VirtualKeyboard(canvas()));
		panel.reset(new TestPanel(canvas(), *keyboard));
		keyboardPeer.reset(new TestPeer::VirtualKeyboardPeer(*keyboard));
	}

	void updateEditBox()
	{
		keyboard->setEditBox(&panel->editBox);
		panel->setFocusOnEditBox();
		keyboardPeer->enterText(L"Test");
	}

protected:
	std::unique_ptr<VirtualKeyboard>				keyboard;
	std::unique_ptr<TestPanel>						panel;
	std::unique_ptr<TestPeer::VirtualKeyboardPeer>	keyboardPeer;
};

TEST_F(VirtualKeyboardTest, When_EditBoxNotInFocus_Then_KeyboardIsHidden)
{
	panel->setFocusOffEditBox();
	ASSERT_EQ(keyboardPeer->isVisible(), false);
}

TEST_F(VirtualKeyboardTest, When_EditBoxInFocus_Then_KeyboardIsShown)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	ASSERT_EQ(keyboardPeer->isVisible(), true);
}

TEST_F(VirtualKeyboardTest, Given_EditBoxWithoutFilter_When_EditBoxInFocus_Then_KeyboardInAlphabhetMode)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	ASSERT_EQ(keyboardPeer->control.showNumeric, false);
}

TEST_F(VirtualKeyboardTest, Given_EditBoxWithNumericalFilter_When_EditBoxInFocus_Then_KeyboardInNumericMode)
{
	panel->editBox.filter = L"0123456789";
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	ASSERT_EQ(keyboardPeer->control.showNumeric, true);
}

TEST_F(VirtualKeyboardTest, Given_KeyboardInAlphabhetMode_When_TextIsEnteredInKeyboard_Then_EditBoxIsUpdated)
{
	const auto text = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	keyboardPeer->enterText(text);
	ASSERT_EQ(panel->editBox.text, text);
}

TEST_F(VirtualKeyboardTest, Given_KeyboardInNumericMode_When_TextIsEnteredInKeyboard_Then_EditBoxIsUpdated)
{
	const auto text = L"0123456789";
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	keyboardPeer->enterText(text);
	ASSERT_EQ(panel->editBox.text, text);
}

TEST_F(VirtualKeyboardTest, Given_EditBoxWithNumericalFilter_WhenTextIsPressed_Then_OnlyNumericTextIsShown)
{
	panel->editBox.filter = L"0123456789";
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	keyboardPeer->enterText(L"h");
	keyboardPeer->enterText(L"1");
	ASSERT_EQ(panel->editBox.text, L"1");
}

TEST_F(VirtualKeyboardTest, When_EditBoxNotInFocus_Then_ClearButtonIsNotVisible)
{
	panel->setFocusOffEditBox();
	ASSERT_EQ(panel->editBoxPeer->isClearButtonVisible(), false);
}


TEST_F(VirtualKeyboardTest, When_EditBoxWarningDisabled_Then_DefaultColour)
{
	panel->editBoxPeer->enableWarning(false);
	panel->setFocusOnEditBox();
	keyboardPeer->enterText(L"h");
	ASSERT_EQ(Color(Palette::Gray90), panel->editBoxPeer->getEditBoxBGColor());
}

TEST_F(VirtualKeyboardTest, When_EditBoxWarningEnabled_Then_BackGroundColorIsChanged)
{
	panel->editBoxPeer->enableWarning(true);
	panel->setFocusOnEditBox();
	keyboardPeer->enterText(L"h");
	ASSERT_EQ (Color::fromRgb(255, 241, 211) , panel->editBoxPeer->getEditBoxBGColor());
}

TEST_F(VirtualKeyboardTest, When_EditBoxWarningEnabled_Then_FontColorIsChanged)
{
	panel->editBoxPeer->enableWarning(true);
	panel->setFocusOnEditBox();
	keyboardPeer->enterText(L"h");
	ASSERT_EQ(Color::fromRgb(230, 88, 0), panel->editBoxPeer->getEditBoxFontColor());
}

TEST_F(VirtualKeyboardTest, When_EditBoxWarningEnabled_Then_BorderColorIsChanged)
{
	panel->editBoxPeer->enableWarning(true);
	panel->setFocusOnEditBox();
	keyboardPeer->enterText(L"h");
	ASSERT_EQ(Color::fromRgb(240, 124, 0), panel->editBoxPeer->getEditBoxBorderColor());
}

TEST_F(VirtualKeyboardTest, When_EditBoxIsInFocusAndEmpty_Then_ClearButtonIsNotVisible)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	ASSERT_EQ(panel->editBoxPeer->isClearButtonVisible(), false);
}

TEST_F(VirtualKeyboardTest, When_EditBoxIsInFocusAndHasText_Then_ClearButtonIsVisible)
{
	updateEditBox();
	ASSERT_EQ(panel->editBoxPeer->isClearButtonVisible(), true);
}

TEST_F(VirtualKeyboardTest, Given_EditboxWithText_When_ClearButtonIsClicked_Then_EditBoxTextIsCLeared)
{
	updateEditBox();
	panel->editBoxPeer->clear();
	ASSERT_EQ(panel->editBox.text, L"");
}

TEST_F(VirtualKeyboardTest, Given_Editbox_When_FocusIsSet_Then_FocusChangedEventIsSentWithFocus)
{
    bool hasFocus = false;
    panel->editBox.focusChanged.subscribe([&hasFocus](const bool& focus)
    {
        hasFocus = focus;
    });
    panel->setFocusOnEditBox();
    ASSERT_EQ(hasFocus, true);
}

TEST_F(VirtualKeyboardTest, Given_Editbox_When_FocusIsLost_Then_FocusChangedEventIsSentWithNoFocus)
{
    panel->setFocusOnEditBox();
    bool hasFocus = true;
    panel->editBox.focusChanged.subscribe([&hasFocus](const bool& focus)
    {
        hasFocus = focus;
    });
    panel->setFocusOffEditBox();
    ASSERT_EQ(hasFocus, false);
}

TEST_F(VirtualKeyboardTest, When_EditBoxTextIsChanged_Then_textChangedEventIsCalled)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	auto flag = false ;
	panel->editBox.textChanged.subscribe([&flag]() {flag = true; });
	keyboardPeer->enterText(L"T");
	ASSERT_EQ(flag, true);
}

TEST_F(VirtualKeyboardTest, When_EditBoxTextIsEntered_Then_characterEnteredEventIsCalled)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	panel->editBox.characterEntered.subscribe([](wchar_t wchar, bool& result) {
		if (wchar == L'e') { result = false; }
	});
	keyboardPeer->enterText(L"Test");
	ASSERT_EQ(panel->editBox.text, L"Tst");
}

TEST_F(VirtualKeyboardTest, When_EditBoxTextIsEntered_Then_tabPressedEventIsCalled)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	auto flag = false;
	panel->editBox.tabPressed.subscribe([&flag]() {flag = true; });
	keyboardPeer->pressTab();
	ASSERT_EQ(flag, true);
}

TEST_F(VirtualKeyboardTest, When_TabIsEntered_Then_FocusIsLostFromEditBox)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	panel->editBox.focusChanged.subscribe([](bool focus) {
		ASSERT_EQ(focus, false); });
	keyboardPeer->pressTab();
}

TEST_F(VirtualKeyboardTest, When_EditBoxIsInFocus_Then_keyboardShowEventIsCalled)
{
	auto flag = false;
	panel->editBox.keyboardShow.subscribe([&flag]() {flag = true; });
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	ASSERT_EQ(flag, true);
}

TEST_F(VirtualKeyboardTest, When_EditBoxIsOutOfFocus_Then_keyboardHideEventIsCalled)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	auto flag = false;
	panel->editBox.keyboardHide.subscribe([&flag]() {flag = true; });
	keyboardPeer->hideKeyboard();
	ASSERT_EQ(flag, true);
}

TEST_F(VirtualKeyboardTest, When_EditBoxIsInFocus_Then_focusChangedEventIsCalled)
{
	auto flag = false;
	panel->editBox.keyboardHide.subscribe([&flag]() {flag = true; });
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	keyboardPeer->hideKeyboard();
	ASSERT_EQ(flag, true);
}

TEST_F(VirtualKeyboardTest, When_KeyboardIsResized_Then_resizeEventIsCalled)
{
	auto flag = false;
	keyboard->resize.subscribe([&flag]() { flag = true; });
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	ASSERT_EQ(flag, true);
}

TEST_F(VirtualKeyboardTest, Given_EditBoxWithText_When_BackSpaceIsPressed_Then_EditBoxTextIsUpdated)
{
	updateEditBox();
	keyboardPeer->pressBackSpace();
	ASSERT_EQ(panel->editBox.text, L"Tes");
}

TEST_F(VirtualKeyboardTest, Given_KeyboardInAlphabetMode_When_LeftNumericButtonIsPressed_Then_KeyboardModeChangesToNumeric)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	keyboardPeer->pressLeftNumeric();
	ASSERT_EQ(keyboard->showNumeric, true);
}

TEST_F(VirtualKeyboardTest, Given_KeyboardInAlphabetMode_When_RightNumericButtonIsPressed_Then_KeyboardModeChangesToNumeric)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	keyboardPeer->pressRightNumeric();
	ASSERT_EQ(keyboard->showNumeric, true);
}

TEST_F(VirtualKeyboardTest, Given_KeyboardInNumericMode_When_LeftShiftButtonIsPressed_Then_KeyboardKeysAreUpdated)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	keyboard->showNumeric = true;
	keyboardPeer->pressLeftShift();
	ASSERT_EQ(keyboardPeer->firstKeyText(), L"!");
}

TEST_F(VirtualKeyboardTest, Given_KeyboardInNumericMode_When_RightShiftButtonIsPressed_Then_KeyboardKeysAreUpdated)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	keyboard->showNumeric = true;
	keyboardPeer->pressRightShift();
	ASSERT_EQ(keyboardPeer->firstKeyText(), L"!");
}

TEST_F(VirtualKeyboardTest, When_HideButtonIsPressed_Then_KeyboardIsHidden)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	keyboardPeer->pressHide();
	ASSERT_EQ(keyboardPeer->isVisible(), false);
}

TEST_F(VirtualKeyboardTest, When_SpaceButtonIsPressed_Then_EditBoxTextIsUpdated)
{
	updateEditBox();
	keyboardPeer->pressSpace();
	ASSERT_EQ(panel->editBox.text, L"Test ");
}

TEST_F(VirtualKeyboardTest, When_EnterButtonIsPressed_Then_EditBoxTextIsUpdated)
{
	updateEditBox();
	keyboardPeer->pressEnter();
	ASSERT_EQ(panel->editBox.text, L"Test\n");
}

TEST_F(VirtualKeyboardTest, When_ClearButtonSizeIsSetToDefault_VerifyClearButtonImageSizeIsSetToSmallSize)
{
	ASSERT_EQ(panel->editBox.clearButtonSize, TouchEditBox::ClearButtonSize::Small);
	ASSERT_EQ(panel->editBoxPeer->getClearButtonImageSize(), Size(22, 22));
}

TEST_F(VirtualKeyboardTest, When_ClearButtonSizeIsSetToLarge_VerifyClearButtonImageSizeIsSetToLargeSize)
{
	panel->editBox.clearButtonSize = TouchEditBox::ClearButtonSize::Large;
	ASSERT_EQ(panel->editBox.clearButtonSize, TouchEditBox::ClearButtonSize::Large);
	ASSERT_EQ(panel->editBoxPeer->getClearButtonImageSize(), Size(32, 32));
}

TEST_F(VirtualKeyboardTest, When_TextIsAddedInEditBox_VerifyClearButtonIsVisible)
{
	updateEditBox();
	ASSERT_TRUE(panel->editBoxPeer->isClearButtonVisible());
}

TEST_F(VirtualKeyboardTest, When_ClearButtonIsHiddenWhenFocussed_VerifyClearButtonIsInvisible)
{
	updateEditBox();
	panel->editBox.hideClearButton();
	ASSERT_FALSE(panel->editBoxPeer->isClearButtonVisible());
}

TEST_F(VirtualKeyboardTest, When_ClearButtonVisibilityIsResetWhenFocussed_VerifyClearButtonIsVisible)
{
	updateEditBox();
	panel->editBox.hideClearButton();
	panel->editBox.resetClearButtonVisibility();
	ASSERT_TRUE(panel->editBoxPeer->isClearButtonVisible());
}

TEST_F(VirtualKeyboardTest, When_EditBoxInFocus_Then_VerifyKeyBoardRect)
{
	keyboard->setEditBox(&panel->editBox);
	panel->setFocusOnEditBox();
	ASSERT_EQ(Sense::Rect(0, 0, 1280, 342), keyboard->getCurrentBounds());
}

TEST_F(VirtualKeyboardTest, When_EditBoxDisabled_Then_DisableColour)
{
	panel->editBoxPeer->enableControl(false);
	panel->editBoxPeer->enableWarning(false);
	Color fontColour;
	fontColour.r = 0.29372549019607841;
	fontColour.g = 0.29372549019607841;
	fontColour.b = 0.29372549019607841;
	fontColour.a = 1;
	ASSERT_EQ(fontColour, panel->editBoxPeer->getEditBoxFontColor());
}

TEST_F(VirtualKeyboardTest, When_EditBoxWithWarningAndDisabled_Then_VerifyFontColour)
{
	panel->editBoxPeer->enableControl(false);
	panel->editBoxPeer->enableWarning(true);
	Color fontColour;
	fontColour.r = 0.90196078431372551;
	fontColour.g = 0.34509803921568627;
	fontColour.b = 0.00000000000000000;
	fontColour.a = 1;
	ASSERT_EQ(fontColour, panel->editBoxPeer->getEditBoxFontColor());
}