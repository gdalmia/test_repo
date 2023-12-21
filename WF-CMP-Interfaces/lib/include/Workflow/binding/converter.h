// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "collection.h"
#include "Workflow/stringconverter.h"
#include <iomanip>

namespace Philips { namespace Workflow { namespace Binding {

	struct Invert
	{
		bool operator()(bool value) const
		{
			return !value;
		}
	};

	struct StringToWstring
	{
		std::wstring operator()(const std::string& value) const
		{
			return convert(value);
		}
	};

	struct WstringToString
	{
		std::string operator()(const std::wstring& value) const
		{
			return convert(value);
		}
	};

	struct IntToWstring
	{
		std::wstring operator()(const int value) const
		{
			return std::to_wstring(value);
		}
	};

	struct WstringToInt
	{
		int operator()(const std::wstring& value) const
		{
			return (value.empty() ? 0 : std::stoi(value));
		}
	};

	struct DoubleToWstring
	{
		DoubleToWstring(const int setPrecision = 1) :_precision(setPrecision){}
		std::wstring operator()(const double value) const
		{
			std::stringstream stream;
			stream << std::fixed << std::setprecision(_precision) << value;
			return convert(stream.str());
		}
	private:
		int _precision;
	};

	struct WstringToDouble
	{
		double operator()(const std::wstring& value) const
		{
			return (value.empty() ? 0 : std::stod(value));
		}
	};

	struct StringCollectionToComboBoxItems
	{
		std::vector<ComboBox::Item> operator()(const Collection<std::string>::Values& values) const
		{
			std::vector<ComboBox::Item>items;
			items.reserve(values.size());
			for (auto& value : values)
			{
				items.emplace_back(convert(value));
			}
			return items;
		}
	};

	template <class Value>
	struct ValueToBool
	{
		explicit ValueToBool(Value value) : _value(value) {}
		bool operator()(Value selected) const
		{
			return selected == _value;
		}

		Value value() const
		{
			return _value;
		}

	private:
		Value _value;
	};

	template <class Value>
	struct ValuesToBool
	{
		explicit ValuesToBool(std::vector<Value> values) : _values(values) {}
		bool operator()(Value selected) const
		{
			return std::find(_values.begin(), _values.end(), static_cast<Value>(selected)) != _values.end();
		}
		
	private:
		std::vector<Value> _values;
	};
	
	template <class Value>
	struct ValueToImage
	{
		explicit ValueToImage(std::vector<std::pair<Value, Sense::Image>> values)
		{
			for (auto& value : values)
			{
				_valueImageMap.insert(value);
			}
		}

		Sense::Image operator()(const Value& selected) const
		{
			return _valueImageMap.find(selected) != _valueImageMap.end() ? _valueImageMap.at(selected) : Sense::Image();
		}

	private:
		std::map<Value, Sense::Image> _valueImageMap;
	};

	template <class Value>
	struct IsNotEmpty
	{
		bool operator()(std::vector<Value> values) const
		{
			return !values.empty();
		}

		bool operator()(Value value) const
		{
			return !value.empty();
		}
	};
	
}}}