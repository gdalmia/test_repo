// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/control/testpeer/editablecomboboxpeer.h"
#include "Workflow/control/editablecombobox.h"
#include <Automation.h>

namespace Philips { namespace Workflow { namespace Control { namespace TestPeer
{
    EditableComboBoxPeer::EditableComboBoxPeer(EditableComboBox& editableComboBox):
        Peer<ComboBox>(editableComboBox),
        _editBox(editableComboBox.editBox)
    {
	}

    bool EditableComboBoxPeer::editBoxVisible() const
    {
        return _editBox.control.isVisible();
    }

    bool EditableComboBoxPeer::editBoxEnabled() const
    {
        return _editBox.control.isEnabled();
    }

    void EditableComboBoxPeer::insertText(const std::wstring &text)
    {
        _editBox.control.text = text;
    }

	void EditableComboBoxPeer::pressKey(const IKeyboard::Button button)
	{
		_editBox.mouseClick();
		_editBox.keyboardDown(button);
	}

    void EditableComboBoxPeer::selectItem(const int index)
    {
        select(index);
    }

}}}}
