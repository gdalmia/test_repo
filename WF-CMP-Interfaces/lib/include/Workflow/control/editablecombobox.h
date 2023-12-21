// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include <Workflow/event.h>
#include <Sense.h>
#include <DLS.h>
#include "virtualkeyboard.h"
#include "toucheditbox.h"

namespace Philips { namespace Workflow { namespace Control
{
    using namespace Sense;
    using namespace Sense::DLS;

    namespace TestPeer { class EditableComboBoxPeer; }
	class EditableComboBox;
	
    //! This class represents Editable area in the ComboBox.
    class EditBoxArea : public TouchEditBox
    {
    public:
        explicit EditBoxArea(EditableComboBox& parent, VirtualKeyboard &keyboard);
        EditBoxArea(const EditBoxArea&) = delete;
        EditBoxArea& operator=(const EditBoxArea&) = delete;
        virtual ~EditBoxArea() = default;

		Event<IKeyboard::Button>	keyboardDown;				//TICS !INT#002: Event needs to be public so that it can be subscribed

    private:
        void onResize()									override;
		bool onKeyboardDown(IKeyboard::Button button)	override;
        void onKeyboardEnter(bool argShowFocus)         override;
        void onKeyboardLeave()                          override;
        void onEnabledChanged()                         override;

		EditableComboBox&			_parent;
        Sense::Color                defaultBorder[8];
    };


    //! This class represents a ComboBox that is editable.
    class EditableComboBox : public ComboBox
    {
        friend class TestPeer::EditableComboBoxPeer;
    public:
        explicit EditableComboBox(Control &Parent, VirtualKeyboard& keyboard);
		virtual ~EditableComboBox() = default;
        EditableComboBox(const EditableComboBox&) = delete;
        EditableComboBox& operator=(const EditableComboBox&) = delete;

    	std::wstring text() const;

		Event<>					   tabPressed;			//TICS !INT#002: Event needs to be public so that it can be subscribed
		Event<>					   editboxTextChanged;			//TICS !INT#002: Event needs to be public so that it can be subscribed
		EditBoxArea				   editBox;						//TICS !INT#002: Control needs to be public so that its properties can be used.

    private:
        void onResize()				override;
        void onSelectionChanged()	override;
        void onVisibleChanged()		override;
        void onEnabledChanged()		override;
		void render(IRenderer& renderer) const override;
		bool onKeyboardDown(IKeyboard::Button button) override;    	

		mutable Drawing						customArrow;
    };

}}}
