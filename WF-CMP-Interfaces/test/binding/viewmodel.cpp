// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "viewmodel.h"

namespace Test { namespace Binding {

	ViewModel::ViewModel()
	: headerLabel(L"Binding")
	, staticsPage(L"Statics")
	, listsPage(L"Lists")
	, wstringLabel(L"label_wstring")
	, stringLabel("label_string")
	, intLabel(1)
	, wstringTouchEditBox(L"toucheditbox_wstring")
	, stringTouchEditBox("toucheditbox_string")
	, intTouchEditBox(2)
	, doubleTouchEditBox(2.0)
    , doubleTouchPrecisionEditBox(2.0)
	, wstringOnewayTouchEditBox(L"oneway_wstring")
	, stringOnewayTouchEditBox("oneway_string")
	, radiobutton1Checked(false)
	, radiobutton3Checked(false)
	, enableRadioButton3(true)
	, visibleRadioButton3(true)
	, disableCheckBox(false)
	, hideCheckBox(false)
	, selectFaderPage(true)
	, tabPage(Pages::StaticsPage)
	, labelType(LabelType::Int)
	, stringComboBox({"1", "2", "3"}, 0)
	, customComboBox({ {false, "c1", false}, {true, "c2", true} }, 0)
	, customList({ {false, "l1", false}, {true, "l2", true} })
	, customMultiSelectableList({ {false, "l1", false}, {true, "l2", true}, {false, "l3", true} },  {})
	, customOneWayMultiSelectableList({ {false, "l1", false}, {true, "l2", true}, {false, "l3", true} },  {})
	, oneWayComboBox({"item1", "item2", "item3"}, -1)
	, twoWayEditableComboBox({ "value1", "value2" }, -1)
	, visibilityLabel("")
	, image(Sense::Image())
	, customImage(LabelType::Int)
	, labelVisible(false)
	{
		button.execute = [this]
		{
			wstringTouchEditBox = std::wstring();
			stringTouchEditBox = std::string();
			intTouchEditBox = 0;
			doubleTouchEditBox = 0.0;
			button.canExecute = false;
		};

	}
}}