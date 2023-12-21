// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include <Workflow/event.h>
#include <vector>

namespace Philips { namespace Workflow { namespace Binding {

	//! The Collection template represents a collection of values and selection 
	//! It provides changed event when the value changes and method to bind to target entities
	//! Composite types need to implement the comparison operator, assignment operator, and copy constructor.
	template<class Value>
	class Collection
	{
	public:
		typedef std::vector<Value> Values;

		//! Create a collection
		//!
		//! \code
		//! Collection<std::string> values;
		//!
		//! struct Composite;
		//! Collection<Composite> composites;
		//! \endcode
		Collection() :_values()
		{
		}

		//! Creates a Value with value
		explicit Collection(const Values& values) : _values(values)
		{
		}

		//! Copy constructor
		Collection(const Collection& other) : _values(other.values()), changed(other.changed), added(other.added), removed(other.removed), updated(other.updated)
		{
		}

		//! Move constructor
		Collection(Collection&& other) noexcept : _values(std::move(other.values())), changed(std::move(other.changed)), added(std::move(other.added)), removed(std::move(other.removed)), updated(std::move(other.updated))
		{
		}
		
		virtual ~Collection() = default;

		//! Returns the value of the collection
		const Values& values() const
		{
			return _values;
		}

		//! Sets the value
		//! Sets and triggers changed event only if the new values are not equal to previous values
		void set(const Values& values)
		{
			if (_values != values)
			{
				_values = values;
				changed(_values);
			}
		}

		//! Assigns the value
		//! Sets and does not trigger the changed event
		void assign(const Values& values)
		{
			_values = values;
		}

		//! Update the values
		//! Sets and triggers the changed event
		void update(const Values& values)
		{
			_values = values;
			changed(_values);
		}

		//! Adds a value at the end of the collection
		void emplace_back(const Value& value)
		{
			_values.emplace_back(value);
			added(static_cast<int>(_values.size()-1));
		}

		//! Inserts a value at the given index in the collection
		void insert(const Value& value, const int index)
		{
			if (inBounds(index))
			{
				onInsert(value, index);
			}

			if (index == static_cast<int>(_values.size()))
				emplace_back(value);
		}

		//! Removes the item at the given index from the collection
		void remove(const int index)
		{
			if (!inBounds(index))
				return;

			onRemove(index);
		}

		//! Updates the value at the given index in the collection
		void updateItem(const Value& value, const int index)
		{
			if (!inBounds(index))
				return;

			_values[index] = value;
			updated(index);
		}

		//! Event is raised when the value is changed
		Event<const Values&> changed;

		//! Event is raised when an item is added
		Event<int> added;

		//! Event is raised when an item is removed
		Event<const Value&> removed;

		//! Event is raised when an item is updated
		Event<int> updated;

		Collection& operator=(const Collection& other)
		{
			if(this != &other)
			{
				this->set(other.values());
				this->changed = other.changed;
				this->added = other.added;
				this->removed = other.removed;
				this->updated = other.updated;
			}
			return *this;
		};
		
		Collection& operator=(Collection&& other)
		{
			if(this != &other)
			{
				this->set(other.values());
				this->changed = std::move(other.changed);
				this->added = std::move(other.added);
				this->removed = std::move(other.removed);
				this->updated = std::move(other.updated);
			}
			
			return *this;
		};

	protected:
		virtual void onInsert(const Value& value, const int index)
		{
			_values.insert(_values.begin() + index, value);
			added(index);
		}
		
		virtual void onRemove(const int index)
		{
			auto removalIndex = _values.begin();
			std::advance(removalIndex, index);
			auto removedItem = *removalIndex;

			_values.erase(removalIndex);
			removed(removedItem);
		}

		bool inBounds(const int index)
		{
			return ((index >= 0) && (index < _values.size()));
		}
		Values _values;
	};
	
}}}
