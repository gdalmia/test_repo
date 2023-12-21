// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include <workflow.h>
#include <Sense.h>
#include <DLS.h>

namespace Philips { namespace Workflow { namespace Control
{	
	//! This class provides a hyper link label. 
	class HyperlinkLabel : public Label
	{
	public:
	    explicit HyperlinkLabel(Sense::Control &parent);
		HyperlinkLabel(const HyperlinkLabel&) = delete;
		HyperlinkLabel& operator=(const HyperlinkLabel&) = delete;
		virtual ~HyperlinkLabel() = default;
		
		//!< Use this to set the font for the control. 
		Property<Font>					linkFont;			//TICS !INT#002: Property needs to be public so that view can be bound to it.

		//!< This is emitted whenever the link is clicked by the user. 
	    Philips::Workflow::Event<>		eventClicked;		//TICS !INT#002: Property needs to be public so that view can be bound to it.	

	protected:
		virtual void onMouseEnter() override;
		virtual void onMouseLeave() override;
		virtual bool onMouseDown(IMouse::Button button, bool doubleClick) override;
		virtual void onMouseUp(IMouse::Button button, bool cancel) override;
		bool getCursor(Cursor &cursor) const override;

	private:
		bool	_hovering;
	};

}}}

