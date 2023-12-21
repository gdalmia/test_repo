// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "composite.h"
#include <Workflow/binding.h>
#include <gtest/gtest.h>

namespace Test{ namespace Binding {

	using namespace Philips::Workflow::Binding;
	
	class MultiSelectableCollectionTest: public ::testing::Test
	{
	protected:
		const std::vector<Composite> composites{ Composite("1"), Composite("2"), Composite("3") };

	};

	TEST_F(MultiSelectableCollectionTest, When_DefualtSelectableCollectionCreated_Then_ReturnsDefaultSelection)
	{
		const Binding::MultiSelectableCollection<Composite> collection;
		ASSERT_EQ(collection.selection().size(), 0);
	}

	TEST_F(MultiSelectableCollectionTest, Given_SingleSelection_When_ParameterizedCollectionCreated_Then_ReturnsPassedSelection)
	{
		const std::set<int> selection = { 1 };
		const Binding::MultiSelectableCollection<Composite> collection(composites, selection);
		ASSERT_EQ(collection.selection(), selection);
	}

	TEST_F(MultiSelectableCollectionTest, Given_MultipleSelections_When_ParameterizedCollectionCreated_Then_ReturnsPassedSelection)
	{
		const std::set<int> selection = { 1, 2 };
		const Binding::MultiSelectableCollection<Composite> collection(composites, selection);
		ASSERT_EQ(collection.selection(), selection);
	}

	TEST_F(MultiSelectableCollectionTest, When_ValuesAreSet__Then_ReturnsDefaultSelection)
	{
		Binding::MultiSelectableCollection<Composite> collection;

		collection.set(composites);

		ASSERT_EQ(collection.selection().size(), 0);
	}
	
	TEST_F(MultiSelectableCollectionTest, Given_CollectionWithValues_When_MultipleSelectionsAreSet_Then_ReturnsNewSelection)
	{
		Binding::MultiSelectableCollection<Composite> collection;
		collection.set(composites);

		const std::set<int> selection = { 1, 2 };
		collection.select(selection);

		ASSERT_EQ(collection.selection(), selection);
	}

	TEST_F(MultiSelectableCollectionTest, Given_CollectionWithValues_When_SelectionIsSet_Then_SelectionChangedEventIsCalled)
	{
		Binding::MultiSelectableCollection<Composite> collection;
		collection.set(composites);
		
		std::set<int> newSelection;
		auto triggered{ false };
		collection.selectionChanged += [&triggered, &newSelection](const std::set<int>& value)
		{
			triggered = true;
			newSelection = value;
		};

		const std::set<int> selection = { 1, 2 };
		collection.select(selection);

		ASSERT_TRUE(triggered);
		ASSERT_EQ(newSelection, selection);
	}

	TEST_F(MultiSelectableCollectionTest, Given_CollectionWithValues_When_SelectionIsSetAgain_Then_SelectionChangedEventIsNotCalled)
	{
		Binding::MultiSelectableCollection<Composite> collection;
		collection.set(composites);
		const std::set<int> selection = { 1, 2 };
		collection.select(selection);

		std::set<int> newSelection;
		auto triggered{ false };
		collection.selectionChanged += [&triggered, &newSelection](const std::set<int>& value)
		{
			triggered = true;
			newSelection = value;
		};

		collection.select(selection);
		ASSERT_FALSE(triggered);
	}

	TEST_F(MultiSelectableCollectionTest, Given_CollectionWithValues_When_SelectAllCalled_Then_SelectionChangedEventIsCalled)
	{
		Binding::MultiSelectableCollection<Composite> collection;
		collection.set(composites);

		std::set<int> newSelection;
		auto triggered{ false };
		collection.selectionChanged += [&triggered, &newSelection](const std::set<int>& value)
		{
			triggered = true;
			newSelection = value;
		};

		collection.selectAll();

		ASSERT_TRUE(triggered);
	}

	TEST_F(MultiSelectableCollectionTest, Given_CollectionWithValues_When_SelectAllCalled_Then_AllValuesSelected)
	{
		Binding::MultiSelectableCollection<Composite> collection;
		collection.set(composites);

		collection.selectAll();
		const std::set<int> allItems = { 0,1,2 };
		ASSERT_EQ(collection.selection().size(), composites.size());
		ASSERT_EQ(collection.selection(), allItems);
	}

	TEST_F(MultiSelectableCollectionTest, Given_CollectionWithValuesAndAllValuesSelected_When_SelectAllCalled_Then_AllValuesRemainSelected)
	{
		Binding::MultiSelectableCollection<Composite> collection;
		collection.set(composites);

		const std::set<int> allValues = {0,1,2};
		collection.select(allValues);

		collection.selectAll();
		ASSERT_EQ(collection.selection().size(), composites.size());
		ASSERT_EQ(collection.selection(), allValues);
	}

	TEST_F(MultiSelectableCollectionTest, Given_CollectionWithValuesAndAllValuesSelected_When_SelectAllCalled_Then_SelectionChangedEventNotRaised)
	{
		Binding::MultiSelectableCollection<Composite> collection;
		collection.set(composites);

		std::set<int> newSelection = { 0,1,2 };
		collection.select(newSelection);
		auto triggered{ false };
		collection.selectionChanged += [&triggered, &newSelection](const std::set<int>& value)
		{
			triggered = true;
			newSelection = value;
		};

		collection.selectAll();

		ASSERT_FALSE(triggered);
	}

	TEST_F(MultiSelectableCollectionTest, Given_CollectionWithValuesAndAllSelected_When_DeselectAllCalled_Then_AllValuesDeselected)
	{
		Binding::MultiSelectableCollection<Composite> collection;
		collection.set(composites);

		collection.selectAll();

		collection.deselectAll();
		ASSERT_EQ(collection.selection().size(), 0);
	}

	TEST_F(MultiSelectableCollectionTest, Given_CollectionWithValuesAndAllValuesDeselected_When_DeselectAllCalled_Then_AllValuesRemainDeselected)
	{
		Binding::MultiSelectableCollection<Composite> collection;
		collection.set(composites);

		collection.deselectAll();
		ASSERT_EQ(collection.selection().size(), 0);
	}

	TEST_F(MultiSelectableCollectionTest, Given_CollectionWithValuesAndAllValuesDeselected_When_DeselectAllCalled_Then_SelectionChangedEventNotRaised)
	{
		Binding::MultiSelectableCollection<Composite> collection;
		collection.set(composites);

		std::set<int> newSelection;
		auto triggered{ false };
		collection.selectionChanged += [&triggered, &newSelection](const std::set<int>& value)
		{
			triggered = true;
			newSelection = value;
		};

		collection.deselectAll();
		ASSERT_FALSE(triggered);
	}
	
	TEST_F(MultiSelectableCollectionTest, Given_SetOfInvalidSelection_When_SelectionIsSet_Then_SelectionIsSetToEmpty)
	{
		Binding::MultiSelectableCollection<Composite> collection;
		collection.set(composites);
		const std::set<int> selection = { -2, -1, 3, 5 };
		collection.select(selection);
		ASSERT_EQ(collection.selection().size(), 0);
	}

	TEST_F(MultiSelectableCollectionTest, Given_SetOfValidAndInvalidSelections_When_SelectionIsSet_Then_SelectionIsSetToValidSelectionsOnly)
	{
		Binding::MultiSelectableCollection<Composite> collection;
		collection.set(composites);
		const std::set<int> selection = { -2, -1, 5, 0, 1 };
		const std::set<int> validSelection = { 0, 1 };
		collection.select(selection);
		ASSERT_EQ(collection.selection(), validSelection);
	}

	struct MultiSelectionChangedParams
	{
		std::set<int> previousSelection;
		int index;
		bool selectionChangedTriggered;
		std::set<int> newSelection;
	};

	class MultiSelectableCollectionInsertTest : public ::testing::TestWithParam<MultiSelectionChangedParams>
	{
	};

	INSTANTIATE_TEST_SUITE_P(Given_PreviousSelection, MultiSelectableCollectionInsertTest, ::testing::Values(
		MultiSelectionChangedParams{ {}, 3, false,  {}},
		MultiSelectionChangedParams{ {}, 0, false,  {}},
		MultiSelectionChangedParams{ {1,2}, 0, true,  {2,3}},
		MultiSelectionChangedParams{ {1}, 1, true, {2} },
		MultiSelectionChangedParams{ {0, 1}, 2, false, {0, 1} },
		MultiSelectionChangedParams{ {0}, 4, false, {0} },
		MultiSelectionChangedParams{ {0, 1, 2}, 3, false, {0, 1, 2} },
		MultiSelectionChangedParams{ {0, 2, 3}, 0, true, {1,3,4} },
		MultiSelectionChangedParams{ {0, 2, 3}, 3, true, {0,2,4} },
		MultiSelectionChangedParams{ {0, 2, 3}, 1, true, {0,3,4} },
		MultiSelectionChangedParams{ {0, 2, 3}, 2, true, {0,3,4} },
		MultiSelectionChangedParams{ {0,1}, -1, false, {0,1} },
		MultiSelectionChangedParams{ {0,1}, 7, false, {0, 1} },
		MultiSelectionChangedParams{ {}, -1, false, {} }));

	TEST_P(MultiSelectableCollectionInsertTest, Given_CollectionWithValues_When_InsertIsCalled_Then_VerifyResult)
	{
		Binding::MultiSelectableCollection<Composite> multiSelectableCollection;
		const std::vector<Composite> composites{ Composite("1"), Composite("2"), Composite("3"), Composite("4"), Composite("5") };
		multiSelectableCollection.assign(composites);

		bool selectionChangedCalled = false;
		const auto testParams = GetParam();
		multiSelectableCollection.select(testParams.previousSelection);

		multiSelectableCollection.selectionChanged += [&](auto) { selectionChangedCalled = true; };

		multiSelectableCollection.insert(Composite("6"), testParams.index);

		ASSERT_EQ(selectionChangedCalled, testParams.selectionChangedTriggered);
		ASSERT_EQ(multiSelectableCollection.selection(), testParams.newSelection);
	}

	class MultiSelectableCollectionRemoveTest : public ::testing::TestWithParam<MultiSelectionChangedParams>
	{
	};

	INSTANTIATE_TEST_SUITE_P(Given_PreviousSelection, MultiSelectableCollectionRemoveTest, ::testing::Values(
		MultiSelectionChangedParams{ {}, 3, false,  {} },
		MultiSelectionChangedParams{ {}, 0, false,  {} },
		MultiSelectionChangedParams{ {1,2}, 0, true,  {0,1} },
		MultiSelectionChangedParams{ {1}, 1, true, {} },
		MultiSelectionChangedParams{ {0, 1}, 2, false, {0, 1} },
		MultiSelectionChangedParams{ {0}, 4, false, {0} },
		MultiSelectionChangedParams{ {0, 1, 2}, 3, false, {0, 1, 2} },
		MultiSelectionChangedParams{ {0, 2, 3}, 0, true, {1,2} },
		MultiSelectionChangedParams{ {0, 2, 3}, 3, true, {0,2} },
		MultiSelectionChangedParams{ {0, 2, 3}, 1, true, {0,1,2} },
		MultiSelectionChangedParams{ {0, 2, 3}, 2, true, {0,2} },
		MultiSelectionChangedParams{ {0,1}, -1, false, {0,1} },
		MultiSelectionChangedParams{ {0,1}, 7, false, {0, 1} },
		MultiSelectionChangedParams{ {}, -1, false, {} }));

	TEST_P(MultiSelectableCollectionRemoveTest, Given_CollectionWithValues_When_RemoveIsCalled_Then_VerifyResult)
	{
		Binding::MultiSelectableCollection<Composite> multiSelectableCollection;
		const std::vector<Composite> composites{ Composite("1"), Composite("2"), Composite("3"), Composite("4") };
		multiSelectableCollection.assign(composites);

		bool selectionChangedCalled = false;
		const auto testParams = GetParam();
		multiSelectableCollection.select(testParams.previousSelection);

		multiSelectableCollection.selectionChanged += [&](auto) { selectionChangedCalled = true; };

		multiSelectableCollection.remove(testParams.index);

		ASSERT_EQ(selectionChangedCalled, testParams.selectionChangedTriggered);
		ASSERT_EQ(multiSelectableCollection.selection(), testParams.newSelection);
	}


	enum OperatorTypes
	{
		CopyConstructor,
		MoveConstructor,
		AssignmentOperator,
		AssignmentMoveOperator,
	};

	class MultiSelectableCollectionConstructorTest : public MultiSelectableCollectionTest,
		public ::testing::WithParamInterface<OperatorTypes>
	{
	public:
		void createObject()
		{
			switch (GetParam())
			{
			case OperatorTypes::CopyConstructor:
			{
				collectionNew = std::make_unique<MultiSelectableCollection<Composite>>(_collection);
				break;
			}
			case OperatorTypes::MoveConstructor:
			{
				collectionNew = std::make_unique<MultiSelectableCollection<Composite>>(std::move(_collection));
				break;
			}
			case OperatorTypes::AssignmentOperator:
			{
				MultiSelectableCollection<Composite> newCollection;
				newCollection = _collection;
				collectionNew = std::make_unique<MultiSelectableCollection<Composite>>(newCollection);
				break;
			}
			case OperatorTypes::AssignmentMoveOperator:
			{
				MultiSelectableCollection<Composite> newCollection;
				newCollection = std::move(_collection);
				collectionNew = std::make_unique<MultiSelectableCollection<Composite>>(std::move(newCollection));
				break;
			}
			}
		}

		void initialize()
		{
			_collection.assign(composites);
			_collection.select(std::set<int>{2});
		}

		MultiSelectableCollection<Composite>	            	_collection;
		std::unique_ptr<MultiSelectableCollection<Composite>>   collectionNew;
		
	};                 
	
	INSTANTIATE_TEST_CASE_P(Given_ValidFormats, MultiSelectableCollectionConstructorTest, ::testing::Values(
		OperatorTypes::CopyConstructor,
		OperatorTypes::MoveConstructor,
		OperatorTypes::AssignmentOperator,
		OperatorTypes::AssignmentMoveOperator));


	TEST_P(MultiSelectableCollectionConstructorTest, Given_ValuesAreAssigned_Then_VerifyItemsSelection)
	{
		initialize();
		createObject();

		ASSERT_EQ(collectionNew->selection().size(), 1);
		ASSERT_EQ(*collectionNew->selection().begin(), 2);
	}

	TEST_P(MultiSelectableCollectionConstructorTest, Given_ValuesAreAssigned_When_ItemsSelectionIsChanged_Then_SelectionChangedEventIsRaised)
	{
		bool selectionChangedTriggered = false;
		_collection.selectionChanged += [&](auto) { selectionChangedTriggered = true; };

		initialize();
		createObject();

		collectionNew->selectAll();

		ASSERT_TRUE(selectionChangedTriggered);
		ASSERT_EQ(collectionNew->selection().size(), 3);
	}

	TEST_P(MultiSelectableCollectionConstructorTest, Given_ValuesAreAssigned_Then_VerifyCollectionItems)
	{
		initialize();
		createObject();

		ASSERT_EQ(collectionNew->values().size(), 3);
		ASSERT_EQ(collectionNew->values()[0], Composite("1"));
		ASSERT_EQ(collectionNew->values()[1], Composite("2"));
		ASSERT_EQ(collectionNew->values()[2], Composite("3"));
	}

}}