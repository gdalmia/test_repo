// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "composite.h"
#include <Workflow/binding.h>
#include <gtest/gtest.h>

namespace Test{ namespace Binding {

	using namespace Philips::Workflow::Binding;
	
	class EditableSelectableCollectionTest: public ::testing::Test
	{
	};

	TEST_F(EditableSelectableCollectionTest, Given_EditableSelectableCollection_When_DefualtEditableSelectableCollectionCreated_Then_ReturnsDefaultValue)
	{
		const Binding::EditableSelectableCollection<Composite> collection;
		ASSERT_EQ(collection.editableValue.value(), Composite());
	}

	TEST_F(EditableSelectableCollectionTest, Given_EditableSelectableCollection_When_CollectionAndSelectionParameterizedCollectionCreated_Then_ReturnsDefaultValue)
	{
		const auto selection = 1;
		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		const Binding::EditableSelectableCollection<Composite> collection(composites, selection);
		ASSERT_EQ(collection.editableValue.value(), Composite());
	}

	TEST_F(EditableSelectableCollectionTest, Given_EditableSelectableCollection_When_CollectionAndValueParameterizedCollectionCreated_Then_ReturnsPassedValueAndSelectionIsDefault)
	{
		const Composite value("value");
		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		const Binding::EditableSelectableCollection<Composite> collection(composites, value);
		ASSERT_EQ(collection.editableValue.value(), value);
		ASSERT_EQ(collection.selection(), -1);
	}

	TEST_F(EditableSelectableCollectionTest, Given_EditableSelectableCollection_When_CollectionWitSelectionAndValueParameterizedCollectionCreated_Then_ReturnsPassedValue)
	{
		const Composite value("value");
		const auto selection = 1;
		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		const Binding::EditableSelectableCollection<Composite> collection(composites, selection, value);
		ASSERT_EQ(collection.editableValue.value(), value);
	}

	TEST_F(EditableSelectableCollectionTest,Given_EditableSelectableCollection_When_valueIsSet_Then_ReturnsSetValue)
	{
		const Composite value("value");
		Binding::EditableSelectableCollection<Composite> collection;
		collection.editableValue = value;
		ASSERT_EQ(collection.editableValue.value(), value);
	}

	TEST_F(EditableSelectableCollectionTest, Given_EditableSelectableCollection_When_ValueIsSet_Then_valueChangedEventIsCalled)
	{
		Composite value("value");
		Binding::EditableSelectableCollection<Composite> collection;
		bool triggered = false;
		collection.editableValue.changed += [&triggered](const Composite&)
		{
			triggered = true;
		};
		collection.editableValue =value;
		ASSERT_TRUE(triggered);
	}
}}