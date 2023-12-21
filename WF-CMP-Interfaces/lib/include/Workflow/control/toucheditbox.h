// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "VirtualKeyboard.h"
#include <Sense.h>
#include <DLS.h>

//TICS -INT#002: Sense controls needs data member in public

namespace Philips { namespace Workflow { namespace Control {
	using namespace Sense;
	using namespace Sense::DLS;

	namespace TestPeer
	{
		class TouchEditBoxPeer;
	}
	
	/** This class provides Touch friendly Sense EditBox
	*/
	class TouchEditBox : public EditBox
	{
	public:	

		enum class ClearButtonVisible { Hide, Auto, Show };
		enum class ClearButtonSize	  { VerySmall, Small, Large };

		TouchEditBox(	Control &parent, VirtualKeyboard &keyboard
					  ,	const ClearButtonVisible showClear = ClearButtonVisible::Auto);
		virtual ~TouchEditBox() = default;
	
		/**
		* \brief To enter the character in the TouchEditBox
		* \return void
		*/
		void	press			(wchar_t str);

		/**
		* \brief To handle the pressing of special buttons i.e. Navigation keys/Backspace/Delete etc.
		* \return void
		*/
		void	press			(IKeyboard::Button button);
	
		/**
		* \ Raised when TouchEditBox text is changed
		*/
		Event<>					textChanged;

		/**
		* \ Raised when character is entered into TouchEditBox
		*/
		Event<wchar_t,bool&>	characterEntered;

		/**
		* \ Raised when Tab button is pressed TouchEditBox
		*/
		Event<>					tabPressed;

		/**
		* \ Raised when keyboard is hidden
		*/
		Event<>					keyboardHide;

		/**
		* \ Raised when keyboard is shown
		*/
		Event<>					keyboardShow;

		/**
		* \ Raised when focus is changed with respect to TouchEditBox
		*/
		Event<bool>					focusChanged;

		/**
		* \ Raised when clearButton clicked
		*/
		Event<>					clearButtonClicked;

		/**
		* \ Represents allowed characters in TouchEditBox
		*/
		Property<std::wstring>	filter;

		/**
		* \ Enable warning color in TouchEditBox
		*/
		Sense::Property<bool>   enableWarning;

		/**
		* \ set clear button size in TouchEditBox
		*/
		Sense::Property<ClearButtonSize>   clearButtonSize;
	
	protected:
		void onResize				()											override;
		bool onMouseDown			(IMouse::Button button, bool doubleClick)	override;
		void onKeyboardEnter		(bool showFocus)							override;
		void onKeyboardLeave		()											override;
		bool onKeyboardDown			(IKeyboard::Button button)					override;
		void onKeyboardPress		(IKeyboard::Button button)					override;
		void onTextChanged			()                                          override;
		bool onKeyboardChar			(wchar_t)									override;
		void onEnabledChanged		()											override;
		void onFocusChanged			()											override;
		void onClearButtonClicked	();
		void setColorForEditBox		();
		void setClearButtonSize();
		void setClearButtonVisibility(bool show);
	
	private:

		ButtonQuiet		    clearButton;
		VirtualKeyboard&	keyboard;
		ClearButtonVisible	clearButtonVisibility;
		ClearButtonVisible	originalClearButtonVisibility;
		Sense::Control&		editBoxParent;

		friend class TestPeer::TouchEditBoxPeer;

		void setClearButtonImage(bool warningEnabled);
	};
}}}