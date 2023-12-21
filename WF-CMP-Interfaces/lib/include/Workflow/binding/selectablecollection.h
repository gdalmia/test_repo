// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "collection.h"

namespace Philips { namespace Workflow { namespace Binding {

	//! The SelectableCollection template represents a collection of values and selection 
	//! It provides changed event when the value changes, a selection changed event when the selection changes
	//! and method to bind to target entities
	//! Composite types need to implement the comparison operator, assignment operator, and copy constructor.
	template<class Value>
	class SelectableCollection: public Collection<Value>
	{
	public:
		typedef int Selection;
		
		//! Create a selectable collection
		//!
		//! \code
		//! SelectableCollection<std::string> values;
		//!
		//! struct Composite;
		//! SelectableCollection<Composite> composites;
		//! \endcode
		SelectableCollection():Collection<Value>(), _selection(-1)
		{
		}

		//! Creates a Value with value
		SelectableCollection(const typename Collection<Value>::Values& values, Selection selection)
			: Collection<Value>(values), _selection(-1)
		{
			select(selection);
		}

		~SelectableCollection() = default;

		//! Returns the index of the current
		Selection selection() const
		{
			return _selection;
		}

		//! Selects the item corresponding to the given index
		void select(const Selection selection)
		{
			if (selection < -1 || selection >= static_cast<int>(this->_values.size()))
			{
				return;
			}
			
			if (_selection != selection)
			{
				_selection = selection;
				selectionChanged(_selection);
			}
		}

		//! Event is raised when the selection is changed
		Event<Selection> selectionChanged;					//TICS !INT#002: Event needs to be public so that it can be subscribed

		SelectableCollection(const SelectableCollection&) = delete;
		SelectableCollection(SelectableCollection&&) = delete;
		SelectableCollection& operator=(const SelectableCollection&) = delete;
		SelectableCollection& operator=(SelectableCollection&&) = delete;

	private:

		void onInsert(const Value& value, const int index) override
		{
			Collection<Value>::onInsert(value, index);
			if (index <= _selection)
			{
				_selection++;
				selectionChanged(_selection);
			}
		}

		void onRemove(const int index) override
		{
			if (index <= _selection)
			{
				_selection = index == _selection ? -1 : _selection - 1;
				selectionChanged(_selection);
			}
			Collection<Value>::onRemove(index);
		}

		Selection _selection;
	};

}}}
