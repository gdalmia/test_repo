// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>
#include <workflow.h>

namespace Philips { namespace Workflow { namespace Test {

	using namespace Philips::Workflow;
	using namespace std::chrono;

	class OptionalTest : public ::testing::Test
	{
	protected:
		std::unique_ptr<DateTime> m_dateTime;
	};

	TEST_F(OptionalTest, Given_Optional_When_AccessedUninitializedValue_Then_ExceptionIsThrown)
	{
		Optional<DateTime> optional;
		try
		{
			optional.value();
		}
		catch (std::string message)
		{
			ASSERT_FALSE(optional);
			ASSERT_FALSE(optional.has_value());
			ASSERT_EQ(message, "Invalid value");
		}
	}	

	TEST_F(OptionalTest, Given_Optional_When_AccessedUninitializedValueOr_Then_DefaultValueIsReturned)
	{
		Optional<DateTime> optional;
		DateTime dateTime;
		ASSERT_EQ(dateTime, optional.value_or(dateTime));
	}

	TEST_F(OptionalTest, Given_Optional_When_ValueSet_Then_ValueIsValid)
	{
		Optional<DateTime> optional;
		optional = DateTime();
		ASSERT_TRUE(optional);
	}

	TEST_F(OptionalTest, Given_Optional_When_Reset_Then_ValueIsInvalid)
	{
		Optional<DateTime> optional;
		optional = DateTime();
		optional.reset();
		ASSERT_FALSE(optional);
	}

	TEST_F(OptionalTest, Given_Optional_When_CreatedWithValue_Then_ValueIsValid)
	{
		DateTime dateTime;
		Optional<DateTime> optional(dateTime);
		ASSERT_TRUE(optional);
	}

	TEST_F(OptionalTest, Given_Optional_When_UpdatedWithOtherOptional_Then_ValueIsValid)
	{
		DateTime dateTime;
		Optional<DateTime> optional(dateTime);
		Optional<DateTime> optionalCreated(optional);
		Optional<DateTime> optionalAssigned;
		optionalAssigned = optional;
		ASSERT_TRUE(optionalCreated);
		ASSERT_TRUE(optionalAssigned);
		ASSERT_EQ(optionalCreated.value(), dateTime);
	}
	
	TEST_F(OptionalTest, Given_OptionalAttributes_When_Compared_Then_ResultIsReturned)
	{
		Optional<DateTime> optional1(DateTime::fromString("dd/MM/yyyy HH mm ss", "02/02/2020 02 02 02", true));
		Optional<DateTime> optional2(DateTime::fromString("dd/MM/yyyy HH mm ss", "03/02/2020 02 02 02", true));
		ASSERT_TRUE(optional1 < optional2);
		ASSERT_FALSE(optional1 >= optional2);
		ASSERT_FALSE(optional1 > optional2);
		ASSERT_TRUE(optional1 <= optional2);
	}
}}}