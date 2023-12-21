// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>

#include <workflow.h>

namespace DateTimeTest {

	using namespace Philips::Workflow;
	using namespace std::chrono;

	typedef std::pair<std::string, std::string> StringParams;

	class DateTimeConstructorTest : public ::testing::Test
	{
	protected:
		//Datetime set to 02/02/2020 14:02:02 GMT
		std::tm m_time =
		{	/* .tm_sec  = */ 2,
			/* .tm_min  = */ 2,
			/* .tm_hour = */ 14,
			/* .tm_mday = */ (2),
			/* .tm_mon  = */ (2) - 1,
			/* .tm_year = */ (2020) - 1900,
		};
	};

	class DateTimeTest : public ::testing::Test
	{
	protected:
		std::unique_ptr<DateTime> m_dateTime;
		void SetUp() override
		{
			std::tm time =
			{	/* .tm_sec  = */ 2,
				/* .tm_min  = */ 2,
				/* .tm_hour = */ 14,
				/* .tm_mday = */ (2),
				/* .tm_mon  = */ (2) - 1,
				/* .tm_year = */ (2020) - 1900,
			};
			//Datetime set to 02/02/2020 14:02:02 GMT
			m_dateTime.reset(new DateTime(system_clock::from_time_t(_mkgmtime(&time))));
		}
	};

	class InvalidDateTimeTest : public ::testing::Test
	{
	protected:
		std::unique_ptr<DateTime> m_dateTime;
		void SetUp() override
		{
			m_dateTime.reset(new DateTime());
		}
	};


	//Constructor Tests

	TEST_F(DateTimeConstructorTest, When_DefaultConstructorCalled_Then_DateTimeObjectWith0TimeStampCreated)
	{
		const DateTime dateTime;
		ASSERT_LE(dateTime.time_since_epoch().count(), 0);
	}	

	TEST_F(DateTimeConstructorTest, When_ParameterizedConstructorCalledWithGivenTimePoint_Then_DateTimeObjectCreatedWithOriginalTimePoint)
	{
		const auto timePoint = system_clock::from_time_t(_mkgmtime(&m_time));
		const DateTime dateTime(timePoint);
		ASSERT_EQ(timePoint.time_since_epoch().count(), dateTime.time_since_epoch().count());
	}

	TEST_F(DateTimeConstructorTest, When_CopyConstructorCalled_Then_DateTimeObjectCreatedEqualToOriginalDateTimeObject)
	{
		DateTime dateTime(system_clock::from_time_t(_mkgmtime(&m_time)));
		ASSERT_EQ(DateTime((dateTime)), dateTime);
	}
	
	TEST_F(DateTimeConstructorTest, When_MoveConstructorCalledWithGivenDateTimeObject_Then_DateTimeObjectCreatedEqualToOriginalDateTimeObject)
	{
		DateTime dateTime(system_clock::from_time_t(_mkgmtime(&m_time)));
		const auto dateTime1(dateTime);
		auto dateTime2(std::move(dateTime));
		ASSERT_EQ(dateTime2, dateTime1);
	}

	//Assignment Operator Tests

	TEST_F(DateTimeTest, Given_InitializedDateTime_When_ReAssignedUsingAssignmentOperator_ThenDateTimeIsCorrectlyAssigned)
	{
		auto dateTime = DateTime();
		auto now = DateTime::now();
		dateTime = DateTime(now);

		ASSERT_EQ(now, dateTime);
	}

	//ToString Function Tests
	
	class ValidToStringDateTimeTest :	public DateTimeTest,
										public ::testing::WithParamInterface<StringParams>
	{
	};

	INSTANTIATE_TEST_CASE_P(Given_NValidFormats, ValidToStringDateTimeTest, ::testing::Values(
		StringParams{ "yyyyMMdd HHmmss", "20200202 140202" },
		StringParams{ "yyyy-MM-dd hh:mm:ss tt" , "2020-02-02 02:02:02 PM" },
		StringParams{ "dd/MM/yyyy HH mm ss" , "02/02/2020 14 02 02" },
		StringParams{ "HH mm ss" , "14 02 02" },
		StringParams{ "hh:mm:ss tt" , "02:02:02 PM" },
		StringParams{ "dd" , "02" },
		StringParams{ "MM" , "02" },
		StringParams{ "yyyy" , "2020" },
		StringParams{ "HH" , "14" },
		StringParams{ "mm" , "02" },
		StringParams{ "ss" , "02" },
		StringParams{ "tt" , "PM" }));

	TEST_P(ValidToStringDateTimeTest, When_ToStringCalled_Then_ExpectedDateTimeStringReturned)
	{
		const auto format = GetParam().first;
		const auto result = GetParam().second;
		ASSERT_EQ(m_dateTime->toString(format, false), result);
	}

	TEST_F(DateTimeTest, Given_ValidTimeFormatAndLocal_When_ToStringCalled_Then_OnFromStringCalledOriginalDateTimeObjectReturned)
	{
		auto dateTime = m_dateTime->toString("yyyyMMdd HHmmss", true);
		ASSERT_EQ(DateTime::fromString("yyyyMMdd HHmmss", dateTime, true), *m_dateTime);
	}	

	class InvalidToStringDateTimeTest :	public DateTimeTest,
										public ::testing::WithParamInterface<StringParams>
	{
	};

	INSTANTIATE_TEST_SUITE_P(Given_NInvalidFormats, InvalidToStringDateTimeTest, ::testing::Values(
		StringParams{ "", "" },
		StringParams{ "abcx1234!@@$#%^$", "abcx1234!@@$#%^$" },
		StringParams{ "yyyyMMdd HHmmss tt", "20200202 140202 tt" }));

	TEST_P(InvalidToStringDateTimeTest, Given_ValidTime_When_ToStringCalled_Then_InvalidDateTimeStringReturned)
	{
		const auto format = GetParam().first;
		const auto result = GetParam().second;
		
		ASSERT_EQ(m_dateTime->toString(format, false), result);
	}

	TEST_F(InvalidDateTimeTest, Given_ValidTimeFormatAndInvalidTime_When_ToStringCalled_Then_EmptyDateTimeStringReturned)
	{
		ASSERT_TRUE(m_dateTime->toString("yyyyMMdd HHmmss", false).empty());
	}

	//FromString Function Tests

	class ValidFromStringDateTimeTest : public DateTimeTest,
										public ::testing::WithParamInterface<StringParams>
	{
	};

	INSTANTIATE_TEST_SUITE_P(Given_NValidFormatsAndTimeString, ValidFromStringDateTimeTest, ::testing::Values(
		StringParams{ "yyyyMMdd HHmmss", "20200202 140202" },
		StringParams{ "yyyyMMdd hhmmss tt", "20200202 020202 PM" },
		StringParams{ "yyyy-MM-dd HH:mm:ss", "2020-02-02 14:02:02" },
		StringParams{ "dd/MM/yyyy hh mm ss tt", "02/02/2020 02 02 02 PM" }));

	TEST_P(ValidFromStringDateTimeTest, Given_UTC_When_FromStringCalled_Then_ExpectedDateTimeReturned)
	{
		const auto format = GetParam().first;
		const auto dateTime = GetParam().second;

		ASSERT_EQ(DateTime::fromString(format, dateTime, false), *m_dateTime);
	}

	TEST_F(DateTimeTest, Given_Date2ndFeb2020WithoutTime_When_FromStringCalled_Then_DateTimeReturnedIsDate2ndFeb2020Time00_00_00)
	{
		auto dateTime1 = DateTime::fromString("dd/MM/yyyy", "02/02/2020", false);
		const auto expected = DateTime::fromString("dd/MM/yyyy HH:mm:ss", "02/02/2020 00:00:00", false);

		ASSERT_EQ(dateTime1, expected);
	}

	TEST_F(DateTimeTest, Given_ValidTimeFormatAndTimeStringAndLocal_When_FromStringCalled_Then_OnToStringCalledOriginalTimeStringReturned)
	{
		auto format = "dd/MM/yyyy HH mm ss";
		const auto dateTimeValue = "02/02/2020 02 02 02";
		
		DateTime dateTime = DateTime::fromString(format, dateTimeValue, true);
		ASSERT_EQ(dateTime.toString(format, true), dateTimeValue);
	}

	class InvalidFromStringDateTimeTest : public InvalidDateTimeTest,
										  public ::testing::WithParamInterface<StringParams>
	{
	};

	INSTANTIATE_TEST_SUITE_P(Given_NInvalidFormatsAndValidTimeString, InvalidFromStringDateTimeTest, ::testing::Values(
		StringParams{ "yyMd Hms", "20200202 140202" },
		StringParams{ "", "20200202 140202" },
		StringParams{ "yyyyMMdd HHmmss tt", "20200202 140202" },
		StringParams{ "abcd1234!@@$#%^$", "20200202 140202" }));

	INSTANTIATE_TEST_SUITE_P(Given_NValidFormatsAndInvalidTimeString, InvalidFromStringDateTimeTest, ::testing::Values(
		StringParams{ "yyyyMMdd HHmmss", "00000000 000000" },
		StringParams{ "yyyyMMdd HHmmss", "" },
		StringParams{ "yyyyMMdd HHmmss", "abcdaj asdksahf" },
		StringParams{ "yyyyMMdd HHmmss", "!$#!$#@$%sadsad" }));

	TEST_P(InvalidFromStringDateTimeTest, When_FromStringCalled_Then_EmptyDateTimeReturned)
	{
		const auto format = GetParam().first;
		const auto dateTime = GetParam().second;
		
		auto fromStringDateTime = DateTime::fromString(format, dateTime, false);
		ASSERT_EQ(fromStringDateTime, *m_dateTime);
	}

	//DateTime Now Function Tests

	TEST_F(DateTimeTest, When_DateTimeNowCalledInBetweenSystemClockNow_Then_DateTimeNowValueIsBetweenSystemClockNowValues)
	{
		const auto time1 = DateTime(system_clock::now());
		const auto time2 = DateTime::now();
		const auto time3 = DateTime(system_clock::now());

		ASSERT_GE(time2, time1);
		ASSERT_GE(time3, time2);
	}

	//ToDate Function Tests

	TEST_F(DateTimeTest, Given_2ndFeb2020DateTime_When_ToDateCalled_Then_Day2Month2Year2020Returned)
	{
		auto date = m_dateTime->toDate(false);
		ASSERT_EQ(date.day, 2);
		ASSERT_EQ(date.month, 2);
		ASSERT_EQ(date.year, 2020);

		const auto year = m_dateTime->toString("yyyy", true);
		const auto month = m_dateTime->toString("MM", true);
		const auto day = m_dateTime->toString("dd", true);

		date = m_dateTime->toDate(true);
		ASSERT_EQ(date.day, stoi(day));
		ASSERT_EQ(date.month, stoi(month));
		ASSERT_EQ(date.year, stoi(year));
	}

	TEST_F(InvalidDateTimeTest, Given_0DateTime_When_ToDateCalled_Then_Day0Month0Year0Returned)
	{
		auto date = m_dateTime->toDate();
		ASSERT_EQ(date.day, 0);
		ASSERT_EQ(date.month, 0);
		ASSERT_EQ(date.year, 0);
	}

	//Operators
	TEST_F(DateTimeTest, Given_2ndFeb2020DateTime_When_ComparedTo3rdFeb2020_Then_LessThanOperatorReturnsTrue)
	{
		const auto feb2_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "02/02/2020 02 02 02", true);
		const auto feb3_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "03/02/2020 02 02 02", true);

		ASSERT_TRUE(feb2_2020 < feb3_2020);
	}

	TEST_F(DateTimeTest, Given_2ndFeb2020DateTime_When_ComparedTo3rdFeb2020_Then_GreaterThanOperatorReturnsFalse)
	{
		const auto feb2_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "02/02/2020 02 02 02", true);
		const auto feb3_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "03/02/2020 02 02 02", true);

		ASSERT_FALSE(feb2_2020 > feb3_2020);
	}

	TEST_F(DateTimeTest, Given_2ndFeb2020DateTime_When_ComparedTo3rdFeb2020_Then_LessThanOrEqualToOperatorReturnsTrue)
	{
		const auto feb2_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "02/02/2020 02 02 02", true);
		const auto feb3_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "03/02/2020 02 02 02", true);

		ASSERT_TRUE(feb2_2020 <= feb3_2020);
	}

	TEST_F(DateTimeTest, Given_2ndFeb2020DateTime_When_ComparedTo3rdFeb2020_Then_GreaterThanOrEqualToOperatorReturnsFalse)
	{
		const auto feb2_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "02/02/2020 02 02 02", true);
		const auto feb3_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "03/02/2020 02 02 02", true);

		ASSERT_FALSE(feb2_2020 >= feb3_2020);
	}

	TEST_F(DateTimeTest, Given_3rdFeb2020_When_ComparedTo2ndFeb2020DateTime_Then_GreaterThanOperatorReturnsTrue)
	{
		const auto feb2_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "02/02/2020 02 02 02", true);
		const auto feb3_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "03/02/2020 02 02 02", true);

		ASSERT_TRUE(feb3_2020 > feb2_2020);
	}

	TEST_F(DateTimeTest, Given_3rdFeb2020_When_ComparedTo2ndFeb2020DateTime_Then_LessThanOperatorReturnsTrue)
	{
		const auto feb2_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "02/02/2020 02 02 02", true);
		const auto feb3_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "03/02/2020 02 02 02", true);

		ASSERT_FALSE(feb3_2020 < feb2_2020);
	}

	TEST_F(DateTimeTest, Given_3rdFeb2020_When_ComparedTo2ndFeb2020DateTime_Then_GreaterThanOrEqualToOperatorReturnsTrue)
	{
		const auto feb2_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "02/02/2020 02 02 02", true);
		const auto feb3_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "03/02/2020 02 02 02", true);

		ASSERT_TRUE(feb3_2020 >= feb2_2020);
	}

	TEST_F(DateTimeTest, Given_3rdFeb2020_When_ComparedTo2ndFeb2020DateTime_Then_LessThanOrEqualToOperatorReturnsFalse)
	{
		const auto feb2_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "02/02/2020 02 02 02", true);
		const auto feb3_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "03/02/2020 02 02 02", true);

		ASSERT_FALSE(feb3_2020 <= feb2_2020);
	}

	TEST_F(DateTimeTest, Given_2ndFeb2020DateTime_When_ComparedToItself_Then_LessThanOrEqualToOperatorReturnsTrue)
	{
		const auto feb2_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "02/02/2020 02 02 02", true);

		ASSERT_TRUE(feb2_2020 <= feb2_2020);
	}

	TEST_F(DateTimeTest, Given_2ndFeb2020DateTime_When_ComparedTo3rdFeb2020_Then_GreaterThanOrEqualToOperatorReturnsTrue)
	{
		const auto feb2_2020 = DateTime::fromString("dd/MM/yyyy HH mm ss", "02/02/2020 02 02 02", true);

		ASSERT_TRUE(feb2_2020 >= feb2_2020);
	}
	
}