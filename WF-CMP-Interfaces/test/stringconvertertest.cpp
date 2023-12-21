// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>
#include "workflow.h"

using namespace Philips::Workflow;

TEST(StringConverterTest, Given_AString_When_ConvertedToWstring_Then_ValueIsAsExpected)
{
	const auto string = "string";
	ASSERT_EQ(convert(string), L"string");
}

TEST(StringConverterTest, Given_Wstring_When_ConvertedToString_Then_ValueIsAsExpected)
{
	const auto string = L"string";
	ASSERT_EQ(convert(string), "string");
}