// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include <Workflow/binding.h>

namespace Test { namespace Binding {

	using namespace Philips::Workflow::Binding;

	class ViewModel
	{
	public:

		enum class Pages
		{
			StaticsPage,
			ListsPage,
		};

		enum class LabelType
		{
			Int,
			String,
			Unknown
		};

		ViewModel();

		Binding::Property<std::wstring>			headerLabel;
		Binding::Property<std::wstring>			staticsPage;
		Binding::Property<std::wstring>			listsPage;
		
		Binding::Property<std::wstring>			wstringLabel;
		Binding::Property<std::string>			stringLabel;
		Binding::Property<int>					intLabel;

		Binding::Property<std::wstring>			wstringTouchEditBox;
		Binding::Property<std::string>			stringTouchEditBox;
		Binding::Property<int>					intTouchEditBox;
		Binding::Property<double>				doubleTouchEditBox; //TICS !INT#002: Members need to be public so they can be accessed in tests
		Binding::Property<double>				doubleTouchPrecisionEditBox; //TICS !INT#002: Members need to be public so they can be accessed in tests
		
		Binding::Property<std::wstring>			wstringOnewayTouchEditBox;
		Binding::Property<std::string>			stringOnewayTouchEditBox;

		Binding::Property<bool>					radiobutton1Checked;
		Binding::Property<bool>					radiobutton3Checked;

		Binding::Property<bool>					enableRadioButton3;
		Binding::Property<bool>					visibleRadioButton3;

		Binding::Property<bool>					oneWayCheckBox;
		Binding::Property<bool>					disableCheckBox;
		Binding::Property<bool>					hideCheckBox;
		
		
		Binding::Property<bool>					selectFaderPage;

		Binding::Property<Pages>				tabPage;      //TICS !INT#002: Members need to be public so they can be accessed in tests

		Binding::Property<LabelType>			labelType;      //TICS !INT#002: Members need to be public so they can be accessed in tests

		Binding::Command						button;
		Binding::Property<bool>                 buttonAccent; //TICS !INT#002: Members need to be public so they can be accessed in tests

		Binding::Command						visibilityButton;		//TICS !INT#002: Members need to be public so they can be accessed in tests
		Binding::Property<Image>                image;					//TICS !INT#002: Members need to be public so they can be accessed in tests
		Binding::Property<LabelType>            customImage;					//TICS !INT#002: Members need to be public so they can be accessed in tests
		Binding::Property<bool>                 labelVisible;
		
		struct CustomItem
		{
			bool isDefault;
			std::string name;
			bool status;
		};
		
		Binding::SelectableCollection<std::string>			stringComboBox;
		Binding::SelectableCollection<CustomItem>			customComboBox;

		Binding::Collection<CustomItem>						customList;
		Binding::MultiSelectableCollection<CustomItem>		customMultiSelectableList;
		Binding::MultiSelectableCollection<CustomItem>		customOneWayMultiSelectableList;	//TICS !INT#002: Members need to be public so they can be accessed in tests

		Binding::SelectableCollection<std::string>			oneWayComboBox;						//TICS !INT#002: Members need to be public so they can be accessed in tests

		Binding::EditableSelectableCollection<std::string>	twoWayEditableComboBox;				//TICS !INT#002: Members need to be public so they can be accessed in tests

		Binding::Property<std::string>						visibilityLabel;
		
	};
	
}}