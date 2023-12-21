// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>

#include "workflow.h"

#include <chrono>

namespace EventTest
{
	using namespace Philips::Workflow;

	struct TestArgument
	{
		TestArgument() : _value(false) { }
		explicit TestArgument(bool value) : _value(value) { }
		bool _value;
	};

	struct TestHandler
	{
		TestHandler() : _triggered(false) { }
		void onTriggered() { _triggered = true; }
		void onTriggeredWithArgument(TestArgument argument) { _triggeredArgument = argument; }
		
		TestArgument _triggeredArgument;
		bool _triggered;
	};

	namespace WhenEventIsTriggered
	{
		class ThenHandlersInvoked : public testing::Test
		{
		};

		TEST_F(ThenHandlersInvoked, Given_LambdaSubscriptionUsingSubscribeMethod)
		{
			const Event<> event;
			auto triggered1{ false };
			event.subscribe([&triggered1]() { triggered1 = true; });

			auto triggered2{ false };
			event.subscribe([&triggered2]() { triggered2 = true; });

			event();

			ASSERT_TRUE(triggered1);
			ASSERT_TRUE(triggered2);
		}

		TEST_F(ThenHandlersInvoked, Given_LambdaWithArgumentSubscriptionUsingSubscribeMethod)
		{
			const Event<TestArgument> event;

			TestArgument triggered1;
			event.subscribe([&triggered1](TestArgument argument) { triggered1 = argument; });

			TestArgument triggered2;
			event.subscribe([&triggered2](TestArgument argument) { triggered2 = argument; });

			const TestArgument argument{ true };
			event(argument);

			ASSERT_TRUE(triggered1._value);
			ASSERT_TRUE(triggered2._value);
		}

		TEST_F(ThenHandlersInvoked, Given_ClassMemberSubscriptionUsingSubscribeMethod)
		{
			const Event<> event;
			TestHandler handler1;
			event.subscribe(std::bind(&TestHandler::onTriggered, &handler1));

			TestHandler handler2;
			event.subscribe(std::bind(&TestHandler::onTriggered, &handler2));

			event();

			ASSERT_TRUE(handler1._triggered);
			ASSERT_TRUE(handler2._triggered);
		}

		TEST_F(ThenHandlersInvoked, Given_ClassMemberWithArgumentsSubscriptionUsingSubscribeMethod)
		{
			const Event<TestArgument> event;
			TestHandler handler1;
			event.subscribe(std::bind(&TestHandler::onTriggeredWithArgument, &handler1, std::placeholders::_1));

			TestHandler handler2;
			event.subscribe(std::bind(&TestHandler::onTriggeredWithArgument, &handler2, std::placeholders::_1));

			const TestArgument argument{ true };
			event(argument);

			ASSERT_TRUE(handler1._triggeredArgument._value);
			ASSERT_TRUE(handler2._triggeredArgument._value);
		}

		TEST_F(ThenHandlersInvoked, Given_LambdaSubscriptionUsingSubscribeOperator)
		{
			const Event<> event;
			auto triggered1{ false };
			event += ([&triggered1]() { triggered1 = true; });

			auto triggered2{ false };
			event += ([&triggered2]() { triggered2 = true; });

			event();

			ASSERT_TRUE(triggered1);
			ASSERT_TRUE(triggered2);
		}

		TEST_F(ThenHandlersInvoked, Given_LambdaWithArgumentSubscriptionUsingSubscribeOperator)
		{
			const Event<TestArgument> event;

			TestArgument triggered1;
			event += ([&triggered1](TestArgument argument) { triggered1 = argument; });

			TestArgument triggered2;
			event += ([&triggered2](TestArgument argument) { triggered2 = argument; });

			const TestArgument argument{ true };
			event(argument);

			ASSERT_TRUE(triggered1._value);
			ASSERT_TRUE(triggered2._value);
		}

		TEST_F(ThenHandlersInvoked, Given_ClassMemberSubscriptionUsingSubscribeOperator)
		{
			const Event<> event;
			TestHandler handler1;
			event += std::bind(&TestHandler::onTriggered, &handler1);

			TestHandler handler2;
			event += std::bind(&TestHandler::onTriggered, &handler2);

			event();

			ASSERT_TRUE(handler1._triggered);
			ASSERT_TRUE(handler2._triggered);
		}

		TEST_F(ThenHandlersInvoked, Given_ClassMemberWithArgumentsSubscriptionUsingSubscribeOperator)
		{
			const Event<TestArgument> event;
			TestHandler handler1;
			event += std::bind(&TestHandler::onTriggeredWithArgument, &handler1, std::placeholders::_1);

			TestHandler handler2;
			event += std::bind(&TestHandler::onTriggeredWithArgument, &handler2, std::placeholders::_1);

			const TestArgument argument{ true };
			event(argument);

			ASSERT_TRUE(handler1._triggeredArgument._value);
			ASSERT_TRUE(handler2._triggeredArgument._value);
		}

		class ThenHandlersNotInvoked : public testing::Test
		{
		};

		TEST_F(ThenHandlersNotInvoked, Given_LambdaUnsubscriptionUsingUnsubscribeMethod)
		{
			const Event<> event;
			auto triggered1{ false };
			const auto id = event.subscribe([&triggered1]() { triggered1 = true; });

			auto triggered2{ false };
			event.subscribe([&triggered2]() { triggered2 = true; });

			event.unsubscribe(id);

			event();

			ASSERT_FALSE(triggered1);
			ASSERT_TRUE(triggered2);
		}

		TEST_F(ThenHandlersNotInvoked, Given_LambdaWithArgumentUnsubscriptionUsingUnsubscribeMethod)
		{
			const Event<TestArgument> event;

			TestArgument triggered1;
			const auto id = event.subscribe([&triggered1](TestArgument argument) { triggered1 = argument; });

			TestArgument triggered2;
			event.subscribe([&triggered2](TestArgument argument) { triggered2 = argument; });

			event.unsubscribe(id);

			const TestArgument argument{ true };
			event(argument);

			ASSERT_FALSE(triggered1._value);
			ASSERT_TRUE(triggered2._value);
		}

		TEST_F(ThenHandlersNotInvoked, Given_ClassMemberUnsubscriptionUsingUnsubscribeMethod)
		{
			const Event<> event;
			TestHandler handler1;
			const auto id = event.subscribe(std::bind(&TestHandler::onTriggered, &handler1));

			TestHandler handler2;
			event.subscribe(std::bind(&TestHandler::onTriggered, &handler2));

			event.unsubscribe(id);

			event();

			ASSERT_FALSE(handler1._triggered);
			ASSERT_TRUE(handler2._triggered);
		}

		TEST_F(ThenHandlersNotInvoked, Given_ClassMemberWithArgumentsUnsubscriptionUsingUnsubscribeMethod)
		{
			const Event<TestArgument> event;
			TestHandler handler1;
			const auto id = event.subscribe(std::bind(&TestHandler::onTriggeredWithArgument, &handler1, std::placeholders::_1));

			TestHandler handler2;
			event.subscribe(std::bind(&TestHandler::onTriggeredWithArgument, &handler2, std::placeholders::_1));

			event.unsubscribe(id);

			const TestArgument argument{ true };
			event(argument);

			ASSERT_FALSE(handler1._triggeredArgument._value);
			ASSERT_TRUE(handler2._triggeredArgument._value);
		}

		TEST_F(ThenHandlersNotInvoked, Given_LambdaUnsubscriptionUsingUnsubscribeOperator)
		{
			const Event<> event;
			auto triggered1{ false };
			const auto id = event += ([&triggered1]() { triggered1 = true; });

			auto triggered2{ false };
			event += ([&triggered2]() { triggered2 = true; });

			event -= id;

			event();

			ASSERT_FALSE(triggered1);
			ASSERT_TRUE(triggered2);
		}

		TEST_F(ThenHandlersNotInvoked, Given_LambdaWithArgumentUnsubscriptionUsingUnsubscribeOperator)
		{
			const Event<TestArgument> event;

			TestArgument triggered1;
			const auto id = event += ([&triggered1](TestArgument argument) { triggered1 = argument; });

			TestArgument triggered2;
			event += ([&triggered2](TestArgument argument) { triggered2 = argument; });

			event -= id;

			const TestArgument argument{ true };
			event(argument);

			ASSERT_FALSE(triggered1._value);
			ASSERT_TRUE(triggered2._value);
		}

		TEST_F(ThenHandlersNotInvoked, Given_ClassMemberUnsubscriptionUsingUnsubscribeOperator)
		{
			const Event<> event;
			TestHandler handler1;
			const auto id = event += std::bind(&TestHandler::onTriggered, &handler1);

			TestHandler handler2;
			event += std::bind(&TestHandler::onTriggered, &handler2);

			event -= id;

			event();

			ASSERT_FALSE(handler1._triggered);
			ASSERT_TRUE(handler2._triggered);
		}

		TEST_F(ThenHandlersNotInvoked, Given_ClassMemberWithArgumentsUnsubscriptionUsingUnsubscribeOperator)
		{
			const Event<TestArgument> event;
			TestHandler handler1;
			const auto id = event += std::bind(&TestHandler::onTriggeredWithArgument, &handler1, std::placeholders::_1);

			TestHandler handler2;
			event += std::bind(&TestHandler::onTriggeredWithArgument, &handler2, std::placeholders::_1);

			event -= id;

			const TestArgument argument{ true };
			event(argument);

			ASSERT_FALSE(handler1._triggeredArgument._value);
			ASSERT_TRUE(handler2._triggeredArgument._value);
		}

		typedef std::pair<int, std::chrono::milliseconds> TestParams;
		class ThenHandlersAreInvokedInStipulatedTime : public testing::TestWithParam<TestParams>
		{

		};

		INSTANTIATE_TEST_SUITE_P(GivenNSubscriptions, ThenHandlersAreInvokedInStipulatedTime, ::testing::Values(
			TestParams{ 10, std::chrono::milliseconds(10) },
			TestParams{ 100, std::chrono::milliseconds(20) },
			TestParams{ 1000, std::chrono::milliseconds(30) },
			TestParams{ 10000, std::chrono::milliseconds(40) }
		));

		TEST_P(ThenHandlersAreInvokedInStipulatedTime, Given_LambdaSubscription)
		{
			const Event<> event;

			const auto params = GetParam();
			const auto numberOfSubscribers = params.first;
			const auto expectedDuration = params.second;

			std::vector<bool> triggered(numberOfSubscribers, false);

			for (auto i = 0; i < numberOfSubscribers; i++)
			{
				event.subscribe([&triggered, i]() { triggered[i] = true; });
			}

			const auto start = std::chrono::high_resolution_clock::now();
			event();
			const auto duration = std::chrono::high_resolution_clock::now() - start;


			ASSERT_TRUE(std::all_of(triggered.begin(), triggered.end(), [](const bool value) { return value == true; }));
			ASSERT_TRUE(duration < expectedDuration);
		}

		TEST_P(ThenHandlersAreInvokedInStipulatedTime, Given_MemberFunctionSubscription)
		{
			const Event<> event;

			const auto params = GetParam();
			const auto numberOfSubscribers = params.first;
			const auto expectedDuration = params.second;

			std::vector<TestHandler> handlers(numberOfSubscribers);

			for (auto i = 0; i < numberOfSubscribers; i++)
			{
				event.subscribe(std::bind(&TestHandler::onTriggered, &handlers[i]));
			}

			const auto start = std::chrono::high_resolution_clock::now();
			event();
			const auto duration = std::chrono::high_resolution_clock::now() - start;

			ASSERT_TRUE(std::all_of(handlers.begin(), handlers.end(), [](const TestHandler& handler) { return handler._triggered == true; }));
			ASSERT_TRUE(duration < expectedDuration);
		}

		enum OperatorTypes
		{
			CopyConstructor,
			MoveConstructor,
			AssignmentOperator,
			AssignmentMoveOperator,
		};

		class MultiOperatorTypeEventTest : public ThenHandlersNotInvoked,	public ::testing::WithParamInterface<OperatorTypes>
		{
		public:
			void createObject()
			{
				switch(GetParam())
				{
				case OperatorTypes::CopyConstructor:
				{
					eventTestnew = std::make_unique<Event<TestArgument>>(eventTest);
					break;
				}
				case OperatorTypes::MoveConstructor:
				{
					eventTestnew = std::make_unique<Event<TestArgument>>(std::move(eventTest));
					break;
				}
				case OperatorTypes::AssignmentOperator:
				{
					Event<TestArgument> neweventTest;
					neweventTest = eventTest;
					eventTestnew = std::make_unique<Event<TestArgument>>(neweventTest);
					break;
				}
				case OperatorTypes::AssignmentMoveOperator:
				{
					Event<TestArgument> newevent;
					newevent = std::move(eventTest);
					eventTestnew = std::make_unique<Event<TestArgument>>(std::move(newevent));
					break;
				}
				}
			}
			Event<TestArgument> eventTest;
			std::unique_ptr<Event<TestArgument>> eventTestnew;
		};

		INSTANTIATE_TEST_CASE_P(Given_ValidFormats, MultiOperatorTypeEventTest, ::testing::Values(
			OperatorTypes::CopyConstructor,
			OperatorTypes::MoveConstructor,
			OperatorTypes::AssignmentOperator,
			OperatorTypes::AssignmentMoveOperator));

		TEST_P(MultiOperatorTypeEventTest, Given_LambdaSubscription_Then_VerifySubscriptionEventTriggered)
		{
			TestArgument triggered;
			eventTest += ([&triggered](TestArgument argument) {
				triggered = argument;
			});
			
			createObject();

			const TestArgument argument{ true };
			Event<TestArgument> &eventnew = *eventTestnew.get();
			eventnew(argument);
			ASSERT_TRUE(triggered._value);
		}
		
		TEST_P(MultiOperatorTypeEventTest, Given_LambdaWithArgumentUnsubscription_Then_VerifyUnsubscriptionEventTriggred)
		{
			TestArgument triggered1;
			const auto id = eventTest.subscribe([&triggered1](TestArgument argument) { triggered1 = argument; });

			const TestArgument argumenttest{ false };
			eventTest(argumenttest);

			TestArgument triggered2;
			eventTest.subscribe([&triggered2](TestArgument argument) { triggered2 = argument; });

			createObject();

			eventTestnew.get()->unsubscribe(id);

			const TestArgument argument{ true };
			Event<TestArgument> &eventnew = *eventTestnew.get();
			eventnew(argument);

			ASSERT_FALSE(triggered1._value);
			ASSERT_TRUE(triggered2._value);
		}
	}
}

