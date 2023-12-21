// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include <Workflow/control/testpeer/toucheditboxpeer.h>
#include "view.h"
#include <Automation/Peers/Peer.h>
#include <Workflow/control/testpeer/editablecomboboxpeer.h>

namespace Test { namespace Binding {

	using namespace TestPeer;

	class ViewtTestPeer
	{
	public:
		explicit ViewtTestPeer(View& view);

		bool isFaderPage1Selected() const;
		bool isFaderPage2Selected() const;

		bool isStaticsPageSelected() const;
		bool isListsPageSelected() const;

		Peer<TabPage>		staticsPagePeer;
		Peer<TabPage>		listsPagePeer;

		Peer<Label>			wstringLabelPeer;
		Peer<Label>			stringLabelPeer;
		Peer<Label>			intLabelPeer;
		Peer<Label>			intStringLabelPeer;

		Peer<TouchEditBox>	wstringTouchEditBoxPeer;
		Peer<TouchEditBox>	stringTouchEditBoxPeer;
		Peer<TouchEditBox>	intTouchEditBoxPeer;
		Peer<TouchEditBox>	doubleTouchEditBoxPeer; //TICS !INT#002: Members need to be public so they can be accessed in tests
		Peer<TouchEditBox>	doubleTouchPrecisionEditBox; //TICS !INT#002: Members need to be public so they can be accessed in tests

		Peer<TouchEditBox>	wstringOnewayTouchEditBoxPeer;
		Peer<TouchEditBox>	stringOnewayTouchEditBoxPeer;
		
		Peer<RadioButton>	radioButton1Peer;
		Peer<RadioButton>	radioButton2Peer;
		Peer<RadioButton>	radioButton3Peer;

		Peer<CheckBox>		oneWayCheckBoxPeer;
		Peer<CheckBox>		disableCheckBoxPeer;
		Peer<CheckBox>		hideCheckBoxPeer;

		Peer<Button>		buttonPeer;

		Peer<ComboBox>		stringComboBoxPeer;
		Peer<ComboBox>		customComboBoxPeer;

		Peer<ComboBox>		onewayComboBoxPeer;

		Peer<ListView>		customListViewPeer;
		
		Peer<ListView>		customMultiSelectableListViewPeer;				//TICS !INT#002: Members need to be public so they can be accessed in tests
		Peer<ListView>		customOneWayMultiSelectableListViewPeer;		//TICS !INT#002: Members need to be public so they can be accessed in tests

		EditableComboBoxPeer twowayEditableComboBoxPeer;					//TICS !INT#002: Members need to be public so they can be accessed in tests

		Peer<Button>		buttonVisibilityPeer;					//TICS !INT#002: Members need to be public so they can be accessed in tests
		Peer<ImageBox>		imageBoxPeer;							//TICS !INT#002: Members need to be public so they can be accessed in tests
		Peer<ImageBox>		customImageBoxPeer;							//TICS !INT#002: Members need to be public so they can be accessed in tests

		Peer<Label>			visibilityLabelPeer;							//TICS !INT#002: Members need to be public so they can be accessed in tests

	private:
		Peer<FaderPage>		faderPage1Peer;
		Peer<FaderPage>		faderPage2Peer;

	};
	
}}