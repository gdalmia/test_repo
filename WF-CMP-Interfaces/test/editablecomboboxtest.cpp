// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
//TICS -OLC#009 -INT#002

#include "Workflow/control/testpeer/editablecomboboxpeer.h"
#include "Workflow/control/editablecombobox.h"
#include "automateduitest.h"

using namespace Philips::Workflow::Control;

class TestEditableComboBox : public AutomatedUiTest
{
public:
    TestEditableComboBox() :
        AutomatedUiTest(Rect(Point(), Size(1280, 980))),
        keyboard(canvas()),
        editableComboBox(static_cast<Control&>(canvas()), keyboard),
        editableComboBoxPeer(editableComboBox),
        button(canvas()),
        buttonPeer(button)
    {
        editableComboBox.size = Size(200, 40);
        editableComboBox.position = Point(20, 20);
        button.size = Size(300, 60);
        button.position = Point(80, 80);
    }

    ~TestEditableComboBox()
    {
    }

protected:
    VirtualKeyboard                               keyboard;
    EditableComboBox                              editableComboBox     ;
    TestPeer::EditableComboBoxPeer                editableComboBoxPeer ;
    Button                                        button;
    Sense::Automation::Peer<Button>               buttonPeer;
};

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_ComboboxNotVisible_Then_Verify_EditBoxNotVisibile)
{
    editableComboBox.visible = false;
    ASSERT_EQ(editableComboBoxPeer.editBoxVisible(), false);
    
}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_ComboboxIsVisible_Then_Verify_EditBoxVisibile)
{
    editableComboBox.visible = true;
    ASSERT_EQ(editableComboBoxPeer.editBoxVisible(), true);

}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_ComboboxNotEnabled_Then_Verify_EditBoxNotEnabled)
{
    editableComboBox.enabled = false;
    ASSERT_EQ(editableComboBoxPeer.editBoxEnabled(), false);

}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_ComboboxIsEnabled_Then_Verify_EditBoxEnabled)
{
    editableComboBox.enabled = true;
    ASSERT_EQ(editableComboBoxPeer.editBoxEnabled(), true);
}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_assignItem_Then_Verify_ItemCount)
{
    std::vector<ComboBox::Item> Items = { {L"Mr"}, {L"Mrs"} };
    editableComboBox.assign(Items);
    ASSERT_EQ(editableComboBox.getItems().size(), 2);
}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_SelectItem_Then_Verify_EditBoxText)
{
    std::vector<ComboBox::Item> Items = { {L"Mr"}, {L"Mrs"} };
    editableComboBox.assign(Items);
    editableComboBoxPeer.selectItem(1);
    ASSERT_EQ(editableComboBox.text(), L"Mrs");
}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_InsertText_Then_Verify_EditBoxText)
{
    editableComboBoxPeer.insertText(L"Mr");
    ASSERT_EQ(editableComboBox.text(), L"Mr");
}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_InsertText_Then_Verify_ComboBoxTextSelection)
{
    std::vector<ComboBox::Item> Items = { {L"Mr"}, {L"Mrs"} };
    editableComboBox.assign(Items);
    editableComboBoxPeer.insertText(L"Mr");
    ASSERT_EQ(editableComboBox.getSelectedItem(), 0);
}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_RemoveText_Then_Verify_ComboBoxTextSelection)
{
    std::vector<ComboBox::Item> Items = { {L"Mr"}, {L"Mrs"} };
    editableComboBox.assign(Items);
    editableComboBoxPeer.insertText(L"Mr");
    editableComboBoxPeer.insertText(L"Male");
    ASSERT_EQ(editableComboBox.getSelectedItem(), -1);
}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_InsertText_Then_Verify_ComboBoxEventSelection)
{
    bool eventTriggered = false;
    std::vector<ComboBox::Item> Items = { {L"Mr"}, {L"Mrs"} };
    editableComboBox.assign(Items);
	editableComboBox.eventSelectionChanged = [&eventTriggered]	{	eventTriggered = true;	};
    editableComboBoxPeer.insertText(L"Mr");
    ASSERT_EQ(eventTriggered, true);
}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_keyboardPressedInEditBox_Then_VerifyEventIsFired)
{
	bool eventTriggered = false;
	editableComboBox.editBox.keyboardDown += [&eventTriggered](IKeyboard::Button) { eventTriggered = true;	};

	std::vector<ComboBox::Item> Items = { {L"Mr"}, {L"Mrs"} };
	editableComboBox.assign(Items);
	editableComboBoxPeer.pressKey(IKeyboard::Button::Enter);
	ASSERT_EQ(eventTriggered, true);
}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_ClickInsideEditBox_Then_VerifyVirtualKeyboardVisibility)
{
    editableComboBoxPeer.pressKey(IKeyboard::Button::Enter);
    ASSERT_EQ(keyboard.isKeyBoardVisible(), true);
}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_ClickOutsideEditBox_Then_VerifyVirtualKeyboardVisibility)
{
    editableComboBoxPeer.pressKey(IKeyboard::Button::Enter);
    ASSERT_EQ(keyboard.isKeyBoardVisible(), true);
    buttonPeer.mouseClick();
    ASSERT_EQ(keyboard.isKeyBoardVisible(), false);
}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_SelectItem_Then_VerifyVirtualKeyboardVisibility)
{
    std::vector<ComboBox::Item> Items = { {L"Mr"}, {L"Mrs"} };
    editableComboBox.assign(Items);
    ASSERT_EQ(keyboard.isKeyBoardVisible(), false);
    editableComboBoxPeer.selectItem(1);
    ASSERT_EQ(keyboard.isKeyBoardVisible(), false);
}

TEST_F(TestEditableComboBox, Given_EditableComboBox_When_EditBoxValueIsChanged_Then_VerifyeditboxTextChangedIsFired)
{
	bool eventTriggered = false;
	std::wstring value = L"New";
	std::wstring newValue;
	editableComboBox.editboxTextChanged += [&]()
	{
		newValue = editableComboBox.editBox.text;
		eventTriggered = true;
	};

	editableComboBox.editBox.text = value;

	ASSERT_TRUE(eventTriggered);
	ASSERT_EQ(newValue, value);
}

TEST_F(TestEditableComboBox, Given_EditableComboBoxInFocus_When_TabKeyIsPressed_Then_Verify_TabPressedEventIsRaised)
{
	bool eventTriggered = false;
	std::vector<ComboBox::Item> Items = { {L"Mr"}, {L"Mrs"} };
	editableComboBox.assign(Items);
	editableComboBox.tabPressed += [&eventTriggered] {	eventTriggered = true;	};
	editableComboBoxPeer.pressKey(IKeyboard::Button::Tab);
	ASSERT_EQ(eventTriggered, true);
}