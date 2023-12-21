// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "selectablecollection.h"

namespace Philips { namespace Workflow { namespace Binding {

	//! The EditableSelectableCollection template represents a collection of values, a selected value and selection 
	//! It provides changed event when the value changes, a selection changed event when the selection changes
	//! and method to bind to target entities
	template<class Value>
	class EditableSelectableCollection : public SelectableCollection<Value>
	{
	public:
		typedef int Selection;
		
		//! Create a editableSelectableCollection collection
		EditableSelectableCollection():SelectableCollection<Value>(), editableValue(Value())
		{}

		//! Creates a Selection with selection
		EditableSelectableCollection(const typename Collection<Value>::Values& values, Selection selection=-1)
			: SelectableCollection<Value>(values, selection), editableValue(Value()) 
		{}

		//! Creates a Value with value
		EditableSelectableCollection(const typename Collection<Value>::Values& values, Value value)
			: SelectableCollection<Value>(values,-1)
		{
			editableValue = value;
		}

		//! Creates a Selection with selection and Value with value
		EditableSelectableCollection(const typename Collection<Value>::Values& values, Selection selection, Value value)
			: SelectableCollection<Value>(values, selection)
		{
			editableValue = value;
		}

		Property<Value> editableValue;							//TICS !INT#002: Event needs to be public so that it can be subscribed

		~EditableSelectableCollection() = default;
		EditableSelectableCollection(const EditableSelectableCollection&) = delete;
		EditableSelectableCollection(EditableSelectableCollection&&) = delete;
		EditableSelectableCollection& operator=(const EditableSelectableCollection&) = delete;
		EditableSelectableCollection& operator=(EditableSelectableCollection&&) = delete;
	};
	
}}}
