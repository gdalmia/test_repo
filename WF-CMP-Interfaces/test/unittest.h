// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once 
#include <gtest/gtest.h>

namespace Philips { namespace Workflow { namespace Test {

	//! Base class for all Unit Test Suites in Workflow
	class UnitTest
	{
	public:
		UnitTest()
		{
			testing::Test::RecordProperty("Test Level", "Unit");
		}

		static void RequirementTag(const std::string& requirement)
		{
			testing::Test::RecordProperty("Requirement Tag", requirement);
		}

		static void TestSpecificationTag(const std::string& tag)
		{
			testing::Test::RecordProperty("Test Specification Tag", tag);
		}

		static void Objective(const std::string& objective)
		{
			testing::Test::RecordProperty("Objective", objective);
		}

		static void Preconditions(const std::vector<std::string>& conditions)
		{
			auto index { 1 };
			for (const auto& condition : conditions)
			{
				auto key = "Precondition" + std::to_string(index++);
				testing::Test::RecordProperty(key, condition);
			}
		}

		static void Action(const std::string& action)
		{
			testing::Test::RecordProperty("Action", action);
		}

		static void ExpectedResult(const std::string& expectedResult)
		{
			testing::Test::RecordProperty("Expected Result", expectedResult);
		}

		static std::string ActualResult(const std::string& actualResult)
		{
			testing::Test::RecordProperty("Actual Result", actualResult);
			return actualResult;
		}
	};

}}}