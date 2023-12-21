// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "composite.h"
#include <Workflow/binding.h>
#include <gtest/gtest.h>

namespace Test{ namespace Binding {

	using namespace Philips::Workflow::Binding;
	
	class PropertyTest: public ::testing::Test
	{
	protected:
		
		void SetUp(const Composite composite)
		{
			_property = composite;
			_property.changed += [this](const Composite&)
			{
				triggered = true;
			};
		}

	public:
		Binding::Property<Composite> _property; //TICS !INT#002: Needs to be public so that tests can access it
		bool triggered = false; //TICS !INT#002: Needs to be public so that tests can access it
	};

	TEST_F(PropertyTest, When_DefualtCustomValuePropertyCreated_Then_ReturnsDefaultValue)
	{
		ASSERT_EQ(_property.value().id, "default");
	}

	TEST_F(PropertyTest, When_ParameterizedCustomValuePropertyCreated_Then_ReturnsPassedValue)
	{
		Composite custom;
		custom.id = "value";
		const Binding::Property<Composite> propertyParam(custom);
		ASSERT_EQ(propertyParam.value(), custom);
	}

	TEST_F(PropertyTest, When_ValueIsSet_Then_ReturnsNewValue)
	{
		const auto custom = Composite("value");
		_property = custom;
		ASSERT_EQ(_property.value(), custom);
	}

	TEST_F(PropertyTest, When_ValueIsSet_Then_ChangedEventIsCalled)
	{
		Composite newValue;
		_property.changed += [this, &newValue](const Composite& value)
		{
			triggered = true;
			newValue = value;
		};

		const auto custom = Composite("value");
		_property = custom;
		
		ASSERT_TRUE(triggered);
		ASSERT_EQ(newValue, custom);
	}

	TEST_F(PropertyTest, When_SameValueIsSetAgain_Then_ChangedEventIsNotCalledSecondTime)
	{
		const auto custom = Composite("value");
		SetUp(custom);
		_property = custom;
		ASSERT_FALSE(triggered);
	}

	TEST_F(PropertyTest, When_SameValueIsAssignedAgain_Then_ChangedEventIsNotCalled)
	{
		const auto assignVariable = Composite("value");
		SetUp(assignVariable);
		_property.assign(assignVariable);
		ASSERT_FALSE(triggered);
	}

	TEST_F(PropertyTest, Given_UpdateMethod_When_SameValueIsUpdated_Then_ChangedEventCalled)
	{
		const auto updateVariable = Composite("value");
		SetUp(updateVariable);
		_property.update(updateVariable);
		ASSERT_TRUE(triggered);
	}


	TEST_F(PropertyTest, When_SameValueIsUpdated_Then_ChangedAndNewValueAreSame)
	{
		Binding::Property<Composite> propertyLocal;
		auto updateVariable = Composite("value");
		propertyLocal = updateVariable;
		Composite newUpdatedValue;
		propertyLocal.changed += [&newUpdatedValue](const Composite& value)
		{
			newUpdatedValue = value;
		};
		updateVariable.id = "updated";
		
		propertyLocal.update(updateVariable);

		ASSERT_EQ(newUpdatedValue, updateVariable);
	}
	
	TEST_F(PropertyTest, Given_SetMethod_When_DifferentValueIsSet_Then_ChangedEventIsCalled)
	{
		const auto setVariable = Composite("value");
		SetUp(Composite());

		_property.set(setVariable);

		ASSERT_TRUE(triggered);
	}

	TEST_F(PropertyTest, Give_SetMethod_When_SameValueIsSet_Then_ChangedEventIsNotCalled)
	{
		const auto setVariable = Composite("value");
		SetUp(setVariable);
		_property.set(setVariable);
		ASSERT_FALSE(triggered);
	}
}}
