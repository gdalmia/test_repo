// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "Workflow/control/toucheditbox.h"
#include <Automation.h>

//TICS -INT#002: Test peers needs data member in public

namespace Philips { namespace Workflow { namespace Control { namespace TestPeer {
	class TouchEditBoxPeer
	{
	public:
		explicit TouchEditBoxPeer(TouchEditBox& editBox);
		bool isClearButtonVisible() const;
		void clear() const;
		void enableWarning(const bool& validation) const;
		Color getEditBoxBGColor() const ;
		Color getEditBoxFontColor() const;
		Color getEditBoxBorderColor() const;
		Size getClearButtonImageSize() const;
		void enableControl(const bool& enable) const;
		Sense::Image getClearButtonImage() const;
		void enterText() const;
	
	private:
		TouchEditBox& control;
		Peer<ButtonQuiet> clearPeer;
	};
}}}}