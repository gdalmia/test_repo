// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "customlistview.h"

namespace Philips { namespace Workflow { namespace Control {
				
	using namespace Sense;
	using namespace Sense::DLS;

	CustomListView::CustomListViewItem::CustomListViewItem(CustomListView& parent)
	: VisibleItem(parent), _parent(parent)
	{
	}

	bool CustomListView::CustomListViewItem::onMouseDown(IMouse::Button button, bool doubleClick)
	{
		const bool result = VisibleItem::onMouseDown(button, doubleClick);
		if (doubleClick  && button == IMouse::Button::Left) _parent.eventDoubleClick(this->getIndex());
		return result;
	}

	CustomListView::CustomListView(Control& parent)
	:ListView(parent)
	{
		eventAllocateItem = [this]	{ return new CustomListViewItem(*this); };
	}
}}}