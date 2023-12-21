// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "Workflow/control/editablecombobox.h"
#include <Automation.h>

namespace Philips { namespace Workflow { namespace Control { namespace TestPeer
{
	class EditableComboBoxPeer : public Peer<ComboBox> //TICS !OOP#013: Peer is part of Sense library
	{
	public:
		explicit EditableComboBoxPeer(EditableComboBox& editableComboBox);
		virtual ~EditableComboBoxPeer() = default;

        bool editBoxVisible() const;
        bool editBoxEnabled() const;
        void insertText(const std::wstring &text);
		void pressKey(const IKeyboard::Button button);
        void selectItem(const int index);
		
	private:
        Peer<EditBox>                        _editBox          ;
	};
	
}}}}
