// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "view.ui.h"
#include "viewmodel.h"

namespace Test { namespace Binding {

	using namespace Philips::Workflow::Control;
	
	class View: public Ui::View
	{
		friend class ViewtTestPeer;

	public:
		View(Control& parent, VirtualKeyboard& keyboard, ViewModel& viewModel);
	};
	
}}