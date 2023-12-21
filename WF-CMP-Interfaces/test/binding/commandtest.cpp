// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <Workflow/binding.h>
#include <gtest/gtest.h>

namespace Test{ namespace Binding {

	using namespace Philips::Workflow::Binding;
	
	class CommandTest: public ::testing::Test
	{
	};

	TEST_F(CommandTest, When_ExecuteOperatorCalled_Then_ExecuteMethodInvoked)
	{
		Command command;
		auto executed{ false };
		command.execute = [&executed] { executed = true; };

		command();
		
		ASSERT_TRUE(executed);
	}

	TEST_F(CommandTest, Given_CanExecuteReturnsFalse_When_ExecuteOperatorCalled_Then_ExecuteMethodNotInvoked)
	{
		Command command;
		auto executed{ false };
		command.execute = [&executed] { executed = true; };
		command.canExecute =  false; 		

		command();

		ASSERT_FALSE(executed);
	}

	TEST_F(CommandTest, Given_ExecuteIsNullptr_When_ExecuteOperatorCalled_Then_NoExceptionThrown)
	{
		Command command;
		command.execute = nullptr;

		ASSERT_NO_THROW(command()); //TICS !ERR#017: Catch is part of Google test.
	}
	
}}
