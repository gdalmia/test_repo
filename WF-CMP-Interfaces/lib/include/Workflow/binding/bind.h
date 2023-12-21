// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "collection.h"
#include "command.h"
#include "direction.h"
#include "multiselectablecollection.h"
#include "property.h"
#include "selectablecollection.h"
#include "editableselectablecollection.h"

namespace Philips { namespace Workflow { namespace Binding {

	//! Bind sense property of a control to a property
	//! \params senseProperty Sense property to bind to. 
	//! \params property The source property of type value
	template <class SenseProperty, class Value>
	void bind(SenseProperty& senseProperty, Property<Value>& property)
	{
		senseProperty = property.value();
		property.changed += [&senseProperty](Value value)
		{
			senseProperty = value;
		};
	}

	//! Bind sense property of a control to a property
	//! Binds only in one direction. From source property to control
	//! \params senseProperty Sense property to bind to. 
	//! \params property The source property of type value
	//! \params converter  A Converter which converts the value type to type expected by Sense Property and vice versa
	template <class SenseProperty, class Value, class Converter>
	void bind(SenseProperty& senseProperty, Property<Value>& property, Converter converter)
	{
		senseProperty = converter(property.value());
		property.changed += [&senseProperty, converter](Value value)
		{
			senseProperty = converter(value);
		};
	}
	
	//! Bind enable of a control to a bool property
	//! Binds only in one direction. From source property to control
	//! \params control Control to bind to. Must have a member enabled.
	//! \params property The source bool property
	template <class Control>
	void bind_enable(Control& control, Property<bool>& property)
	{
		bind(control.enabled, property);
	}

	//! Bind enable of a control to a bool property
	//! Binds only in one direction. From source property to control
	//! \params control Control to bind to. Must have a member enabled.
	//! \params property The source bool property
	//! \params operation A binary operation (eg. Invert) to be operated on property value, before assigning it to control
	template <class Control, class BinaryOperation>
	void bind_enable(Control& control, Property<bool>& property, BinaryOperation operation)
	{
		bind(control.enabled, property, operation);
	}

	//! Bind enable of a control to a property
	//! Binds only in one direction. From source property to control
	//! \params control Control to bind to. Must have a member enabled.
	//! \params property The source property of type value
	//! \params converter A Converter which converts the value type to a bool value or returns the required value type
	template <class Control, class Value, class Converter>
	void bind_enable(Control& control, Property<Value>& property, Converter converter)
	{
		bind(control.enabled, property, converter);
	}
	
	//! Bind visible of a control to a bool property
	//! \params control Control to bind to. Must have a member visible.
	//! \params property The source bool property
	template <class Control>
	void bind_visible(Control& control, Property<bool>& property)
	{
		bind(control.visible, property);
	}

	//! Bind visible of a control to a property
	//! Binds only in one direction. From source property to control
	//! \params control Control to bind to. Must have a member visible.
	//! \params property The source property of type value
	//! \params converter A Converter which converts the value type to a bool value or returns the required value type
	template <class Control, class Value, class Converter>
	void bind_visible(Control& control, Property<Value>& property, Converter converter)
	{
		bind(control.visible, property, converter);
	}

	//! Bind visible of a control to a collection
	//! Binds only in one direction. From source collection to control
	//! \params control Control to bind to. Must have a member visible.
	//! \params property The source collection of type value
	//! \params converter A Converter which converts the value type to a bool value or returns the required value type
	template <class Control, class Value, class Converter>
	void bind_visible(Control& control, Collection<Value>& collection, Converter converter)
	{
		control.visible = converter(collection.values());
		collection.changed += [&control, converter](const std::vector<Value>& values)
		{
			control.visible = converter(values);
		};
	}

	//! Bind select of a control to a bool property
	//! Binds only in one direction. From source property to control
	//! \params control Control to be selected based on property true value. Must have a method select
	//! \params property The source bool property
	template <class Control>
	void bind_select(Control& control, Property<bool>& property)
	{
		if (property.value()) { control.select(); }
		property.changed += [&control](bool value)
		{
			if (value) { control.select(); }
		};
	}

	//! Bind select of a control to a bool property
	//! Binds only in one direction. From source property to control
	//! \params control Control to be selected based on property true value. Must have a method select
	//! \params property The source bool property
	template <class Control, class BinaryOperation>
	void bind_select(Control& control, Property<bool>& property, BinaryOperation operation)
	{
		if (operation(property.value())) { control.select(); }
		property.changed += [&control, operation](bool value)
		{
			if (operation(value)) { control.select(); }
		};
	}

	//! Bind select of a control to a property
	//! Binds only in both direction. From source property to control and vice versa
	//! \params control Control to be selected based on property true value. Must have a method select
	//! \params property The source property of type value
	//! \params converter A Converter which converts the value type to a bool value or returns the required value type
	//! \params direction Binding direction. If direction is TwoWay, when control eventSelect is called, property is updated
	template <class Control, class Converter, class Value>
	void bind_select(Control& control, Property<Value>& property, Converter converter, Direction direction)
	{
		if (converter(property.value())) { control.select(); }
		property.changed += [&control, converter](Value value)
		{
			if (converter(value)) { control.select(); }
		};

		if(direction == Direction::TwoWay)
		{
			control.eventSelect = [&property, converter]
			{
				property = converter.value();
			};
		}
	}

	//! Bind checked of a control to a bool property
	//! \params control Control to bind to. Must have members checked, assign and eventToggled.
	//! \params property The source bool property
	//! \direction Binding direction. If direction is TwoWay, when control eventToggled is called, property is updated
	template <class Control>
	void bind_checked(Control& control, Property<bool>& property, Direction direction)
	{
		control.checked = property.value();
		property.changed += [&control](bool value)
		{
			control.assign(value);
		};

		if (direction == Direction::TwoWay)
		{
			control.eventToggled = [&control, &property]
			{
				property = control.checked;
			};
		}
	}

	//! Bind checked of a control to a bool property
	//! \params control Control to bind to. Must have members checked, assign and eventToggled.
	//! \params property The source bool property
	//! \params operation A binary operation (eg. Invert) to be operated on property value, before assigning it to control
	//! \direction Binding direction. If direction is TwoWay, when control eventToggled is called, property is updated
	template <class Control, class BinaryOperation>
	void bind_checked(Control& control, Property<bool>& property, BinaryOperation operation, Direction direction)
	{
		control.checked = operation(property.value());
		property.changed += [&control, operation](bool value)
		{
			control.assign(operation(value));
		};

		if (direction == Direction::TwoWay)
		{
			control.eventToggled = [&control, &property, operation]
			{
				property = operation(control.checked);
			};
		}
	}

	//! Bind text of a control to a std::wstring property
	//! Binds only in one direction. From source property to control
	//! \params control Control to bind to. Must have a member text.
	//! \params property The source std::wstring property
	template <class Control>
	void bind_text(Control& control, Property<std::wstring>& property)
	{
		control.text = property.value();
		property.changed += [&control](const std::wstring& value)
		{
			control.text = value;
		};
	}

	//! Bind text of a control to a std::wstring property
	//! \params control Control to bind to. Must have members text and eventChanged.
	//! \params property The source wstring property
	//! \direction Binding direction. If direction is TwoWay, when control eventChanged is called, property is updated
	template <class Control>
	void bind_text(Control& control, Property<std::wstring>& property, Direction direction)
	{
		control.text = property.value();
		property.changed += [&control](const std::wstring& value)
		{
			control.text = value;
		};

		if (direction == Direction::TwoWay)
		{
			control.eventChanged = [&control, &property]
			{
				property = control.text;
			};
		}
	}

	//! Bind text of a control to a property
	//! Binds only in one direction. From source property to control
	//! \params control Control to bind to. Must have a member text.
	//! \params property The source property
	//! \params toText Converter to convert from property value to text value expected by the control.
	template <class Control, class Value, class ToText>
	void bind_text(Control& control, Property<Value>& property, ToText toText)
	{
		control.text = toText(property.value());
		property.changed += [&control, toText](const Value& value)
		{
			control.text = toText(value);
		};
	}

	//! Bind text of a control to a property.
	//! Binds in both direction. From source property to control and vice versa
	//! \params control Control to bind to. Must have members text and eventChanged.
	//! \params property The source property
	//! \params toText Converter to convert from property value to text value expected by the control.
	//! \params toText Converter to convert from control text to property value.
	template <class Control, class Value, class ToText, class FromText>
	void bind_text(Control& control, Property<Value>& property, ToText toText, FromText fromText)
	{
		control.text = toText(property.value());
		property.changed += [&control, toText](const Value& value)
		{
			control.text = toText(value);
		};

		control.eventChanged = [&control, &property, fromText]
		{
			property = fromText(control.text);
		};
	}

	//! Bind text of a control to a collection
	//! Binds values only in one direction. From source collection to control
	//! Binds selection in direction passed
	//! \params control Control to bind to. Must have members text.
	//! \params collection The source collection
	//! \params toText Converter to convert from collection to items list expected by the control
	//! \direction Binding direction. If direction is TwoWay, when control eventSelectionChanged is called, collection selection is updated
	template <class Control, class Value, class ToList>
	void bind_list(Control& control, Collection<Value>& collection, ToList toList)
	{
		control.setItems(toList(collection.values()));
		collection.changed += [&control, toList](const typename Collection<Value>::Values& values)
		{
			control.setItems(toList(values));
		};
	}

	//! Bind text of a control to a collection
	//! Binds values only in one direction. From source collection to control
	//! Binds selection in direction passed
	//! \params control Control to bind to. Must have members text.
	//! \params collection The source collection
	//! \params toText Converter to convert from collection to items list expected by the control
	//! \direction Binding direction. If direction is TwoWay, when control eventSelectionChanged is called, collection selection is updated
	template <class Control, class Value, class ToList>
	void bind_selectableList(Control& control, SelectableCollection<Value>& collection, ToList toList, Direction direction)
	{
		control.setItems(toList(collection.values()), collection.selection());
		collection.changed += [&control, toList](const typename SelectableCollection<Value>::Values& values)
		{
			control.setItems(toList(values));
		};

		collection.selectionChanged += [&control](const typename SelectableCollection<Value>::Selection selection)
		{
			control.setSelectedItem(selection);
		};

		if(direction == Direction::TwoWay)
		{
			control.eventSelectionChanged = [&control, &collection]
			{
				collection.select(control.getSelectedItem());
			};
		}
	}

	//! Bind text of a control to a collection
	//! Binds values only in one direction. From source collection to control
	//! Binds selection in direction passed
	//! \params control Control to bind to. Must have members text.
	//! \params collection The source collection
	//! \params toText Converter to convert from collection to items list expected by the control
	//! \direction Binding direction. If direction is TwoWay, when control eventSelectionChanged is called, collection selection is updated
	template <class Control, class Value, class ToList>
	void bind_multiSelectableList(Control& control, MultiSelectableCollection<Value>& collection, ToList toList, Direction direction)
	{
		control.setItems(toList(collection.values()), collection.selection());
		collection.changed += [&control, toList](const typename MultiSelectableCollection<Value>::Values& values)
		{
			control.setItems(toList(values));
		};

		collection.selectionChanged += [&control](const typename MultiSelectableCollection<Value>::Selection selection)
		{
			control.setSelection(selection);
		};

		if (direction == Direction::TwoWay)
		{
			control.eventSelectionChanged = [&control, &collection]
			{
				collection.select(control.getSelection());
			};
		}
	}

	//! Binds a control to a command
	//! \param control Control to bind to. Must have members enabled or visible and eventClicked.
	//! \param command The source command
	//! \param bindVisibility If true, binds the can execute to visible property. Else, binds the can execute value to enabled property. Binds enabled by default.
	template <class Control>
	void bind_command(Control& control, Command& command, bool bindVisibility = false)
	{
		(bindVisibility ? control.visible : control.enabled) = command.canExecute.value();
		control.eventClicked = [&command] { command(); };
		command.canExecute.changed += [&control, &command, bindVisibility](bool)
		{
			(bindVisibility ? control.visible : control.enabled) = command.canExecute.value();
		};
	}

	//! Bind accent of a control to a bool property
	//! \params control Control to bind to. Must have a member accent.
	//! \params property The source bool property
	template <class Control>
	void bind_accent(Control& control, Property<bool>& property)
	{
		control.accent = property.value();
		property.changed += [&control](bool value)
		{
			control.accent = value;
		};
	}

	//! Bind accent of a control to a bool property
	//! Binds only in one direction. From source property to control
	//! \params control Control to bind to. Must have a member accent.
	//! \params property The source bool property
	//! \params operation A binary operation (eg. Invert) to be operated on property value, before assigning it to control
	template <class Control, class BinaryOperation>
	void bind_accent(Control& control, Property<bool>& property, BinaryOperation operation)
	{
		control.accent = operation(property.value());
		property.changed += [&control, operation](bool value)
		{
			control.accent = operation(value);
		};
	}

	//! Bind text of a EditableComboBox to a collection and a property
	//! Binds in both direction. From source property to control and vice versa
	//! \params control EditableComboBox control.
	//! \params collection The source collection of EditableComboBox combobox
	//! \params toText Converter to convert from property value to text value expected by the EditableComboBox editbox.
	//! \params fromText Converter to convert from EditableComboBox editbox text to property value.
	template <class Control, class Value, class ToList, class ToText, class FromText>
	void bind_EditableComboBox(Control& control, EditableSelectableCollection<Value>& collection, ToList toList, ToText toText, FromText fromText)
	{
		control.editBox.text = toText(collection.editableValue.value());
		control.setItems(toList(collection.values()), collection.selection());

		collection.changed += [&control, &collection, &toList, &toText](const typename SelectableCollection<Value>::Values& values)
		{
			control.setItems(toList(values));
		};
		collection.selectionChanged += [&control, &collection](const typename SelectableCollection<Value>::Selection& selection)
		{
			control.setSelectedItem(selection);
		};

		collection.editableValue.changed += [&control, &collection, &toText](const Value& value)
		{
			control.editBox.text = toText(value);
		};

		control.eventSelectionChanged = [&control, &collection]
		{
			collection.select(control.getSelectedItem());
		};
		control.editboxTextChanged += [&control, &collection, &fromText]()
		{
			collection.editableValue = fromText(control.editBox.text);
		};
	}

	//! Binds image of a control to an image property
	//! Binds only in one direction. From source property to control
	//! \params control Control to bind to. Must have a member image.
	//! \params property The source image property
	template <class Control>
	void bind_image(Control& control, Property<Sense::Image>& property)
	{
		control.image = property.value();
		property.changed += [&control](Sense::Image value)
		{
			control.image = value;
		};
	}

	//! Binds image of a control to a property
	//! Binds only in one direction. From source property to control
	//! \params control Control to bind to. Must have a member image.
	//! \params property The source property
	//! \params converter A Converter which converts the value type to a Sense::Image value
	template <class Control, class Value, class Converter>
	void bind_image(Control& control, Property<Value>& property, Converter converter)
	{
		control.image = converter(property.value());
		property.changed += [&control, converter](const Value& value)
		{
			control.image = converter(value);
		};
	}
	
}}}
