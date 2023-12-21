// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include "collection.h"
#include <set>

namespace Philips { namespace Workflow { namespace Binding {

	//! The MultiSelectableCollection template represents a collection of values and multiple selections 
	//! It provides a changed event when the value changes, selection changed event when the selections change
	//! and method to bind to target entities
	//! Composite types need to implement the comparison operator, assignment operator, and copy constructor.
	template<class Value>
	class MultiSelectableCollection: public Collection<Value>
	{
	public:
		typedef std::set<int> Selection;
		typedef int Focus;
		
		//! Create a Multi selectable collection
		//!
		//! \code
		//! MultiSelectableCollection<std::string> values;
		//!
		//! struct Composite;
		//! MultiSelectableCollection<Composite> composites;
		//! \endcode
		MultiSelectableCollection():Collection<Value>()
		{
		}

		//! Creates a Value with value
		MultiSelectableCollection(const typename Collection<Value>::Values& values, Selection selection)
			: Collection<Value>(values)
		{
			select(selection);
		}

		~MultiSelectableCollection() = default;

		//! Copy constructor
		MultiSelectableCollection(const MultiSelectableCollection& other) noexcept : Collection<Value>(other),
			selectionChanged(other.selectionChanged),
			_selection(other._selection)
		{
		}

		//! Move constructor
		MultiSelectableCollection(MultiSelectableCollection&& other) noexcept : Collection<Value>(std::move(other)),
			selectionChanged(std::move(other.selectionChanged)),
			_selection(std::move(other._selection))
		{
		}

		//! Returns the index of the current selections
		Selection selection() const
		{
			return _selection;
		}

		//! Selects the items in the collection with the given index
		void select(const Selection& selection)
		{
			Selection validSelection;
			for (auto value : selection)
			{
				if (Collection<Value>::inBounds(value))
				{
					validSelection.insert(value);
				}
			}

			if (_selection != validSelection)
			{
				_selection = validSelection;
				selectionChanged(_selection);
			}
		}

		//! Selects all items in the collection
		void selectAll()
		{
			Selection selections;
			for (auto value = 0; value < Collection<Value>::_values.size(); value++)
			{
				selections.insert(value);
			}
			select(selections);
		}

		//! Deselects all items in the collection
		void deselectAll()
		{
			select({});
		}

		//! Event is raised when the selections have changed
		Event<Selection> selectionChanged;			//TICS !INT#002: Event needs to be public so that it can be subscribed

		MultiSelectableCollection& operator=(const MultiSelectableCollection& other)
		{
			if (this != &other)
			{
				Collection<Value>::operator=(other);
				this->selectionChanged = other.selectionChanged;
				this->_selection = other._selection;
			}
			return *this;
		};

		MultiSelectableCollection& operator=(MultiSelectableCollection&& other) noexcept
		{
			if (this != &other)
			{
				Collection<Value>::operator=(std::move(other));
				this->selectionChanged = std::move(other.selectionChanged);
				this->_selection = std::move(other._selection);
			}
			return *this;
		};

	private:

		void onInsert(const Value& value, const int index) override
		{
			Collection<Value>::onInsert(value, index);
			Selection newSelection;
			for (auto selection : _selection)
			{
				if (index <= selection)
					selection++;

				newSelection.insert(selection);
			}
			updateSelection(newSelection);
		}

		void onRemove(const int index) override
		{
			Selection newSelection;
			for (auto selection : _selection)
			{
				if (selection > index)
					newSelection.insert(selection - 1);

				if (selection < index)
					newSelection.insert(selection);
			}
			updateSelection(newSelection);
			Collection<Value>::onRemove(index);
		}

		void updateSelection(Selection updatedSelection)
		{
			if (_selection != updatedSelection)
			{
				_selection = updatedSelection;
				selectionChanged(_selection);
			}
		}
		
		Selection	_selection;
	};
	
}}}
