// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include <workflow.h>

namespace Philips { namespace Workflow { namespace Binding {

	//! The Property template represents a value 
	//! It provides changed event when the value changes
	//! Composite types need to implement the comparison operator, assignment operator, and copy constructor.
	template<class Value>
	class Property
	{
	public:
		//! Create a property with default value
		//!
		//! \code
		//! Property<bool> enable;
		//!
		//! struct Composite;
		//! Property<Composite> value;
		//! \endcode
		Property():_value()
		{
		}
		
		//! Creates a property with a value
		//!
		//! \code
		//! Property<bool> enable(false)
		//!
		//! struct Composite { string id; };
		//! Property<Composite> value(Composite{"id"});
		//! \endcode
		explicit Property(const Value& val) : _value(val)
		{
		}
		
		~Property() = default;

		//! Returns the value of the property
		const Value& value() const
		{
			return _value;
		}

		//! Assigns a value to the property
		Property<Value> &operator=(const Value &value)
		{
			set(value);
			return *this;
		}

		//! Event is raised when the value is changed
		Event<Value> changed;

		Property(const Property&) = delete;
		Property(Property&&) = delete;
		Property& operator=(const Property&) = delete;
		Property& operator=(Property&&) = delete;
		
		//! Sets the value
		//! sets and triggers changed event only if the new value is not equal to previous value
		void set(const Value& value)
		{
			if (_value != value)
			{
				_value = value;
				changed(_value);
			}
		}

		//! assigns the value
		//! Sets and does not trigger changed event
		void assign(const Value& value)
		{
			_value = value;
		}

		//! Updates the value
		//! Sets and triggers changed event
		void update(const Value& value)
		{
			_value = value;
			changed(_value);
		}
		
	private:
		Value _value;
	};

	template class Property<bool>;
	template class Property<int>;
	template class Property<std::string>;
	template class Property<std::wstring>;
	
}}}
