// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "view.h"
#include "Workflow/binding.h"

namespace Test { namespace Binding {
	
	struct CustomCollectionToComboBoxItems
	{
		std::vector<ComboBox::Item> operator()(const Collection<ViewModel::CustomItem>::Values& values) const
		{
			std::vector<ComboBox::Item> items;
			items.reserve(values.size());
			for (auto& value : values)
			{
				ComboBox::Item item;
				item.text = Philips::Workflow::convert(value.name);
				item.enabled = value.status;
				items.emplace_back(item);
			}
			return items;
		}
	};

	struct CustomCollectionToListViewItems
	{
		std::vector<ListView::Item> operator()(const Collection<ViewModel::CustomItem>::Values& values) const
		{
			std::vector<ListView::Item> items;
			items.reserve(values.size());
			for (auto& value : values)
			{
				std::vector<ListViewItem::Cell> cell(3);
				cell[0].image = value.isDefault ? Icons::Star : Image();
				cell[0].text = Philips::Workflow::convert(value.name);
				cell[1].image = value.status ? Icons::Checkmark : Icons::CheckmarkError;
				cell[1].text = L"";

				ListView::Item item(cell);
				items.emplace_back(item);
			}
			return items;
		}
	};

	View::View(Control& parent, VirtualKeyboard& keyboard, ViewModel& model)
	: Ui::View(parent, keyboard)
	{
		bind(intLabel.visible, model.labelVisible);
		bind(stringLabel.visible, model.labelVisible, Invert());
		
		bind_text(headerLabel, model.headerLabel);
		bind_text(staticsPage, model.staticsPage);
		bind_text(listsPage, model.listsPage);
		
		bind_text(wstringLabel, model.wstringLabel);
		bind_text(stringLabel, model.stringLabel, StringToWstring());
		bind_text(intLabel, model.intLabel, IntToWstring());

		bind_visible(intLabel, model.labelType, ValueToBool<ViewModel::LabelType>(ViewModel::LabelType::Int));
		bind_visible(stringLabel, model.labelType, ValueToBool<ViewModel::LabelType>(ViewModel::LabelType::String));
		bind_visible(intStringLabel, model.labelType, ValuesToBool<ViewModel::LabelType>({ ViewModel::LabelType::Int, ViewModel::LabelType::String }));
		
		bind_text(wstringTouchEditBox, model.wstringTouchEditBox, Philips::Workflow::Binding::Direction::TwoWay);
		bind_text(stringTouchEditBox, model.stringTouchEditBox, StringToWstring(), WstringToString());
		bind_text(intTouchEditBox, model.intTouchEditBox, IntToWstring(), WstringToInt());
		bind_text(doubleTouchEditBox, model.doubleTouchEditBox, DoubleToWstring(), WstringToDouble());
		bind_text(doublePrecisionEditBox, model.doubleTouchPrecisionEditBox, DoubleToWstring(3), WstringToDouble());

		bind_text(wstringOnewayTouchEditBox, model.wstringOnewayTouchEditBox, Philips::Workflow::Binding::Direction::OneWay);
		bind_text(stringOnewayTouchEditBox, model.stringOnewayTouchEditBox, StringToWstring());

		bind_checked(radiobutton1, model.radiobutton1Checked, Philips::Workflow::Binding::Direction::TwoWay);
		bind_checked(radiobutton2, model.radiobutton1Checked, Invert(), Philips::Workflow::Binding::Direction::TwoWay);
		bind_checked(radiobutton3, model.radiobutton3Checked, Philips::Workflow::Binding::Direction::OneWay);

		bind_checked(disableCheckBox, model.disableCheckBox, Philips::Workflow::Binding::Direction::TwoWay);
		bind_checked(hideCheckBox, model.hideCheckBox, Philips::Workflow::Binding::Direction::TwoWay);
		
		bind_enable(radiobutton3, model.enableRadioButton3);
		bind_visible(radiobutton3, model.visibleRadioButton3);
		
		bind_enable(radiobutton1, model.disableCheckBox, Invert());
		bind_visible(radiobutton2, model.hideCheckBox, Invert());

		bind_enable(intStringLabel, model.labelType, ValuesToBool<ViewModel::LabelType>({ ViewModel::LabelType::Int, ViewModel::LabelType::String }));
		
		bind_select(faderPage1, model.selectFaderPage);
		bind_select(faderPage2, model.selectFaderPage, Invert());

		bind_select(staticsPage, model.tabPage, ValueToBool<ViewModel::Pages>(ViewModel::Pages::StaticsPage), Philips::Workflow::Binding::Direction::OneWay);
		bind_select(listsPage, model.tabPage, ValueToBool<ViewModel::Pages>(ViewModel::Pages::ListsPage), Philips::Workflow::Binding::Direction::TwoWay);

		bind_command(button, model.button);

		bind_accent(radiobutton1, model.buttonAccent);
		bind_accent(radiobutton2, model.buttonAccent, Invert());

		bind_selectableList(stringComboBox, model.stringComboBox, StringCollectionToComboBoxItems(), Philips::Workflow::Binding::Direction::TwoWay);
		bind_selectableList(customComboBox, model.customComboBox, CustomCollectionToComboBoxItems(), Philips::Workflow::Binding::Direction::TwoWay);

		bind_list(customListView, model.customList, CustomCollectionToListViewItems());
		bind_multiSelectableList(customMultiSelectableListView, model.customMultiSelectableList, CustomCollectionToListViewItems(), Philips::Workflow::Binding::Direction::TwoWay);
		bind_multiSelectableList(customOneWayMultiSelectableListView, model.customOneWayMultiSelectableList, CustomCollectionToListViewItems(), Philips::Workflow::Binding::Direction::OneWay);

		bind_selectableList(onewayComboBox, model.oneWayComboBox, StringCollectionToComboBoxItems(), Philips::Workflow::Binding::Direction::OneWay);

		bind_EditableComboBox(twowayEditableComboBox, model.twoWayEditableComboBox, StringCollectionToComboBoxItems(), StringToWstring(), WstringToString());

		bind_command(visibilityButton, model.visibilityButton, true);
		bind_image(imageBox, model.image);

		bind_visible(twowayEditableComboBox, model.twoWayEditableComboBox, IsNotEmpty<std::string>());
		bind_visible(visibilityLabel, model.visibilityLabel, IsNotEmpty<std::string>());

		bind_image(customImageBox, model.customImage, ValueToImage<ViewModel::LabelType>({ {ViewModel::LabelType::Int, Icons::Bookmark}, { ViewModel::LabelType::String, Icons::Airplane_24 } }));
	}

	
}}