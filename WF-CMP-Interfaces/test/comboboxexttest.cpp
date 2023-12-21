// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
//TICS -OLC#009 -INT#002

#include "automateduitest.h"
#include "Workflow/control/comboboxext.h"

using namespace Philips::Workflow::Control;

class MockComboBox : public ComboBoxExt
{
public:
	MockComboBox(Control& parent) : ComboBoxExt(parent)
	{
	}

	~MockComboBox()
	{
	}

	void onTabPress()
	{
		ComboBoxExt::onKeyboardDown(IKeyboard::Button::Tab);
	}
};

class ComboBoxExtTest : public AutomatedUiTest
{
public:
	ComboBoxExtTest() :
	AutomatedUiTest(Rect(Point(), Size(1280, 980))),
	comboboxext(canvas())
	{
		comboboxext.layout = Layout::Fill;
		comboboxext.tabStop = true;
	}

protected:
	MockComboBox	comboboxext;
};

TEST_F(ComboBoxExtTest, Given_ComboBoxExt_Then_VerifyEnabledValue)
{
	ASSERT_EQ(this->comboboxext.enabled, true);
}

TEST_F(ComboBoxExtTest, Given_ComboBoxExt_When_TabPressed_Then_TabPressedEventIsRaised)
{
	bool eventTriggered = false;
	comboboxext.tabPressed += [&eventTriggered] {	eventTriggered = true;	};
	comboboxext.onTabPress();		
	ASSERT_TRUE(eventTriggered);
}

TEST_F(ComboBoxExtTest, Given_ComboBoxExt_When_DisableComboBox_Then_VerifyDesabledValue)
{
	this->comboboxext.enabled = false;
	ASSERT_EQ(this->comboboxext.enabled, false);
}
