// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/control/toucheditbox.h"
#include <Trace.h>

namespace Philips { namespace Workflow { namespace Control {

	VirtualKeyboard::VirtualKeyboard(Control &parent)
	:
		Control		(parent),
		showNumeric	(false,			[this] { keyboard.keyLNum.checked   = showNumeric;	keyboard.keyRNum.checked	= showNumeric;	updateContents(); }),
		shift		(false,			[this] { keyboard.keyLShift.checked = shift;		keyboard.keyRShift.checked	= shift;		updateContents(); }),
		capsLock	(false,			[this] { keyboard.keyCaps.checked   = capsLock;													updateContents(); }),
		autoShowHide(true,			[this] { keyboard.hide.visible		= autoShowHide; }),
		animation	(scheduler, 0,	[this] { updatePos(); }),
		keyboard	(*this),
		m_editbox	(nullptr)
	{
		TraceScope();	
		size = keyboard.size;
		keys.reserve(30);

		keys.push_back(&keyboard.key00);
		keys.push_back(&keyboard.key01);
		keys.push_back(&keyboard.key02);
		keys.push_back(&keyboard.key03);
		keys.push_back(&keyboard.key04);
		keys.push_back(&keyboard.key05);
		keys.push_back(&keyboard.key06);
		keys.push_back(&keyboard.key07);
		keys.push_back(&keyboard.key08);
		keys.push_back(&keyboard.key09);

		keys.push_back(&keyboard.key10);
		keys.push_back(&keyboard.key11);
		keys.push_back(&keyboard.key12);
		keys.push_back(&keyboard.key13);
		keys.push_back(&keyboard.key14);
		keys.push_back(&keyboard.key15);
		keys.push_back(&keyboard.key16);
		keys.push_back(&keyboard.key17);
		keys.push_back(&keyboard.key18);

		keys.push_back(&keyboard.key20);
		keys.push_back(&keyboard.key21);
		keys.push_back(&keyboard.key22);
		keys.push_back(&keyboard.key23);
		keys.push_back(&keyboard.key24);
		keys.push_back(&keyboard.key25);
		keys.push_back(&keyboard.key26);
		keys.push_back(&keyboard.key27);
		keys.push_back(&keyboard.key28);

		for (int i = 0; i < static_cast<int>(keys.size()); i++)
		{
			keys[i]->eventClicked = [this, i]
			{
				if		(showNumeric && i == 17) press(IKeyboard::Button::Up);	 
				else if (showNumeric && i == 25) press(IKeyboard::Button::Left);  
				else if (showNumeric && i == 26) press(IKeyboard::Button::Down);  
				else if (showNumeric && i == 27) press(IKeyboard::Button::Right); 
				else	press(getChar(i));
			};
		}

		keyboard.keyBack.eventClicked	= [this] { press(IKeyboard::Button::BackSpace);				};
		keyboard.keyReturn.eventClicked = [this] { press(IKeyboard::Button::Enter);	press(L'\n');	};
		keyboard.keySpace.eventClicked	= [this] { press(L' ');										};
		keyboard.keyCaps.eventClicked	= [this] { capsLock = !capsLock;							};
		keyboard.keyLShift.eventClicked = [this] { shift = !shift;									};
		keyboard.keyRShift.eventClicked = [this] { shift = !shift;									};
		keyboard.keyLNum.eventClicked	= [this] { showNumeric = !showNumeric;						};
		keyboard.keyRNum.eventClicked	= [this] { showNumeric = !showNumeric;						};
		keyboard.hide.eventClicked		= [this] { if (autoShowHide) hide();						};
		keyboard.keyTab.eventClicked = [this] { Control::keyboard.onKeyboardPress(IKeyboard::Button::Tab); press(IKeyboard::Button::Tab); };

		updateContents();
		updatePos();
	}

	Sense::Control* VirtualKeyboard::findControl()
	{
		Control *result = Control::findControl();
		if (result == this) return nullptr;
		return result;
	}

	void VirtualKeyboard::setEditBox(TouchEditBox* box)
	{
		showNumeric = false;
		shift		= false;
		capsLock	= false;
		m_editbox	= box;
		updateContents();

		if (autoShowHide)
		{
			if (nullptr != m_editbox) { if (m_editbox->isEnabled()) show(); }
			else { hide(); }
		}
	}

	TouchEditBox* VirtualKeyboard::editBox() const
	{
		return m_editbox;
	}

	void VirtualKeyboard::show()
	{
		if (m_editbox == nullptr)
		{
			TraceError() << "Edit box is null.";
			return;
		}
		if (m_editbox->enabled)
		{
			// showNumeric property gets updated for every new editbox attachemnt with keyboard
			showNumeric = ((m_editbox->filter == L"0123456789") || (m_editbox->filter == L"0123456789."));
			double parentBottom = getParentSpace().bounds.bottom;
			Size vSize = static_cast<Size>(size);
			position = Point(0, parentBottom - (scale*(vSize.height)));

			if (canvas.isAnimated()) { animation.start(1, 0.2, AnimationCurve::SigmoidFast); }
			else { animation.set(1); }
			m_editbox->keyboardShow();
		}
	}

	void VirtualKeyboard::hide()
	{
		if (canvas.isAnimated()) { animation.start(0, 0.2, AnimationCurve::SigmoidFast); }
		else { animation.set(0); }

		if ((nullptr != this->m_editbox)) {
			this->m_editbox->keyboardHide();
		}
	}

	bool VirtualKeyboard::isKeyBoardVisible() const
	{
		return keyboard.isVisible();
	}

	Sense::Rect VirtualKeyboard::getCurrentBounds() const
	{
		return getSpace().bounds & keyboard.getBounds();
	}

	void VirtualKeyboard::updateNavigationKeys(const int i, bool& localEnabled, wchar_t* str)
	{
		if (showNumeric && i == 17) {
			keys[i]->text = L""; keys[i]->image = Icons::NavigationUp_Alternate_XBold;	 localEnabled = true;
		}
		else if (showNumeric && i == 25) {
			keys[i]->text = L""; keys[i]->image = Icons::NavigationLeft_Alternate_XBold;  localEnabled = true;
		}
		else if (showNumeric && i == 26) {
			keys[i]->text = L""; keys[i]->image = Icons::NavigationDown_Alternate_XBold;  localEnabled = true;
		}
		else if (showNumeric && i == 27) {
			keys[i]->text = L""; keys[i]->image = Icons::NavigationRight_Alternate_XBold; localEnabled = true;
		}
		else
		{
			keys[i]->text = str; keys[i]->image = Image();
		};
	}

	wchar_t VirtualKeyboard::getChar(int index) const
	{
		const wchar_t *table[] =
		{
			L"qQ1!",
			L"wW2@",
			L"eE3#",
			L"rR4$",
			L"tT5%",
			L"yY6^",
			L"uU7&",
			L"iI8*",
			L"oO9(",
			L"pP0)",
			L"aA`~",
			L"sS-_",
			L"dD=+",
			L"fF[{",
			L"gG]}",
			L"hH\\|",
			L"jJ  ",
			L"kK??",
			L"lL  ",
			L"zZ;:",
			L"xX'\"",
			L"cC/?",
			L"vV,<",
			L"bB.>",
			L"nN  ",
			L"mM??",
			L",,??",
			L"..??"
		};

		if (index >= 0 && index < sizeof(table) / sizeof(*table))
		{
			int localLayout = 0;

			if (*shift ^ *capsLock) { localLayout += 1; }
			if (*showNumeric) { localLayout += 2; }

			return table[index][localLayout];
		}

		return 0;
	}
	
	void VirtualKeyboard::onResize()
	{
		Control::onResize();
		updatePos();
	}

	void VirtualKeyboard::press(wchar_t str)
	{
		TraceScope();
		if ((nullptr != m_editbox) && m_editbox->isEnabled()) { m_editbox->press(str); }
		if (!showNumeric) { shift = false; }
	}

	void VirtualKeyboard::press(IKeyboard::Button button)
	{
		if ((nullptr != m_editbox) && m_editbox->isEnabled()) { m_editbox->press(button); }
		if (!showNumeric) { shift = false; }
	}

	void VirtualKeyboard::updateContents()
	{
		std::wstring filter = m_editbox ? *m_editbox->filter : L"";

		for (int i = 0; i < static_cast<int>(keys.size()); i++)
		{
			wchar_t str[2] = { getChar(i), 0 };

			bool localEnabled = true;
			if (!filter.empty()) {
				localEnabled = filter.find(str[0]) != std::wstring::npos;
			}

			updateNavigationKeys(i, localEnabled, str);

			keys[i]->enabled = localEnabled;
			keys[i]->visible = str[0] != L' ';
		}

		keyboard.keySpace.enabled = filter.empty() || filter.find(L' ') != std::wstring::npos;
		keys[16]->size = Size(keys[0]->size->width + (showNumeric ? 25 : 0), keys[0]->size->height);
		keys[18]->size = Size(keys[0]->size->width - (showNumeric ? 25 : 0), keys[0]->size->height);
	}

	void VirtualKeyboard::updatePos()
	{
		Point pos[] = { Point((size->width - keyboard.size->width) / 2.0, size->height),
			Point((size->width - keyboard.size->width) / 2.0, 0) };

		double localVisible[] = { 0.0, 1.0 };

		keyboard.position = animation.get(pos);
		keyboard.visible = animation.get(localVisible) != 0.0;

		resize();
	}

}}}