// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "composite.h"
#include <Workflow/binding.h>
#include <gtest/gtest.h>

namespace Test{ namespace Binding {

	using namespace Philips::Workflow::Binding;
	
	class SelectableCollectionTest: public ::testing::Test
	{
	};

	TEST_F(SelectableCollectionTest, When_DefualtSelectableCollectionCreated_Then_ReturnsDefaultSelection)
	{
		const Binding::SelectableCollection<Composite> collection;
		ASSERT_EQ(collection.selection(), -1);
	}

	TEST_F(SelectableCollectionTest, When_ParameterizedCollectionCreated_Then_ReturnsPassedSelection)
	{
		const auto selection = 1;
		const std::vector<Composite> composites { Composite("1"), Composite("2")};
		const Binding::SelectableCollection<Composite> collection(composites, selection);
		ASSERT_EQ(collection.selection(), selection);
	}

	TEST_F(SelectableCollectionTest, When_ValuesAreSet__Then_ReturnsDefaultSelection)
	{
		Binding::SelectableCollection<Composite> collection;

		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		collection.set(composites);

		ASSERT_EQ(collection.selection(), -1);
	}

	TEST_F(SelectableCollectionTest, Given_CollectionWithValues_When_SelectionIsSet_Then_ReturnsNewSelection)
	{
		Binding::SelectableCollection<Composite> collection;
		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		collection.set(composites);

		const auto selection = 1;
		collection.select(selection);

		ASSERT_EQ(collection.selection(), selection);
	}

	TEST_F(SelectableCollectionTest, Given_CollectionWithValues_When_DefaultSelectionIsSet_Then_ReturnsDefaultSelection)
	{
		Binding::SelectableCollection<Composite> collection;
		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		collection.set(composites);

		collection.select(0);
		const auto selection = -1;
		collection.select(selection);

		ASSERT_EQ(collection.selection(), selection);
	}

	TEST_F(SelectableCollectionTest, Given_CollectionWithValues_When_SelectionIsSet_Then_SelectionChangedEventIsCalled)
	{
		Binding::SelectableCollection<Composite> collection;
		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		collection.set(composites);
		
		int newSelection;
		auto triggered{ false };
		collection.selectionChanged += [&triggered, &newSelection](const int& value)
		{
			triggered = true;
			newSelection = value;
		};

		const auto selection = 1;
		collection.select(selection);

		ASSERT_TRUE(triggered);
		ASSERT_EQ(newSelection, selection);
	}

	TEST_F(SelectableCollectionTest, When_SelectionIsAgain_Then_SelectionChangedEventIsNotCalled)
	{
		Binding::SelectableCollection<Composite> collection;
		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		collection.set(composites);
		const auto selection = 1;
		collection.select(selection);

		int newSelection;
		auto triggered{ false };
		collection.selectionChanged += [&triggered, &newSelection](const int& value)
		{
			triggered = true;
			newSelection = value;
		};

		collection.select(selection);
		ASSERT_FALSE(triggered);
	}

	//TICS -INT#027 -OAL#011
    // Suppressed as warning is thrown for all Testcases related to override specifier, "new" inside smart pointers 

	class SelectableCollectionSelectionOutOfBoundsTest: public ::testing::TestWithParam<int>
	{
	};

	INSTANTIATE_TEST_SUITE_P(When_SelectionIsSet, SelectableCollectionSelectionOutOfBoundsTest, ::testing::Values(-2, -5, 2, 5));

	TEST_P(SelectableCollectionSelectionOutOfBoundsTest, Then_SelectionChangedIsNotChanged)
	{
		Binding::SelectableCollection<Composite> collection;
		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		collection.set(composites);
		const auto selection = 1;
		collection.select(selection);

		collection.select(GetParam());
		ASSERT_EQ(collection.selection(), selection);
	}

	TEST_P(SelectableCollectionSelectionOutOfBoundsTest, Then_SelectionChangedEventIsNotCalled)
	{
		const auto selection = 1;
		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		Binding::SelectableCollection<Composite> collection(composites, selection);

		int newSelection;
		auto triggered{ false };
		collection.selectionChanged += [&triggered, &newSelection](const int& value)
		{
			triggered = true;
			newSelection = value;
		};

		collection.select(GetParam());
		ASSERT_FALSE(triggered);
	}

	struct SelectionChangedParams
	{
		int previousSelection;
		int index;
		bool selectionChangedTriggered;
		int newSelection;
	};

	class SelectableCollectionInsertTest : public ::testing::TestWithParam<SelectionChangedParams>
	{
	};

	INSTANTIATE_TEST_SUITE_P(Given_PreviousSelection, SelectableCollectionInsertTest, ::testing::Values(
		SelectionChangedParams{ 1, 0, true, 2 },
		SelectionChangedParams{ 1, 1, true, 2 },
		SelectionChangedParams{ 1, 2, false, 1 },
		SelectionChangedParams{ 2, 0, true, 3 },
		SelectionChangedParams{ 1, -1, false, 1},
		SelectionChangedParams{ 1, 5, false, 1},
		SelectionChangedParams{ -1, -1, false, -1}));

	TEST_P(SelectableCollectionInsertTest, Given_CollectionWithValues_When_InsertIsCalled_Then_VerifyResult)
	{
		Binding::SelectableCollection<Composite> collection;
		const std::vector<Composite> composites{ Composite("1"), Composite("2"), Composite("3") };
		collection.assign(composites);

		bool selectionChangedCalled = false;
		const auto testParams = GetParam();
		collection.select(testParams.previousSelection);

		collection.selectionChanged += [&](auto) { selectionChangedCalled = true; };

		collection.insert(Composite("4"), testParams.index);

		ASSERT_EQ(selectionChangedCalled, testParams.selectionChangedTriggered);
		ASSERT_EQ(collection.selection(), testParams.newSelection);
	}

	class SelectableCollectionRemoveTest : public ::testing::TestWithParam<SelectionChangedParams>
	{
	};

	INSTANTIATE_TEST_SUITE_P(Given_PreviousSelection, SelectableCollectionRemoveTest, ::testing::Values(
		SelectionChangedParams{ 2, 0, true, 1 },
		SelectionChangedParams{ 2, 1, true, 1 },
		SelectionChangedParams{ 2, 2, true, -1 },
		SelectionChangedParams{ 2, 3, false, 2 },
		SelectionChangedParams{ 1, 2, false, 1 },
		SelectionChangedParams{ 2, -1, false, 2 },
		SelectionChangedParams{ 2, 5, false, 2 },
		SelectionChangedParams{ -1, -1, false, -1 }));

	TEST_P(SelectableCollectionRemoveTest, Given_CollectionWithValues_When_RemoveIsCalled_Then_VerifyResult)
	{
		Binding::SelectableCollection<Composite> collection;
		const std::vector<Composite> composites{ Composite("1"), Composite("2"), Composite("3"), Composite("4") };
		collection.assign(composites);

		bool selectionChangedCalled = false;
		const auto testParams = GetParam();
		collection.select(testParams.previousSelection);

		collection.selectionChanged += [&](auto) { selectionChangedCalled = true; };

		collection.remove(testParams.index);

		ASSERT_EQ(selectionChangedCalled, testParams.selectionChangedTriggered);
		ASSERT_EQ(collection.selection(), testParams.newSelection);
	}

}}