// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "Workflow/control/hyperlinklabel.h"
#include "../Resource.h"

using namespace Sense;
using namespace Sense::DLS;

namespace Philips { namespace Workflow { namespace Control
{
	HyperlinkLabel::HyperlinkLabel(Control &parent)
	    : Label(parent),
		linkFont(Font(L"Philips Healthcare Font", 12.0000, false, false, true, false), [this] { Font tmp = linkFont; tmp.underline = true; font = tmp; }),
		_hovering(false)
	{
		color = enabled ? Palette::GroupBlue30 : Palette::GroupBlue30*0.5;
	}

	void HyperlinkLabel::onMouseEnter()
	{
	    Label::onMouseEnter();
		if (enabled)
		{
			_hovering = true;
			color = Palette::GroupBlue25;
		}
	}

	void HyperlinkLabel::onMouseLeave()
	{
	    Label::onMouseLeave();
		if (enabled)
		{
			_hovering = false;
			color = Palette::GroupBlue30;
		}
	}

	bool HyperlinkLabel::onMouseDown(IMouse::Button button, bool doubleClick)
	{
	    Label::onMouseDown(button, doubleClick);
	    return true;
	}

	void HyperlinkLabel::onMouseUp(IMouse::Button button, bool cancel)
	{
		Label::onMouseUp(button, cancel);
		if (button == IMouse::Button::Left)		eventClicked();
	}

	bool HyperlinkLabel::getCursor(Cursor &cursor) const
	{
		if (_hovering)
		{
			cursor = Cursor(Resource::cursor_hand_c_32x32, 0, 0);
			return true;
		}
		return Control::getCursor(cursor);
	}

}}}
