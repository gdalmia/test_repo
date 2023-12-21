// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "composite.h"
#include <Workflow/binding.h>
#include <gtest/gtest.h>

namespace Test{ namespace Binding {

	using namespace Philips::Workflow::Binding;
	
	class CollectionTest: public ::testing::Test
	{
	protected:

		void SetUp(const std::vector<Composite>& values)
		{
			_collection.set(values);
			subscribe();
		}

		void subscribe()
		{
			_collection.changed += [this](auto changedValues) {	_changedTriggered = true; _changedValues = changedValues; };
			_collection.added += [this](auto addedIndex) { _addedTriggered = true; _addedItemIndex = addedIndex; };
			_collection.removed += [this](auto removedItem) { _removedTriggered = true; _removedItem = removedItem; };
			_collection.updated += [this](auto updatedItemIndex) { _updatedTriggered = true; _updatedItemIndex = updatedItemIndex; };
		}

		void initialize(int count)
		{
			subscribe();

			std::vector<Composite> composites;
			for (int i = 1; i <= count; i++)
				composites.emplace_back(std::to_string(i));

			_collection.assign(composites);
		}

	public:
		Collection<Composite>		_collection				 ;				//TICS !INT#002: Needs to be public so that tests can access it
		bool						_changedTriggered = false;				//TICS !INT#002: Needs to be public so that tests can access it
		std::vector<Composite>		_changedValues			 ;				//TICS !INT#002: Needs to be public so that tests can access it
		bool						_addedTriggered   = false;				//TICS !INT#002: Needs to be public so that tests can access it
		int							_addedItemIndex			 ;				//TICS !INT#002: Needs to be public so that tests can access it
		bool						_removedTriggered = false;				//TICS !INT#002: Needs to be public so that tests can access it
		Composite					_removedItem			 ;				//TICS !INT#002: Needs to be public so that tests can access it
		bool						_updatedTriggered = false;				//TICS !INT#002: Needs to be public so that tests can access it
		int							_updatedItemIndex		 ;				//TICS !INT#002: Needs to be public so that tests can access it

	};

	TEST_F(CollectionTest, When_DefualtCollectionCreated_Then_ReturnsDefaultValues)
	{
		const Binding::Collection<Composite> collection;
		ASSERT_TRUE(collection.values().empty());
	}

	TEST_F(CollectionTest, When_ParameterizedCollectionCreated_Then_ReturnsPassedValues)
	{
		const std::vector<Composite> composites { Composite("1"), Composite("2")};
		const Binding::Collection<Composite> collection(composites);
		ASSERT_EQ(collection.values(), composites);
	}
	
	TEST_F(CollectionTest, When_ValuesAreSet_Then_ReturnsNewValues)
	{
		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		SetUp(composites);
		
		ASSERT_EQ(_collection.values(), composites);
	}

	TEST_F(CollectionTest, When_ValueAreSet_Then_VerifyOnlyChangedEventIsCalled)
	{
		subscribe();

		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		_collection.set(composites);

		ASSERT_TRUE(_changedTriggered);
		ASSERT_FALSE(_addedTriggered);
		ASSERT_FALSE(_removedTriggered);
		ASSERT_FALSE(_updatedTriggered);
	}

	TEST_F(CollectionTest, When_ValueAreSet_Then_ChangedEventIsCalledAndChangedValuesArePassedInEvent)
	{
		subscribe();

		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		_collection.set(composites);

		ASSERT_TRUE(_changedTriggered);
		ASSERT_EQ(_changedValues, composites);
	}
	
	TEST_F(CollectionTest, When_SameValuesAreSetAgain_Then_ChangedEventIsNotCalledSecondTime)
	{
		const std::vector<Composite> composites{ Composite("1"), Composite("2") };
		SetUp(composites);
		
		_collection.set(composites);
		ASSERT_FALSE(_changedTriggered);
	}

	TEST_F(CollectionTest, When_SameValuesAreAssignedAgain_Then_ChangedEventIsNotCalled)
	{
		const std::vector<Composite> composites{ Composite("1") };
		SetUp(composites);
		
		_collection.assign(composites);
		ASSERT_FALSE(_changedTriggered);
	}

	TEST_F(CollectionTest, When_NewValuesAreAssigned_Then_ValuesAreUpdated)
	{
		const std::vector<Composite> values{ Composite("1") };
		SetUp(values);

		const std::vector<Composite> newValues = { Composite("2") };
		_collection.assign(newValues);

		ASSERT_EQ(_collection.values(), newValues);
	}

	TEST_F(CollectionTest, When_SameValuesAreUpdatedAgain_Then_ChangedEventIsCalled)
	{
		const std::vector<Composite> composites{ Composite("1") };
		SetUp(composites);

		_collection.update(composites);
		ASSERT_TRUE(_changedTriggered);
	}

	TEST_F(CollectionTest, When_SameValuesAreUpdated_Then_ChangedAndNewValuesAreSame)
	{
		const std::vector<Composite> values{ Composite("1") };
		SetUp(values);

		_collection.update(values);

		ASSERT_EQ(values, _changedValues);
	}

	TEST_F(CollectionTest, Given_ValuesAreAssigned_When_EmplaceBackIsCalled_Then_VerifyItemIsAddedToLastIndex)
	{
		initialize(2);

		_collection.emplace_back(Composite("3"));

		ASSERT_EQ(_collection.values().size(), 3);
		ASSERT_EQ(_collection.values().back(), Composite("3"));
	}

	TEST_F(CollectionTest, Given_ValuesAreAssigned_When_EmplaceBackIsCalled_Then_VerifyOnlyAddedEventIsCalledAndAddedIndexIsCorrect)
	{
		initialize(2);

		_collection.emplace_back(Composite("3"));

		ASSERT_FALSE(_changedTriggered);
		ASSERT_TRUE(_addedTriggered);
		ASSERT_EQ(_addedItemIndex, 2);
	}
	
	TEST_F(CollectionTest, Given_ValuesAreAssigned_When_InsertIsCalledWithValidIndex_Then_VerifyItemInserted)
	{
		initialize(2);

		_collection.insert(Composite("3"), 1);

		ASSERT_EQ(_collection.values().size(), 3);
		ASSERT_EQ(_collection.values()[1], Composite("3"));
	}	

	TEST_F(CollectionTest, Given_ValuesAreAssigned_When_InsertIsCalledWithValidIndex_Then_VerifyOnlyAddedEventIsCalledAndAddedIndex)
	{
		initialize(2);

		_collection.insert(Composite("3"), 1);

		ASSERT_FALSE(_changedTriggered);
		ASSERT_TRUE(_addedTriggered);

		ASSERT_EQ(_addedItemIndex, 1);
	}

	TEST_F(CollectionTest, Given_ValuesAreAssigned_When_InsertIsCalledWithLastIndex_Then_VerifyOnlyAddedEventIsCalledAndAddedIndex)
	{
		initialize(2);
		
		_collection.insert(Composite("3"), 2);

		ASSERT_FALSE(_changedTriggered);
		ASSERT_TRUE(_addedTriggered);

		ASSERT_EQ(_addedItemIndex, 2);
	}

	TEST_F(CollectionTest, Given_ValuesAreAssigned_When_InsertIsCalledWithInvalidIndex_Then_VerifyAddedEventIsNotCalled)
	{
		initialize(2);

		_collection.insert(Composite("3"), 5);

		ASSERT_FALSE(_changedTriggered);
		ASSERT_FALSE(_addedTriggered);
	}

	TEST_F(CollectionTest, Given_ValuesAreAssigned_When_RemoveIsCalledWithValidIndex_Then_VerifyItemRemoved)
	{
		initialize(3);

		_collection.remove(1);

		ASSERT_EQ(_collection.values().size(), 2);
		ASSERT_EQ(_collection.values()[1], Composite("3"));
	}

	TEST_F(CollectionTest, Given_ValuesAreAssigned_When_RemoveIsCalledWithValidIndex_Then_VerifyOnlyRemovedEventIsCalledAndRemovedItem)
	{
		initialize(3);

		_collection.remove(1);

		ASSERT_FALSE(_changedTriggered);
		ASSERT_TRUE(_removedTriggered);

		ASSERT_EQ(_removedItem, Composite("2"));
	}

	TEST_F(CollectionTest, Given_ValuesAreAssigned_When_RemoveIsCalledWithInvalidIndex_Then_VerifyRemovedEventIsNotCalled)
	{
		initialize(3);

		_collection.remove( 5);

		ASSERT_FALSE(_changedTriggered);
		ASSERT_FALSE(_removedTriggered);
	}

	TEST_F(CollectionTest, Given_ValuesAreAssigned_When_UpdateItemIsCalledWithValidIndex_Then_VerifyItemUpdated)
	{
		initialize(3);

		_collection.updateItem(Composite("updated"), 1);

		ASSERT_EQ(_collection.values().size(), 3);
		ASSERT_EQ(_collection.values()[1], Composite("updated"));
	}

	TEST_F(CollectionTest, Given_ValuesAreAssigned_When_UpdateItemIsCalledWithValidIndex_Then_VerifyOnlyUpdatedEventIsCalledAndUpdatedItemIndex)
	{
		initialize(3);

		_collection.updateItem(Composite("updated"),1);

		ASSERT_FALSE(_changedTriggered);
		ASSERT_TRUE(_updatedTriggered);

		ASSERT_EQ(_updatedItemIndex, 1);
	}

	TEST_F(CollectionTest, Given_ValuesAreAssigned_When_UpdateItemIsCalledWithInvalidIndex_Then_VerifyUpdatedEventIsNotCalled)
	{
		initialize(3);

		_collection.updateItem(Composite("updated"), 5);

		ASSERT_FALSE(_changedTriggered);
		ASSERT_FALSE(_updatedTriggered);
	}
	enum OperatorTypes
	{
		CopyConstructor,
		MoveConstructor,
		AssignmentOperator,
		AssignmentMoveOperator,
	};
	class MultiCollectionTest : public CollectionTest,
		public ::testing::WithParamInterface<OperatorTypes>
	{
	public:
		void createObject()
		{
			switch(GetParam())
			{
			case OperatorTypes::CopyConstructor:
			{
				collectionnew = std::make_unique<Collection<Composite>>(_collection);
				break;
			}
			case OperatorTypes::MoveConstructor:
			{
				collectionnew = std::make_unique<Collection<Composite>>(std::move(_collection));
				break;
			}
			case OperatorTypes::AssignmentOperator:
			{
				Collection<Composite> newcollection;
				newcollection = _collection;
				collectionnew = std::make_unique<Collection<Composite>>(newcollection);
				break;
			}
			case OperatorTypes::AssignmentMoveOperator:
			{
				Collection<Composite> newcollection;
				newcollection = std::move(_collection);
				collectionnew = std::make_unique<Collection<Composite>>(std::move(newcollection));
				break;
			}
			}			
		}
		std::unique_ptr<Collection<Composite>> collectionnew;
	};

	INSTANTIATE_TEST_CASE_P(Given_ValidFormats, MultiCollectionTest, ::testing::Values(
		OperatorTypes::CopyConstructor,
		OperatorTypes::MoveConstructor,
		OperatorTypes::AssignmentOperator,
		OperatorTypes::AssignmentMoveOperator));

	TEST_P(MultiCollectionTest, Given_ValuesAreAssigned_When_InsertIsCalled_Then_VerifyItemInserted)
	{
		initialize(2);
		createObject();
		collectionnew.get()->insert(Composite("3"), 1);

		ASSERT_TRUE(_addedTriggered);
		ASSERT_EQ(collectionnew.get()->values().size(), 3);
		ASSERT_EQ(collectionnew.get()->values()[1], Composite("3"));
	}

	TEST_P(MultiCollectionTest, Given_ValuesAreAssigned_When_RemoveIsCalled_Then_VerifyOnlyRemovedEventIsCalledAndRemovedItem)
	{
		initialize(3);
		createObject();
		collectionnew.get()->remove(1);

		ASSERT_FALSE(_changedTriggered);
		ASSERT_TRUE(_removedTriggered);
		ASSERT_EQ(collectionnew.get()->values().size(), 2);
	}

	TEST_P(MultiCollectionTest, Given_ValuesAreAssigned_When_UpdateItemIsCalled_Then_VerifyOnlyUpdatedEventIsCalledAndUpdatedItemIndex)
	{
		initialize(3);
		createObject();
		collectionnew.get()->updateItem(Composite("updated"), 1);

		ASSERT_FALSE(_changedTriggered);
		ASSERT_TRUE(_updatedTriggered);
		ASSERT_EQ(_updatedItemIndex, 1);
	}

	TEST_P(MultiCollectionTest, Given_ValuesAreAssigned_When_AsignmentOperatorEmplaceBackIsCalled_Then_VerifyUpdatedData)
	{
		initialize(2);

		createObject();
		collectionnew.get()->emplace_back(Composite("3"));

		ASSERT_TRUE(_addedTriggered);
		ASSERT_EQ(collectionnew.get()->values().size(), 3);
	}
}}