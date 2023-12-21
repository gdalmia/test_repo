// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <Trace.h>
#include "Workflow/control/comboboxext.h"

namespace Philips { namespace Workflow { namespace Control
{
	ComboBoxExt::ComboBoxExt(Control &Parent, const std::wstring &name):
		ComboBox(Parent, name),
		enabled(true, [this] { ComboBox::enabled = enabled; setFontColor(); })
	{
		ComboBox::enabled = enabled;
		setFontColor();
	}

	void ComboBoxExt::setFontColor()
	{
		static const Color enabledFont[] = { Palette::Gray15 * 0.80,	Palette::Gray15 * 0.80,	Palette::Gray15 * 0.80,	Palette::Gray15 * 0.80 };
		static const Color defaultFont[] = { Palette::Gray20 * 0.55,	Palette::Gray15 * 0.60,	Palette::Gray15 * 0.55,	Palette::Gray20 * 0.40 };
		
		if (enabled) colorFont = &enabledFont;
		else		 colorFont = &defaultFont;
	}

	bool ComboBoxExt::onKeyboardDown(IKeyboard::Button button)
	{
		const auto result = ComboBox::onKeyboardDown(button);
		if (button == IKeyboard::Button::Tab)
			tabPressed();
		return result;
	}
	
}}}
