// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "Ui/virtualkeyboard.ui.h"
#include "Workflow/event.h"
#include <Sense.h>
#include <DLS.h>

//TICS -INT#002: Sense controls needs data member in public

namespace Philips { namespace Workflow { namespace Control {
	class TouchEditBox;
	namespace TestPeer
	{
		class VirtualKeyboardPeer;
	}
	
	/** This class provides Keyboard to be used with TouchEditBox
	*/
	class VirtualKeyboard : public Sense::Control
	{
	public:
		explicit VirtualKeyboard(Control &Parent);
		virtual ~VirtualKeyboard() = default;

		/**
		* \brief Set the TouchEditBox association with keyboard
		* \return void
		*/
		void setEditBox(TouchEditBox* box);

		/**
		* \brief Get the currently associated TouchEditBox
		* \return TouchEditBox* Associated Editbox instance
		*/
		TouchEditBox* editBox() const;

		/**
		* \brief Show the keyboard
		* \return void
		*/
		void show();

		/**
		* \brief Hide the keyboard
		* \return void
		*/
		void hide();

		/**
		* \ checks whether keyboard is displayed or not
		* \ return bool
		*/
		bool isKeyBoardVisible() const;

		/**
		* \ Returns the bounding rectangle of Keyboard.
		* \ return Rect
		*/
		Sense::Rect getCurrentBounds() const;
		
		/**
		* \ Raised when position of keyboard is changed
		*/
		Event<>			resize;

		/**
		* \ Represents if keyboard is in numeric mode
		*/
		Sense::Property<bool>	showNumeric;

		/**
		* \ Represents if shift is pressed
		*/
		Sense::Property<bool>	shift;

		/**
		* \ Represents if capslock is on
		*/
		Sense::Property<bool>	capsLock;

		/**
		* \ Represents if keyboard should be shown at the time of setEditBox() call
		*/
		Sense::Property<bool>	autoShowHide;
		
	private:
		Control* findControl() override;
		void updateNavigationKeys(const int, bool&, wchar_t*);
		wchar_t getChar(int index) const;
		void	onResize() override;

		void press(wchar_t str);
		void press(Sense::IKeyboard::Button button);
	
		void updateContents();
		void updatePos();

		std::vector<Sense::DLS::Button*> keys;
		Sense::Animation<2>		         animation;
		Ui::VirtualKeyboard		         keyboard;
		TouchEditBox*			         m_editbox;

		friend class TestPeer::VirtualKeyboardPeer;
	};

}}}