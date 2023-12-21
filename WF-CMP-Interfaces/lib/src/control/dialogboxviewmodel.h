// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "Workflow/control/dialogbox.h"
#include "Workflow/binding.h"
#include <string>

//TICS -INT#002: View models needs data member in public

namespace Philips { namespace Workflow { namespace Control {

	class DialogBoxViewModel
	{
	public:
		DialogBoxViewModel();
		~DialogBoxViewModel() = default;

		Binding::Property<std::string>		          header;
		Binding::Property<std::string>		          text;
		Binding::Property<std::string>		          accent;
		Binding::Property<std::string>		          primary;
		Binding::Property<std::string>		          secondary;
		Binding::Property<DialogBox::Message::Type>   type;
		Binding::Property<bool>				          accentVisible;
		Binding::Property<bool>				          primaryVisible;
		Binding::Property<bool>				          secondaryVisible;
		Binding::Property<bool>						  showDialog;
		
		Binding::Command					          accentClicked;
		Binding::Command					          primaryClicked;
		Binding::Command					          secondaryClicked;
	};										          

}}}
