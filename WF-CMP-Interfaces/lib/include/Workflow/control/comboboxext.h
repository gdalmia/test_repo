// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include <Workflow/event.h>
#include <Sense.h>
#include <DLS.h>

namespace Philips { namespace Workflow { namespace Control
{	
	using namespace  Sense;
	using namespace  Sense::DLS;

	//! This control is a extention of ComboBox where selected item text is highligted. 
	class ComboBoxExt : public ComboBox
	{
		
	public:
		explicit ComboBoxExt(Control &Parent, const std::wstring &name = L"");

		Sense::Property<bool>			enabled;		//TICS !INT#002: Property needs to be public so that it can be subscribed.
		Event<>							tabPressed;		//TICS !INT#002: Event needs to be public so that it can be subscribed.

	protected:
		bool onKeyboardDown(IKeyboard::Button button) override;

	private:
		void setFontColor();
	};

}}}
