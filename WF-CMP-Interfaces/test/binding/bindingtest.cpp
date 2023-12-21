// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <Workflow/binding.h>
#include "testpeer.h"
#include "Workflow/control/testpeer/virtualkeyboardpeer.h"
#include "../automateduitest.h"
#include "Workflow/stringconverter.h"

namespace Test { namespace Binding {

	class BindingUiTest : public AutomatedUiTest
	{
	public:
		BindingUiTest() :AutomatedUiTest(Rect(Point(), Size(1026, 850)))
		{
			keyboard.reset(new VirtualKeyboard(canvas()));
			model.reset(new ViewModel());
			view.reset(new View(canvas(), *keyboard, *model));
			view->size = canvas().size;
			
			peer.reset(new ViewtTestPeer(*view));
			keyboardPeer.reset(new VirtualKeyboardPeer(*keyboard));
		}

		std::unique_ptr<VirtualKeyboard> keyboard;
		std::unique_ptr<ViewModel> model;
		std::unique_ptr<View> view;
		std::unique_ptr<ViewtTestPeer> peer;
		std::unique_ptr<VirtualKeyboardPeer> keyboardPeer;
	};

	TEST_F(BindingUiTest, Then_WstringLabelTextIsSet)
	{
		ASSERT_EQ(model->wstringLabel.value(), *peer->wstringLabelPeer.control.text);
	}

	TEST_F(BindingUiTest, When_WstringLabelPropertyIsUpdated_Then_WstringLabelTextIsUpdated)
	{
		model->wstringLabel = L"new text";
		ASSERT_EQ(model->wstringLabel.value(), *peer->wstringLabelPeer.control.text);
	}

	TEST_F(BindingUiTest, Then_StringLabelTextIsSet)
	{
		ASSERT_EQ(Philips::Workflow::convert(model->stringLabel.value()), *peer->stringLabelPeer.control.text);
	}

	TEST_F(BindingUiTest, When_StringLabelPropertyIsUpdated_Then_StringLabelTextIsUpdated)
	{
		model->stringLabel = "new text";
		ASSERT_EQ(Philips::Workflow::convert(model->stringLabel.value()), *peer->stringLabelPeer.control.text);
	}

	TEST_F(BindingUiTest, Then_IntLabelVisiblePropertyIsSetToTrue)
	{
		ASSERT_TRUE(*peer->intLabelPeer.control.visible);
		ASSERT_FALSE(*peer->stringLabelPeer.control.visible);
	}

	TEST_F(BindingUiTest, When_LabelTypePropertyIsChangedToString_Then_StringLabelVisiblePropertyIsSetToTrue)
	{
		model->labelType = ViewModel::LabelType::String;
		ASSERT_TRUE(*peer->stringLabelPeer.control.visible);
		ASSERT_FALSE(*peer->intLabelPeer.control.visible);
	}

	TEST_F(BindingUiTest, When_LabelTypePropertyIsChangedToInt_Then_IntStringLabelVisiblePropertyIsSetToTrue)
	{
		model->labelType = ViewModel::LabelType::Int;
		ASSERT_TRUE(*peer->intStringLabelPeer.control.visible);
	}

	TEST_F(BindingUiTest, When_LabelTypePropertyIsChangedToString_Then_IntStringLabelVisiblePropertyIsSetToTrue)
	{
		model->labelType = ViewModel::LabelType::String;
		ASSERT_TRUE(*peer->intStringLabelPeer.control.visible);
	}

	TEST_F(BindingUiTest, When_LabelTypePropertyIsChangedToUnknown_Then_IntStringLabelVisiblePropertyIsSetToFalse)
	{
		model->labelType = ViewModel::LabelType::Unknown;
		ASSERT_FALSE(*peer->intStringLabelPeer.control.visible);
	}
	
	TEST_F(BindingUiTest, Then_IntLabelTextIsSet)
	{
		ASSERT_EQ(std::to_wstring(model->intLabel.value()), *peer->intLabelPeer.control.text);
	}

	TEST_F(BindingUiTest, When_IntLabelPropertyIsUpdated_Then_IntsLabelTextIsUpdated)
	{
		model->intLabel = 100;
		ASSERT_EQ(std::to_wstring(model->intLabel.value()), *peer->intLabelPeer.control.text);
	}

	TEST_F(BindingUiTest, Then_WstringTouchEditBoxTextIsSet)
	{
		ASSERT_EQ(model->wstringTouchEditBox.value(), *peer->wstringTouchEditBoxPeer.control.text);
	}

	TEST_F(BindingUiTest, When_TextIsEnteredInWstringTouchEditBoxText_Then_WstringTouchEditBoxTextPropertyIsUpdated)
	{
		peer->wstringTouchEditBoxPeer.clearAll();
		ASSERT_TRUE(model->wstringTouchEditBox.value().empty());
	}

	TEST_F(BindingUiTest, When_WstringTouchEditBoxTextPropertyIsUpdated_Then_WstringTouchEditBoxTextIsUpdated)
	{
		model->wstringTouchEditBox = L"new text";
		ASSERT_EQ(model->wstringTouchEditBox.value(), *peer->wstringTouchEditBoxPeer.control.text);
	}

	TEST_F(BindingUiTest, Then_StringTouchEditBoxTextIsSet)
	{
		ASSERT_EQ(Philips::Workflow::convert(model->stringTouchEditBox.value()), *peer->stringTouchEditBoxPeer.control.text);
	}

	TEST_F(BindingUiTest, When_TextIsEnteredInStringTouchEditBox_Then_StringTouchEditBoxPropertyIsUpdated)
	{
		peer->stringTouchEditBoxPeer.clearAll();
		ASSERT_TRUE(model->stringTouchEditBox.value().empty());
	}

	TEST_F(BindingUiTest, When_StringTouchEditBoxTextPropertyIsUpdated_Then_StringTouchEditBoxTextIsUpdated)
	{
		model->stringTouchEditBox = "new text";
		ASSERT_EQ(Philips::Workflow::convert(model->stringTouchEditBox.value()), *peer->stringTouchEditBoxPeer.control.text);
	}

	TEST_F(BindingUiTest, Then_IntTouchEditBoxTextIsSet)
	{
		ASSERT_EQ(std::to_wstring(model->intTouchEditBox.value()), *peer->intTouchEditBoxPeer.control.text);
	}

	TEST_F(BindingUiTest, When_TextIsEnteredInIntTouchEditBoxText_Then_IntTouchEditBoxTextPropertyIsUpdated)
	{
		peer->intTouchEditBoxPeer.clearAll();
		ASSERT_EQ(model->intTouchEditBox.value(), 0);
	}

	TEST_F(BindingUiTest, When_IntTouchEditBoxTextPropertyIsUpdated_Then_IntTouchEditBoxTextIsUpdated)
	{
		model->intTouchEditBox = 77;
		ASSERT_EQ(std::to_wstring(model->intTouchEditBox.value()), *peer->intTouchEditBoxPeer.control.text);
	}

	TEST_F(BindingUiTest, Then_DoubleTouchEditBoxTextIsSet)
	{
		std::stringstream stream;
		stream << std::fixed <<std::setprecision(1) <<2.0;
		ASSERT_EQ(Philips::Workflow::convert(stream.str()), *peer->doubleTouchEditBoxPeer.control.text);
	}

	TEST_F(BindingUiTest, When_TextIsEnteredInDoubleTouchEditBoxText_Then_DoubleTouchEditBoxTextPropertyIsUpdated)
	{
		peer->doubleTouchEditBoxPeer.clearAll();
		ASSERT_EQ(model->doubleTouchEditBox.value(), 0.0);
	}

	TEST_F(BindingUiTest, When_DoubleTouchEditBoxTextPropertyIsUpdated_Then_DoubleTouchEditBoxTextIsUpdated)
	{
		std::stringstream stream;
		stream << std::fixed << std::setprecision(1) << 10.77;
		model->doubleTouchEditBox = 10.77;
		
		ASSERT_EQ(Philips::Workflow::convert(stream.str()), *peer->doubleTouchEditBoxPeer.control.text);
	}

	TEST_F(BindingUiTest, When_DoubleTouchEditBoxTextPropertyIsUpdatedForPrecision_Then_DoubleTouchEditBoxTextIsUpdated)
	{
		std::stringstream stream;
		stream << std::fixed << std::setprecision(3) << 10.77777;
		model->doubleTouchPrecisionEditBox = 10.77777;
		ASSERT_EQ(Philips::Workflow::convert(stream.str()), *peer->doubleTouchPrecisionEditBox.control.text);
	}

	TEST_F(BindingUiTest, Then_wstringOnewayTouchEditBoxTextIsSet)
	{
		ASSERT_EQ(model->wstringOnewayTouchEditBox.value(), *peer->wstringOnewayTouchEditBoxPeer.control.text);
	}

	TEST_F(BindingUiTest, When_TextIsEnteredInWstringOnewayTouchEditBox_Then_wstringOnewayTouchEditBoxPropertyIsNotUpdated)
	{
		peer->wstringOnewayTouchEditBoxPeer.clearAll();
		ASSERT_FALSE(model->wstringOnewayTouchEditBox.value().empty());
	}

	TEST_F(BindingUiTest, When_OnewayWstringTouchEditBoxTextPropertyIsUpdated_Then_OnewayWstringTouchEditBoxTextIsUpdated)
	{
		model->wstringOnewayTouchEditBox = L"new oneway text";
		ASSERT_EQ(model->wstringOnewayTouchEditBox.value(), *peer->wstringOnewayTouchEditBoxPeer.control.text);
	}

	TEST_F(BindingUiTest, Then_StringOnewayTouchEditBoxTextIsSet)
	{
		ASSERT_EQ(Philips::Workflow::convert(model->stringOnewayTouchEditBox.value()), *peer->stringOnewayTouchEditBoxPeer.control.text);
	}

	TEST_F(BindingUiTest, When_TextIsEnteredInStringOnewayTouchEditBox_Then_StringOnewayTouchEditBoxPropertyIsNotUpdated)
	{
		peer->stringOnewayTouchEditBoxPeer.clearAll();
		ASSERT_FALSE(model->stringOnewayTouchEditBox.value().empty());
	}

	TEST_F(BindingUiTest, When_OnewayStringTouchEditBoxTextPropertyIsUpdated_Then_OnewayStringTouchEditBoxTextIsUpdated)
	{
		model->stringOnewayTouchEditBox = "new oneway text";
		ASSERT_EQ(Philips::Workflow::convert(model->stringOnewayTouchEditBox.value()), *peer->stringOnewayTouchEditBoxPeer.control.text);
	}

	TEST_F(BindingUiTest, Then_RadioButton1CheckedIsSet)
	{
		ASSERT_EQ(model->radiobutton1Checked.value(), *peer->radioButton1Peer.control.checked);
	}

	TEST_F(BindingUiTest, Then_RadioButton2CheckedIsSet)
	{
		ASSERT_EQ(!model->radiobutton1Checked.value(), *peer->radioButton2Peer.control.checked);
	}

	TEST_F(BindingUiTest, Then_RadioButton3CheckedIsSet)
	{
		ASSERT_EQ(model->radiobutton3Checked.value(), *peer->radioButton3Peer.control.checked);
	}

	TEST_F(BindingUiTest, Then_CheckBoxCheckedIsSet)
	{
		ASSERT_EQ(model->disableCheckBox.value(), *peer->disableCheckBoxPeer.control.checked);
		ASSERT_EQ(model->hideCheckBox.value(), *peer->hideCheckBoxPeer.control.checked);
	}

	TEST_F(BindingUiTest, Then_FaderPageSelectIsSet)
	{
		ASSERT_EQ(model->selectFaderPage.value(), peer->isFaderPage1Selected());
	}

	TEST_F(BindingUiTest, Then_FaderPage2SelectIsSet)
	{
		ASSERT_EQ(!model->selectFaderPage.value(), peer->isFaderPage2Selected());
	}

	TEST_F(BindingUiTest, Then_RadioButton3VisibleIsSetCorrectly)
	{
		ASSERT_EQ(model->visibleRadioButton3.value(), *peer->radioButton3Peer.control.visible);
	}

	TEST_F(BindingUiTest, Then_RadioButton3EnabledIsSetCorrectly)
	{
		ASSERT_EQ(model->enableRadioButton3.value(), *peer->radioButton3Peer.control.enabled);
	}

	TEST_F(BindingUiTest, When_VisibleRadioButton3PropertyIsUpdated_Then_RadioButton3VisibleIsSetCorrectly)
	{
		model->visibleRadioButton3 = false;
		ASSERT_FALSE(*peer->radioButton3Peer.control.visible);
	}

	TEST_F(BindingUiTest, When_EnableRadioButton3PropertyIsUpdated_Then_RadioButton3VisibleIsSetCorrectly)
	{
		model->enableRadioButton3 = false;
		ASSERT_FALSE(*peer->radioButton3Peer.control.enabled);
	}

	TEST_F(BindingUiTest, When_ViewRadioButton1IsChecked_Then_PropertyIsUpdated)
	{
		peer->radioButton1Peer.check();
		ASSERT_TRUE(model->radiobutton1Checked.value());
	}

	TEST_F(BindingUiTest, When_ViewRadioButton1IsChecked_Then_RadioButton2IsUpdated)
	{
		peer->radioButton1Peer.check();
		ASSERT_FALSE(peer->radioButton2Peer.control.checked);
	}

	TEST_F(BindingUiTest, When_RadioButton1PropertyIsUpdated_Then_ViewRadioButton1IsChecked)
	{
		model->radiobutton1Checked = true;
		ASSERT_TRUE(peer->radioButton1Peer.control.checked);
	}

	TEST_F(BindingUiTest, When_RadioButton1PropertyIsUpdated_Then_ViewRadioButton2IsUnchecked)
	{
		model->radiobutton1Checked = true;
		ASSERT_FALSE(peer->radioButton2Peer.control.checked);
	}

	TEST_F(BindingUiTest, When_ViewRadioButton3IsUnChecked_Then_PropertyIsNotUpdated)
	{
		peer->radioButton1Peer.check();
		ASSERT_NE(model->radiobutton1Checked.value(), *peer->radioButton3Peer.control.checked);
	}

	TEST_F(BindingUiTest, When_ViewCheckBoxDisableIsChecked_Then_PropertyIsUpdated)
	{
		peer->disableCheckBoxPeer.check();
		ASSERT_TRUE(model->disableCheckBox.value());
	}

	TEST_F(BindingUiTest, When_ViewCheckBoxDisableIsChecked_Then_RadioButton1IsDisabled)
	{
		peer->disableCheckBoxPeer.check();
		ASSERT_FALSE(peer->radioButton1Peer.control.enabled);
	}

	TEST_F(BindingUiTest, When_LabelTypePropertyIsChangedToInt_Then_IntStringLabelEnabledPropertyIsSetToTrue)
	{
		model->labelType = ViewModel::LabelType::Int;
		ASSERT_TRUE(*peer->intStringLabelPeer.control.enabled);
	}

	TEST_F(BindingUiTest, When_LabelTypePropertyIsChangedToString_Then_IntStringLabelEnabledPropertyIsSetToTrue)
	{
		model->labelType = ViewModel::LabelType::String;
		ASSERT_TRUE(*peer->intStringLabelPeer.control.enabled);
	}

	TEST_F(BindingUiTest, When_LabelTypePropertyIsChangedToUnknown_Then_IntStringLabelEnabledPropertyIsSetToFalse)
	{
		model->labelType = ViewModel::LabelType::Unknown;
		ASSERT_FALSE(*peer->intStringLabelPeer.control.enabled);
	}

	TEST_F(BindingUiTest, When_ViewOneWayCheckBoxShowIsChecked_Then_PropertyIsNotUpdated)
	{
		peer->oneWayCheckBoxPeer.check();
		ASSERT_FALSE(model->oneWayCheckBox.value());
	}

	TEST_F(BindingUiTest, When_ViewCheckBoxHideIsChecked_Then_PropertyIsUpdated)
	{
		peer->hideCheckBoxPeer.check();
		ASSERT_TRUE(model->hideCheckBox.value());
	}

	TEST_F(BindingUiTest, When_ViewCheckBoxHideIsChecked_Then_RadioButton2IsNotVisible)
	{
		peer->hideCheckBoxPeer.check();
		ASSERT_FALSE(peer->radioButton2Peer.control.visible);
	}

	TEST_F(BindingUiTest, When_TabPagePropertyIsChangedToListsPage_Then_ListsPageIsSelected)
	{
		model->tabPage = ViewModel::Pages::ListsPage;
		ASSERT_TRUE(peer->isListsPageSelected());
		ASSERT_FALSE(peer->isStaticsPageSelected());
	}
	
	TEST_F(BindingUiTest, When_ListsPageIsClicked_Then_PropertyValueIsChangedToListsPage)
	{
		peer->listsPagePeer.select();
		ASSERT_EQ(model->tabPage.value(), ViewModel::Pages::ListsPage);
	}

	TEST_F(BindingUiTest, When_SelectFaderPageIsTrue_Then_FaderPage1IsSelected)
	{
		model->selectFaderPage = false;
		ASSERT_FALSE(peer->isFaderPage1Selected());
	}

	TEST_F(BindingUiTest, When_SelectFaderPageIsTrue_Then_FaderPage2IsNotSelected)
	{
		model->selectFaderPage = false;
		ASSERT_TRUE(peer->isFaderPage2Selected());
	}
	
	TEST_F(BindingUiTest, When_ButtonIsClicked_Then_CommandIsExecuted)
	{
		peer->buttonPeer.mouseClick();
		ASSERT_TRUE(model->wstringTouchEditBox.value().empty());
		ASSERT_TRUE(model->stringTouchEditBox.value().empty());
		ASSERT_EQ(model->intTouchEditBox.value(), 0);
	}

	TEST_F(BindingUiTest, When_CommandIsExecuted_Then_ButtonIsDisabled)
	{
		peer->buttonPeer.mouseClick();
		ASSERT_FALSE(peer->buttonPeer.control.enabled);
	}

	TEST_F(BindingUiTest, Given__When_CanExecuteIsSetToFalse_Then_ButtonIsDisabled)
	{
		model->button.canExecute = false;
		
		ASSERT_FALSE(peer->buttonPeer.control.enabled);
	}

	TEST_F(BindingUiTest, Then_ComboBoxStringItemsAreSet)
	{
		const auto expected = model->stringComboBox.values();
		const auto current = peer->stringComboBoxPeer.control.getItems();
		ASSERT_EQ(expected.size(), current.size());
		for(auto i=0; i< static_cast<int>(expected.size()); i++)
		{
			ASSERT_EQ(Philips::Workflow::convert(expected[i]), current[i].text);
		}
	}

	TEST_F(BindingUiTest, When_CollectionValuesAreSet_Then_ComboBoxStringItemsAreUpdated)
	{
		const std::vector<std::string> newValues = { "value1", "value2" };
		model->stringComboBox.set(newValues);
		const auto current = peer->stringComboBoxPeer.control.getItems();
		ASSERT_EQ(newValues.size(), current.size());
		for (auto i = 0; i < static_cast<int>(newValues.size()); i++)
		{
			ASSERT_EQ(Philips::Workflow::convert(newValues[i]), current[i].text);
		}
	}

	TEST_F(BindingUiTest, When_ComboBoxSelectionIsChanged_Then_CollectionSelectionIsChanged)
	{
		peer->listsPagePeer.select();
		const auto selection{ 1 };
		peer->stringComboBoxPeer.select(selection);
		ASSERT_EQ(model->stringComboBox.selection(), selection);
	}

	TEST_F(BindingUiTest, Then_OnewayComboBoxStringItemsAreSet)
	{
		const auto expected = model->oneWayComboBox.values();
		const auto current = peer->onewayComboBoxPeer.control.getItems();
		ASSERT_EQ(expected.size(), current.size());
		for (auto i = 0; i < static_cast<int>(expected.size()); i++)
		{
			ASSERT_EQ(Philips::Workflow::convert(expected[i]), current[i].text);
		}
	}

	TEST_F(BindingUiTest, When_OnewayCollectionValuesAreSet_Then_OnewayComboBoxStringItemsAreUpdated)
	{
		const std::vector<std::string> newValues = { "value1", "value2" };
		model->oneWayComboBox.set(newValues);
		const auto current = peer->onewayComboBoxPeer.control.getItems();
		ASSERT_EQ(newValues.size(), current.size());
		for (auto i = 0; i < static_cast<int>(newValues.size()); i++)
		{
			ASSERT_EQ(Philips::Workflow::convert(newValues[i]), current[i].text);
		}
	}

	TEST_F(BindingUiTest, When_OnewayComboBoxSelectionIsChanged_Then_OnewayComboBoxPropertySelectionIsChanged)
	{
		peer->listsPagePeer.select();
		const auto selection{ 1 };
		peer->onewayComboBoxPeer.select(selection);
		ASSERT_NE(model->oneWayComboBox.selection(), selection);
	}

	TEST_F(BindingUiTest, Then_ComboBoxCustomItemsAreSet)
	{
		const auto expected = model->customComboBox.values();
		const auto current = peer->customComboBoxPeer.control.getItems();
		ASSERT_EQ(expected.size(), current.size());
		for (auto i = 0; i < static_cast<int>(expected.size()); i++)
		{
			ASSERT_EQ(Philips::Workflow::convert(expected[i].name), current[i].text);
			ASSERT_EQ(expected[i].status, current[i].enabled);
		}
	}

	TEST_F(BindingUiTest, When_ComboBoxCustomSelectionIsChanged_Then_CollectionSelectionIsChanged)
	{
		peer->listsPagePeer.select();
		const auto selection{ 1 };
		peer->customComboBoxPeer.select(selection);
		ASSERT_EQ(model->customComboBox.selection(), selection);
	}

	TEST_F(BindingUiTest, Then_ListViewCustomItemsAreSet)
	{
		const auto expected = model->customList.values();
		const auto current = peer->customListViewPeer.control.getItems();
		ASSERT_EQ(expected.size(), current.size());
		for (auto i = 0; i < static_cast<int>(expected.size()); i++)
		{
			ASSERT_EQ(Philips::Workflow::convert(expected[i].name), current[i].cells[0].text);
		}
	}

	TEST_F(BindingUiTest, Then_MultiSelectListViewCustomItemsAreSet)
	{
		const auto expected = model->customMultiSelectableList.values();
		const auto current = peer->customMultiSelectableListViewPeer.control.getItems();
		ASSERT_EQ(expected.size(), current.size());
		for (auto i = 0; i < static_cast<int>(expected.size()); i++)
		{
			ASSERT_EQ(Philips::Workflow::convert(expected[i].name), current[i].cells[0].text);
		}
	}

	TEST_F(BindingUiTest, When_AllMultiSelectListViewCustomItemsAreSelected_Then_ListViewSelectionIsSetForAllItems)
	{
		const auto expected = model->customMultiSelectableList.values();
		model->customMultiSelectableList.selectAll();
		const auto selections = peer->customMultiSelectableListViewPeer.control.getSelection();
		std::vector<ListView::Item> current;
		for (auto selection : selections)
		{
			current.emplace_back(peer->customMultiSelectableListViewPeer.control.getItem(selection));
		}
		ASSERT_EQ(expected.size(), current.size());
		for (auto i = 0; i < static_cast<int>(expected.size()); i++)
		{
			ASSERT_EQ(Philips::Workflow::convert(expected[i].name), current[i].cells[0].text);
		}
	}

	TEST_F(BindingUiTest, When_MultiSelectListViewCustomSelectionIsChanged_Then_ListViewCustomSelectionIsChanged)
	{
		peer->listsPagePeer.select();
		const std::set<int> selection = { 0 , 1, 2 };
		
		for (auto value : selection)
		{
			peer->customMultiSelectableListViewPeer.select(value);
		}
		ASSERT_EQ(model->customMultiSelectableList.selection(), selection);
	}

	TEST_F(BindingUiTest, When_OneWayMultiSelectListViewCustomSelectionIsChanged_Then_ListViewCustomSelectionIsNotChanged)
	{
		peer->listsPagePeer.select();
		const std::set<int> selection = { 2 };
		const std::set<int> newSelection = { 0 , 1 };

		model->customOneWayMultiSelectableList.select(selection);

		for (auto value : newSelection)
		{
			peer->customOneWayMultiSelectableListViewPeer.select(value);
		}
		
		ASSERT_NE(model->customOneWayMultiSelectableList.selection(), newSelection);
		ASSERT_EQ(model->customOneWayMultiSelectableList.selection(), selection);
	}

	TEST_F(BindingUiTest, When_SelectAllOneWayMultiSelectListViewCustomItemsIsCalled_Then_ListViewSelectionIsSetForAllItems)
	{
		const auto expected = model->customOneWayMultiSelectableList.values();
		model->customOneWayMultiSelectableList.selectAll();
		const auto selections = peer->customOneWayMultiSelectableListViewPeer.control.getSelection();
		std::vector<ListView::Item> current;
		for (auto selection : selections)
		{
			current.emplace_back(peer->customOneWayMultiSelectableListViewPeer.control.getItem(selection));
		}
		ASSERT_EQ(expected.size(), current.size());
		for (auto i = 0; i < static_cast<int>(expected.size()); i++)
		{
			ASSERT_EQ(Philips::Workflow::convert(expected[i].name), current[i].cells[0].text);
		}
	}

	TEST_F(BindingUiTest, Given_AllOneWayMultiSelectListViewCustomItemsAreSelected_When_DeselectAllOneWayMultiSelectListViewCustomItemsIsCalled_Then_ListViewSelectionIsSetForAllItems)
	{
		model->customOneWayMultiSelectableList.selectAll();
		model->customOneWayMultiSelectableList.deselectAll();
		const auto current = peer->customOneWayMultiSelectableListViewPeer.control.getSelection();
		ASSERT_EQ(current.size(), 0);
	}

	TEST_F(BindingUiTest, When_ButtonAccentPropertyIsUpdated_Then_RadioButton1AndRadioButton2AccentIsSetCorrectly)
	{
		model->buttonAccent = true;
		ASSERT_TRUE(*peer->radioButton1Peer.control.accent);
		ASSERT_FALSE(*peer->radioButton2Peer.control.accent);
	}
	
	TEST_F(BindingUiTest, Given_EditableComboBox_When_EditableSelectableCollectionValuesIsSet_Then_EditableComboBoxItemsAreSetSameWithValues)
	{
		peer->listsPagePeer.select();
		const auto expected = model->twoWayEditableComboBox.values();
		const auto current = view->twowayEditableComboBox.getItems();
		ASSERT_EQ(expected.size(), current.size());
		for (auto i = 0; i < static_cast<int>(expected.size()); i++)
		{
			ASSERT_EQ(Philips::Workflow::convert(expected[i]), current[i].text);
		}
	}

	TEST_F(BindingUiTest, Given_EditableComboBox_Then_VerifyEditableComboBoxSelectionIsSetToDefaultSelection)
	{
		peer->listsPagePeer.select();
		ASSERT_EQ(view->twowayEditableComboBox.getSelectedItem(), -1);
	}

	TEST_F(BindingUiTest, Given_EditableComboBox_Then_VerifyEditableSelectableCollectionSelectionIsSetToDefaultSelection)
	{
		peer->listsPagePeer.select();
		ASSERT_EQ(model->twoWayEditableComboBox.selection(), -1);
	}

	TEST_F(BindingUiTest, Given_EditableComboBox_When_EditableComboBoxSelectionIsChanged_Then_VerifyEditableSelectableCollectionValueIsChangedToSelectedItemValue)
	{
		peer->listsPagePeer.select();
		const auto selection{ 1 };
		model->twoWayEditableComboBox.select(selection);
		const auto current = peer->twowayEditableComboBoxPeer.control.getItems();
		ASSERT_EQ(model->twoWayEditableComboBox.editableValue.value(), Philips::Workflow::convert(current[selection].text));
	}

	TEST_F(BindingUiTest, Given_EditableComboBoxWhen_When_EditableSelectableCollectionValueIsChanged_VerifyThen_EditableComboBoxSelectionIsChangedToSameValue)
	{
		peer->listsPagePeer.select();
		const std::string value = "value";
		model->twoWayEditableComboBox.editableValue = value;
		std::wstring currentValue = view->twowayEditableComboBox.editBox.text;
		ASSERT_EQ(Philips::Workflow::convert(value), currentValue);
	}

	TEST_F(BindingUiTest, Given_EditableComboBoxWhen_When_EditableComboBoxEditBoxValueIsChanged_Then_VerifyTwowayComboBoxPropertyIsChangedToSame)
	{
		peer->listsPagePeer.select();
		const std::string value = "value";
		peer->twowayEditableComboBoxPeer.insertText(Philips::Workflow::convert(value));
		std::string currentValue = model->twoWayEditableComboBox.editableValue.value();
		ASSERT_EQ(value, currentValue);
	}

	TEST_F(BindingUiTest, Given_EditableComboBoxWhen_When_EditableComboBoxEditBoxValueIsChangedWithAValueInComboBoxList_Then_VerifyTwowayComboBoxIsSetSameSelectionAndValue)
	{
		peer->listsPagePeer.select();
		const auto selection{ 1 };
		const auto value = peer->twowayEditableComboBoxPeer.control.getItems();
		peer->twowayEditableComboBoxPeer.insertText(value[selection].text);
		ASSERT_EQ(model->twoWayEditableComboBox.selection(), selection);
		ASSERT_EQ(model->twoWayEditableComboBox.editableValue.value(), Philips::Workflow::convert(value[selection].text));
	}

	TEST_F(BindingUiTest, Given_EditableComboBoxWhen_When_EditableComboBoxEditBoxValueIsChangedWithAValueNotInComboBoxList_Then_VerifyTwowayComboBoxIsSetToSameValueAndDefaultSelection)
	{
		peer->listsPagePeer.select();
		const auto value = peer->twowayEditableComboBoxPeer.control.getItems();
		peer->twowayEditableComboBoxPeer.insertText(L"new");
		ASSERT_EQ(model->twoWayEditableComboBox.selection(), -1);
		ASSERT_EQ(model->twoWayEditableComboBox.editableValue.value(), "new");
	}

	TEST_F(BindingUiTest, Given_EditableComboBoxWhen_When_EditableComboBoxSelectionIsChangedWithAValueInComboBoxList_Then_VerifyTwowayComboBoxIsSetSameSelectionAndValue)
	{
		peer->listsPagePeer.select();
		view->twowayEditableComboBox.setSelectedItem(0);
		ASSERT_EQ(model->twoWayEditableComboBox.selection(), 0);
		ASSERT_EQ(model->twoWayEditableComboBox.values()[0], Philips::Workflow::convert(view->twowayEditableComboBox.editBox.text));
	}

	TEST_F(BindingUiTest, Given_EditableComboBoxWhen_When_EditableComboBoxSelectionIsChangedWithAValueNotInComboBoxList_Then_VerifyTwowayComboBoxIsSetToSameValueAndDefaultSelection)
	{
		peer->listsPagePeer.select();
		view->twowayEditableComboBox.setSelectedItem(4);
		ASSERT_EQ(model->twoWayEditableComboBox.selection(), -1);
		ASSERT_EQ("", Philips::Workflow::convert(view->twowayEditableComboBox.editBox.text));
	}

	TEST_F(BindingUiTest, Given_EditableComboBoxWhen_When_TwowayComboBoxValueIsChangedWithAValueInComboBoxList_Then_VerifyEditableComboBoxIsSetSameSelectionAndValue)
	{
		peer->listsPagePeer.select();
		const auto selection{ 1 };
		const auto value = peer->twowayEditableComboBoxPeer.control.getItems();
		model->twoWayEditableComboBox.editableValue = Philips::Workflow::convert(value[selection].text);

		ASSERT_EQ(view->twowayEditableComboBox.getSelectedItem(), selection);
		ASSERT_EQ(Philips::Workflow::convert(view->twowayEditableComboBox.editBox.text), model->twoWayEditableComboBox.editableValue.value());
	}

	TEST_F(BindingUiTest, Given_EditableComboBoxWhen_When_TwowayComboBoxValueIsChangedWithAValueNotInComboBoxList_Then_VerifyEditableComboBoxIsSetToSameValueAndDefaultSelection)
	{
		peer->listsPagePeer.select();
		model->twoWayEditableComboBox.editableValue = "new";
		ASSERT_EQ(view->twowayEditableComboBox.getSelectedItem(), -1);
		ASSERT_EQ(Philips::Workflow::convert(view->twowayEditableComboBox.editBox.text), "new");
	}

	TEST_F(BindingUiTest, Given_EditableComboBoxWhen_When_TwowayComboBoxSelectionIsChangedWithAValueInComboBoxList_Then_VerifyEditableComboBoxIsSetSameSelectionAndValue)
	{
		model->twoWayEditableComboBox.select(0);
		ASSERT_EQ(view->twowayEditableComboBox.getSelectedItem(), 0);
		ASSERT_EQ(model->twoWayEditableComboBox.editableValue.value(), Philips::Workflow::convert(view->twowayEditableComboBox.editBox.text));
	}

	TEST_F(BindingUiTest, Given_EditableComboBoxWhen_When_TwowayComboBoxSelectionIsChangedWithAValueNotInComboBoxList_Then_VerifyEditableComboBoxIsSetToSameValueAndDefaultSelection)
	{
		peer->listsPagePeer.select();
		model->twoWayEditableComboBox.select(4);
		ASSERT_EQ(view->twowayEditableComboBox.getSelectedItem(), -1);
		ASSERT_EQ("", Philips::Workflow::convert(view->twowayEditableComboBox.editBox.text));
	}

	TEST_F(BindingUiTest, Given_EditableComboBoxWhen_When_TwowayComboBoxCollectionIsSetToNewCollection_Then_EditableComboBoxValuesIsAlsoUpdated)
	{
		std::vector<std::string> values1{ "1" };
		Binding::EditableSelectableCollection<std::string> collection(values1);
		collection.select(0);

		std::vector<std::string> values2{ "1","2" };
		model->twoWayEditableComboBox.set(values2);
		
		const auto expected = model->oneWayComboBox.values();
		const auto current = peer->onewayComboBoxPeer.control.getItems();
		for (auto i = 0; i < static_cast<int>(expected.size()); i++)
		{
			ASSERT_EQ(Philips::Workflow::convert(expected[i]), current[i].text);
		}
	}

	TEST_F(BindingUiTest, Given_BindCommandWithVisibility_Then_ButtonVisibilityIsTrueByDefault)
	{
		ASSERT_TRUE(peer->buttonVisibilityPeer.control.isVisible());
	}

	TEST_F(BindingUiTest, Given_BindCommandWithVisibility_When_CommandCanExecuteChangesToFalse_Then_ButtonVisibilityIsFalse)
	{
		model->visibilityButton.canExecute = false;
		ASSERT_FALSE(peer->buttonVisibilityPeer.control.isVisible());
	}

	TEST_F(BindingUiTest, Given_BindImage_Then_DefaultImageIsDisplayed)
	{
		ASSERT_EQ(peer->imageBoxPeer.control.image, Sense::Image());
	}

	TEST_F(BindingUiTest, Given_BindImage_When_ImagePropertyChanges_Then_ImageUpdatedInView)
	{
		model->image = Icons::Airplane_24;
		ASSERT_EQ(peer->imageBoxPeer.control.image, Icons::Airplane_24);
	}

	TEST_F(BindingUiTest, Given_BindVisibleWithIsNotEmptyConverterForString_When_StringIsEmpty_Then_ControlNotVisible)
	{
		peer->listsPagePeer.select();
		model->visibilityLabel.update("");

		ASSERT_FALSE(peer->visibilityLabelPeer.control.isVisible());
	}

	TEST_F(BindingUiTest, Given_BindVisibleWithIsNotEmptyConverterForString_When_StringHasValue_Then_ControlVisible)
	{
		peer->listsPagePeer.select();
		model->visibilityLabel = "value";

		ASSERT_TRUE(peer->visibilityLabelPeer.control.isVisible());
	}

	TEST_F(BindingUiTest, Given_BindVisibleWithIsNotEmptyConverterForCollection_When_CollectionIsEmpty_Then_ControlNotVisible)
	{
		peer->listsPagePeer.select();
		const std::vector<std::string> values;
		model->twoWayEditableComboBox.set(values);

		ASSERT_FALSE(peer->twowayEditableComboBoxPeer.control.isVisible());
	}

	TEST_F(BindingUiTest, Given_BindVisibleWithIsNotEmptyConverterForCollection_When_CollectionHasValues_Then_ControlVisible)
	{
		peer->listsPagePeer.select();
		const std::vector<std::string> values{ "1","2" };
		model->twoWayEditableComboBox.set(values);
		
		ASSERT_TRUE(peer->twowayEditableComboBoxPeer.control.isVisible());
	}

	TEST_F(BindingUiTest, Given_BindImageWithValueToImageConverter_Then_DefaultImageIsDisplayed)
	{
		ASSERT_EQ(peer->customImageBoxPeer.control.image, Icons::Bookmark);
	}

	TEST_F(BindingUiTest, Given_BindImageWithValueToImageConverter_When_ValuePropertyChanges_Then_ImageUpdatedInView)
	{
		model->customImage = ViewModel::LabelType::String;
		ASSERT_EQ(peer->customImageBoxPeer.control.image, Icons::Airplane_24);
	}


	TEST_F(BindingUiTest, When_LabelVisiblePropertyIsUpdated_Then_IntLabelVisibleIsChanged)
	{
		model->labelVisible = true;
		ASSERT_TRUE(*peer->intLabelPeer.control.visible);
	}

	TEST_F(BindingUiTest, When_LabelVisiblePropertyIsUpdated_Then_StringLabelVisibleIsChanged)
	{
		model->labelVisible = true;
		ASSERT_FALSE(*peer->stringLabelPeer.control.visible);
	}
}}