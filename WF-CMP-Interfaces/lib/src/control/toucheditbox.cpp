// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/control/toucheditbox.h"
#include "../Resource.h"
#include <algorithm>

namespace Philips { namespace Workflow { namespace Control {
class ColorCode
	{
	public:
		static ColorCode& instance()
		{
			static ColorCode inst;
			return inst;
		}

		const Color Gray15Alpha80[8] = { Palette::Gray15 * 0.8, Palette::Gray15 * 0.8
			, Palette::Gray15 * 0.8, Palette::Gray15 * 0.8
			, Palette::Gray15 * 0.8, Palette::Gray15 * 0.8
			, Palette::Gray15 * 0.8, Palette::Gray15 * 0.8 };

		const Color Gray15Alpha50[8] = { Palette::Gray15 * 0.5, Palette::Gray15 * 0.5
			, Palette::Gray15 * 0.5, Palette::Gray15 * 0.5
			, Palette::Gray15 * 0.5, Palette::Gray15 * 0.5
			, Palette::Gray15 * 0.5, Palette::Gray15 * 0.5 };

		const Color Gray15Alpha35[8] = { Palette::Gray15 * 0.35, Palette::Gray15 * 0.35
			, Palette::Gray15 * 0.35, Palette::Gray15 * 0.35
			, Palette::Gray15 * 0.35, Palette::Gray15 * 0.35
			, Palette::Gray15 * 0.35, Palette::Gray15 * 0.35 };

		const Color Gray15Alpha20[8] = { Palette::Gray15 * 0.2, Palette::Gray15 * 0.2
			, Palette::Gray15 * 0.2, Palette::Gray15 * 0.2
			, Palette::Gray15 * 0.2, Palette::Gray15 * 0.2
			, Palette::Gray15 * 0.2, Palette::Gray15 * 0.2 };

		const Color Gray15Alpha10[8] = { Palette::Gray15 * 0.1, Palette::Gray15 * 0.1
			, Palette::Gray15 * 0.1, Palette::Gray15 * 0.1
			, Palette::Gray15 * 0.1, Palette::Gray15 * 0.1
			, Palette::Gray15 * 0.1, Palette::Gray15 * 0.1 };

		const Color Gray90Alpha100[8] = { Palette::Gray90, Palette::Gray90
			, Palette::Gray90 , Palette::Gray90
			, Palette::Gray90 , Palette::Gray90
			, Palette::Gray90 , Palette::Gray90 };

		const Color Gray90Alpha10[8] = { Palette::Gray90 * 0.1, Palette::Gray90 * 0.1
			, Palette::Gray90 * 0.1, Palette::Gray90 * 0.1
			, Palette::Gray90 * 0.1, Palette::Gray90 * 0.1
			, Palette::Gray90 * 0.1, Palette::Gray90 * 0.1 };


		const Color warningFontColor[8] = { Color::fromRgb(230,88,0), Color::fromRgb(230,88,0)
																		   , Color::fromRgb(230,88,0), Color::fromRgb(230,88,0)
																		   , Color::fromRgb(230,88,0), Color::fromRgb(230,88,0)
																		   , Color::fromRgb(230,88,0), Color::fromRgb(230,88,0) };

		const Color warningBgColor[8] = { Color::fromRgb(255,241,211), Color::fromRgb(255,241,211)
																				   , Color::fromRgb(255,241,211), Color::fromRgb(255,241,211)
																				   , Color::fromRgb(255,241,211), Color::fromRgb(255,241,211)
																				   , Color::fromRgb(255,241,211), Color::fromRgb(255,241,211) };

		const Color warningBorderColor[8] = { Color::fromRgb(240,124,0), Color::fromRgb(240,124,0)
																				   , Color::fromRgb(240,124,0), Color::fromRgb(240,124,0)
																				   , Color::fromRgb(240,124,0), Color::fromRgb(240,124,0)
																				   , Color::fromRgb(240,124,0), Color::fromRgb(240,124,0) };


	private:

		ColorCode() = default;
	};

TouchEditBox::TouchEditBox(Control &parent, VirtualKeyboard &keyboard, const ClearButtonVisible clearButtonVisibility)
	: EditBox(parent),
	clearButton(*this),
	keyboard(keyboard),
	clearButtonVisibility(clearButtonVisibility),
	originalClearButtonVisibility(clearButtonVisibility),
	editBoxParent(parent),
	filter(L""),
	enableWarning(false, [this] { setColorForEditBox(); }),
	clearButtonSize(ClearButtonSize::Small, [this] { setClearButtonSize(); })
{
	colorBackground	 = &(ColorCode::instance().Gray90Alpha10);
	colorFont		 = &(ColorCode::instance().Gray15Alpha80);
	colorCaret		 = &(ColorCode::instance().Gray15Alpha80);
	colorBorder		 = &(ColorCode::instance().Gray15Alpha20);
	colorWater		 = &(ColorCode::instance().Gray15Alpha50);

	// Set Clear Button Properties
	clearButton.text			= L"";
	clearButton.name			= L"clearButton";
	clearButton.image			= Resource::cross_circle_grey_22;
	setClearButtonSize();
	clearButton.visible		= false;
	clearButton.tabStop		= false;
	this->clearButton.eventClicked = [this]()
	{
		onClearButtonClicked();
	};

	margin = Margin(16, 0, 40, 0);

	// Filtering of characters
	eventAllowCharacter = [this](wchar_t c)
	{ 
		std::wstring val = filter;
		if (val.empty())
		{
			return true;
		}
		return val.find(c) != std::wstring::npos;
	};
}

void TouchEditBox::press(wchar_t str)
{
	if (focus) onKeyboardChar(str);
}

void TouchEditBox::press(IKeyboard::Button button)
{
	if (focus) onKeyboardPress(button);
}

bool TouchEditBox::onMouseDown(IMouse::Button button, bool doubleClick)
{
	bool result = EditBox::onMouseDown(button, doubleClick);

	//if the keyboard was hidden, re-show it when the user clicks the m_editbox again
	if (keyboard.editBox() == this && keyboard.autoShowHide)
	{
		keyboard.show();
		keyboard.bringToFront();
	}

	return result;
}
void TouchEditBox::setColorForEditBox()
{
	if (enableWarning)
	{
		colorBackground		= &ColorCode::instance().warningBgColor;
		colorFont			= &ColorCode::instance().warningFontColor;
		colorBorder			= &ColorCode::instance().warningBorderColor;
		setClearButtonImage(enableWarning);
	}
	else
	{
		colorBackground		= &(ColorCode::instance().Gray90Alpha10);
		setClearButtonImage(enableWarning);
		if(!isEnabled())
		{
			colorFont = &ColorCode::instance().Gray15Alpha35;
			colorBorder = &ColorCode::instance().Gray15Alpha10;
		}
		else
		{
			colorFont = &ColorCode::instance().Gray15Alpha80;
			colorBorder = &ColorCode::instance().Gray15Alpha20;
		}
	}
	invalidate();
}

void TouchEditBox::setClearButtonSize()
{
	Size buttonSize;
	if (clearButtonSize == ClearButtonSize::VerySmall)
	{
		buttonSize = Size(16, 16);
	}
	else if (clearButtonSize == ClearButtonSize::Small)
	{
		buttonSize = Size(22, 22);
	}
	else
	{
		buttonSize = Size(32, 32);
	}
	clearButton.imageSize		= buttonSize;
	clearButton.size			= buttonSize;
	setClearButtonImage(enableWarning);
}

void TouchEditBox::setClearButtonVisibility(bool show)
{
	if (show) clearButtonVisibility = originalClearButtonVisibility;
	else
	{
		clearButtonVisibility	= ClearButtonVisible::Hide;
		clearButton.visible		= false;
	}
	onResize();
}

void TouchEditBox::onKeyboardEnter(bool argShowFocus)
{
	EditBox::onKeyboardEnter(argShowFocus);
	keyboard.setEditBox(this);
	if (enabled && !enableWarning)
	{
		colorBorder = &ColorCode::instance().Gray15Alpha50;
		colorBackground = &ColorCode::instance().Gray90Alpha100;
		clearButton.visible = clearButtonVisibility == ClearButtonVisible::Show || (!text->empty() && clearButtonVisibility == ClearButtonVisible::Auto);
	}
	//make sure m_editbox margin is updated
	onResize();
}

void TouchEditBox::onResize()
{
	EditBox::onResize();

	Rect rect = getClientSpace().bounds;

	if (multiline) { rect.left = rect.right - 60.0; rect.bottom = rect.top + 64.0; }
	else { rect.left = rect.right - rect.height(); }

	rect -= Point(scrollbarSize, 0);

	clearButton.setBounds(rect);
	margin = Margin(margin->left, margin->top, clearButton.isVisible() ? clearButton.size->width : 0, margin->bottom);
}

void TouchEditBox::onKeyboardLeave()
{
	EditBox::onKeyboardLeave();
	keyboard.setEditBox(nullptr);
	if (enabled && !enableWarning)
	{
		colorBorder = &ColorCode::instance().Gray15Alpha20;
		colorBackground = &ColorCode::instance().Gray90Alpha10;
	}
	clearButton.visible = clearButtonVisibility == ClearButtonVisible::Show;
	//make sure m_editbox margin is updated
	onResize();
	keyboardHide();
}

bool TouchEditBox::onKeyboardDown(IKeyboard::Button button)
{
	bool result = EditBox::onKeyboardDown(button);

	if (button == IKeyboard::Button::Tab)    tabPressed();
	
	return result;
}

void TouchEditBox::onKeyboardPress(IKeyboard::Button button)
{
	EditBox::onKeyboardPress(button);

	if (button == IKeyboard::Button::Tab)	tabPressed();
}

void TouchEditBox::onTextChanged()
{
	EditBox::onTextChanged();

	if (focus) clearButton.visible = clearButtonVisibility == ClearButtonVisible::Show || (!text->empty() && isEnabled() && (clearButtonVisibility != ClearButtonVisible::Hide));

	margin = Margin(margin->left, margin->top, clearButton.isVisible() ? clearButton.size->width : 0, margin->bottom);
	textChanged();
}

void TouchEditBox::onClearButtonClicked()
{
	this->text = L"";
	Control::keyboard.setFocus(this);
	clearButtonClicked();
}

bool TouchEditBox::onKeyboardChar(wchar_t character)
{	
	bool result = true;
	characterEntered(character, std::ref(result));
	if (!result)
	{
		return true;
	}	
	return EditBox::onKeyboardChar(character);
}

void TouchEditBox::onEnabledChanged()
{
	if (isEnabled())
	{
		setColorForEditBox();
	}
	else
	{
		colorFont   = (enableWarning) ? &ColorCode::instance().warningFontColor : &ColorCode::instance().Gray15Alpha35;
		colorBorder = (enableWarning) ? &ColorCode::instance().warningBorderColor : &ColorCode::instance().Gray15Alpha10;
	}
}

void TouchEditBox::onFocusChanged()
{
	if (enabled)
	{
		focusChanged(focus);
		EditBox::onFocusChanged();
	}
}
	
void TouchEditBox::setClearButtonImage(bool warningEnabled)
{
	if(warningEnabled == true)
	{
		if (clearButtonSize == ClearButtonSize::VerySmall)
		{
			clearButton.image = Resource::cross_circle_orange_16;
		}
		else if (clearButtonSize == ClearButtonSize::Small)
		{
			clearButton.image = Resource::cross_circle_orange_22;
		}
		else
		{
			clearButton.image = Resource::cross_circle_orange_32;
		}
	}
	else
	{
		if (clearButtonSize == ClearButtonSize::VerySmall)
		{
			clearButton.image = Resource::cross_circle_grey_16;
		}
		else if (clearButtonSize == ClearButtonSize::Small)
		{
			clearButton.image = Resource::cross_circle_grey_22;
		}
		else
		{
			clearButton.image = Resource::cross_circle_grey_32;
		}
	}
}
}}}
