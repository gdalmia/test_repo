// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/control/testpeer/toucheditboxpeer.h"

namespace Philips { namespace Workflow { namespace Control { namespace TestPeer {

	TouchEditBoxPeer::TouchEditBoxPeer(TouchEditBox& editBox)
		: control(editBox)
		, clearPeer(control.clearButton)
	{}

	bool TouchEditBoxPeer::isClearButtonVisible() const
	{
		return control.clearButton.visible;
	}

	void TouchEditBoxPeer::clear() const
	{
		control.clearButton.eventClicked();
	}

	void TouchEditBoxPeer::enableWarning(const bool& validation) const
	{
		control.enableWarning = validation;
	}

	Color TouchEditBoxPeer::getEditBoxBGColor() const
	{
		return Color(control.colorBackground[0]->r
			, control.colorBackground[0]->g
			, control.colorBackground[0]->b);
	}

	Color TouchEditBoxPeer::getEditBoxFontColor() const
	{
		return Color(control.colorFont[0]->r
			, control.colorFont[0]->g
			, control.colorFont[0]->b);
	}

	Color TouchEditBoxPeer::getEditBoxBorderColor() const
	{
		return Color(control.colorBorder[0]->r
			, control.colorBorder[0]->g
			, control.colorBorder[0]->b);
	}

	Size TouchEditBoxPeer::getClearButtonImageSize() const
	{
		return control.clearButton.imageSize;
	}

	void TouchEditBoxPeer::enableControl(const bool& enable) const
	{
		control.enabled = enable;
	}

	Sense::Image TouchEditBoxPeer::getClearButtonImage() const
	{
		return control.clearButton.image;
	}

	void TouchEditBoxPeer::enterText()const
	{
		control.focus = true;
		control.enabled = true;
		control.text = L"Text";
	}
}}}}