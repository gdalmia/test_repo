// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "testpeer.h"

namespace Test { namespace Binding {

	ViewtTestPeer::ViewtTestPeer(View& view)
		: staticsPagePeer(view.staticsPage)
		  , listsPagePeer(view.listsPage)
		  , wstringLabelPeer(view.wstringLabel)
		  , stringLabelPeer(view.stringLabel)
		  , intLabelPeer(view.intLabel)
          , intStringLabelPeer(view.intStringLabel)
		  , wstringTouchEditBoxPeer(view.wstringTouchEditBox)
		  , stringTouchEditBoxPeer(view.stringTouchEditBox)
		  , intTouchEditBoxPeer(view.intTouchEditBox)
		  , doubleTouchEditBoxPeer(view.doubleTouchEditBox)
		  , doubleTouchPrecisionEditBox(view.doublePrecisionEditBox)
		  , wstringOnewayTouchEditBoxPeer(view.wstringOnewayTouchEditBox)
		  , stringOnewayTouchEditBoxPeer(view.stringOnewayTouchEditBox)
		  , radioButton1Peer(view.radiobutton1)
		  , radioButton2Peer(view.radiobutton2)
		  , radioButton3Peer(view.radiobutton3)
		  , oneWayCheckBoxPeer(view.oneWayCheckBox)
		  , disableCheckBoxPeer(view.disableCheckBox)
		  , hideCheckBoxPeer(view.hideCheckBox)
		  , buttonPeer(view.button)
		  , stringComboBoxPeer(view.stringComboBox)
		  , customComboBoxPeer(view.customComboBox)
		  , onewayComboBoxPeer(view.onewayComboBox)
		  , customListViewPeer(view.customListView)
		  , customMultiSelectableListViewPeer(view.customMultiSelectableListView)
		  , customOneWayMultiSelectableListViewPeer(view.customOneWayMultiSelectableListView)
		  , twowayEditableComboBoxPeer(view.twowayEditableComboBox)
		  , buttonVisibilityPeer(view.visibilityButton)
		  , imageBoxPeer(view.imageBox)
		  , customImageBoxPeer(view.customImageBox)
		  , faderPage1Peer(view.faderPage1)
		  , faderPage2Peer(view.faderPage2)
		  , visibilityLabelPeer(view.visibilityLabel)
	{
	}

	bool ViewtTestPeer::isFaderPage1Selected() const
	{
		return faderPage1Peer.control.isSelected();
	}

	bool ViewtTestPeer::isFaderPage2Selected() const
	{
		return faderPage2Peer.control.isSelected();
	}

	bool ViewtTestPeer::isStaticsPageSelected() const
	{
		return staticsPagePeer.control.isSelected();
	}

	bool ViewtTestPeer::isListsPageSelected() const
	{
		return listsPagePeer.control.isSelected();
	}
}}