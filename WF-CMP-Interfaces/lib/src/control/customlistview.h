// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include <Sense.h>
#include <DLS.h>
#include <functional>
#include "Workflow/event.h"


namespace Philips { namespace Workflow { namespace Control {
				
	using namespace Sense;
	using namespace Sense::DLS;
	
	class CustomListView : public ListView
	{
	public:
		class CustomListViewItem : public VisibleItem
		{
		public:
			explicit CustomListViewItem(CustomListView& parent); 
			
		private:
			bool onMouseDown(IMouse::Button button, bool doubleClick) override;			
			CustomListView& _parent;
		};
		
		explicit CustomListView(Control &parent);
		
		Event<int> eventDoubleClick;		//TICS !INT#002: Event needs to be public so that it can be subscribed to.
	};

}}}