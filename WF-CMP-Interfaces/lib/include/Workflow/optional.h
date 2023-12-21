// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

namespace Philips { namespace Workflow {

	//! The Optional template represents a value which is not mandatory to be filled
	template <class T>
	class Optional
	{
	public:
		//! Default constructor which marks optional attribute as uninitialized
		Optional() 
			: _valid(false)
			, _value()
		{
		}

		//! Creates Optional object with the given value
		explicit Optional(const T& value) 
			: _valid(true)
			, _value(value)
		{
		}

		~Optional() = default;

		//! Creates Optional object with the given Optional attribute
		Optional(const Optional & optional)
			: _valid(optional._valid)
			, _value(optional._value)
		{
		}

		//! Overloaded assignment operator with given value
		Optional<T>& operator=(const T & value)
		{
			_valid = true;
			_value = value;
			return *this;
		}

		//! Overloaded != operator
		bool operator!= (const Optional<T> & optional) const
		{
			return !(*this == optional);
		}

		//! Overloaded < operator
		bool operator< (const Optional<T> & optional) const
		{
			if (_valid  && optional._valid) return _value < optional._value;
			if (!_valid && optional._valid) return true;
			return false;
		}

		//! Overloaded >= operator
		bool operator>= (const Optional<T> & optional) const
		{
			return !(*this < optional);
		}

		//! Overloaded > operator
		bool operator> (const Optional<T> & optional) const
		{
			if (_valid && optional._valid)  return _value > optional._value;
			if (_valid && !optional._valid) return true;
			return false;
		}

		//! Overloaded <= operator
		bool operator<= (const Optional<T> & optional) const
		{
			return !(*this > optional);
		}

		//! Overloaded comparison operator
		bool operator== (const Optional<T> & optional) const
		{
			if (!_valid && !optional._valid) return true;
			if (_valid && optional._valid) return _value == optional._value;
			return false;
		}

		//! Overloaded assignment operator with given Optional object
		Optional<T>& operator=(const Optional<T> & optional)
		{
			_valid = optional._valid;
			_value = optional._value;
			return *this;
		}

		//! Overloaded bool operator to not make the call of has_value() mandatory
		explicit operator bool() const
		{
			return _valid;
		}
		
		//! Returns the underline value if present, else throws an exception
		const T & value() const
		{
			if (_valid) return _value;
			throw std::string("Invalid value");
		}

		//! Returns the underline value if present, else returns default value passed to API
		const T & value_or(const T& defaultValue) const
		{
			if(_valid) return _value;
			else return defaultValue;
		}

		//! Invalidates the value
		void reset()
		{
			_valid = false;
		}

		//! Returns If object contains valid value
		bool has_value() const
		{
			return _valid;
		}

	private:
		bool _valid;
		T	 _value;
	};
}}

