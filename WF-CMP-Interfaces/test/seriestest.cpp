// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>

#include <workflow.h>

using namespace Philips::Workflow;

//! Tests the behavior of Series class
class SeriesTest : public testing::Test
{
};

TEST_F(SeriesTest, Given_TwoSeries_When_InstaneUIDsAreSame_Then_VerifySeriesAreEqual)
{
    Series series1;
    series1.instanceUid = "uid";

    Series series2;
    series2.instanceUid = "uid";

    ASSERT_EQ(series1, series2) << "Series are not equal";
}

TEST_F(SeriesTest, Given_TwoSeries_When_InstaneUIDsAreDifferent_Then_VerifySeriesAreNotEqual)
{
    Series series1;
    series1.instanceUid = "uid1";

    Series series2;
    series2.instanceUid = "uid2";

    ASSERT_NE(series1, series2) << "Series are equal";
}

TEST_F(SeriesTest, Given_TwoSeries_When_InstaneUIDsAreDifferentButOneIsEmpty_Then_VerifySeriesAreNotEqual)
{
	Series series1;
	series1.instanceUid = "uid1";

	Series series2;
	series2.instanceUid = {};

	ASSERT_NE(series1, series2) << "Series are equal";
}

TEST_F(SeriesTest, Given_TwoSeriesWithDifferentKeys_When_InstaneUIDsAreSame_Then_VerifySeriesAreEqual)
{
    Series series1("key1");
    series1.instanceUid = "uid";

    Series series2("key2");
    series2.instanceUid = "uid";

    ASSERT_EQ(series1, series2) << "Series are not equal";
}

TEST_F(SeriesTest, Given_TwoSeriesWithDifferentKeys_When_InstaneUIDsAreDifferentAndOneIsEmpty_Then_VerifySeriesAreNotEqual)
{
	Series series1("key1");
	series1.instanceUid = "uid1";

	Series series2("key2");
	series2.instanceUid = {};

	ASSERT_NE(series1, series2) << "Series are equal";
}

TEST_F(SeriesTest, Given_TwoSeriesWithSameParent_When_InstanceUiIdsAreDifferent_Then_VerifySeriesAreNotEqual)
{
    Procedure procedure1("key1");
    procedure1.id = "id1";

    Series series1(procedure1);
    series1.instanceUid = "uid1";
    Series series2(procedure1);
    series2.instanceUid = "uid2";

    ASSERT_NE(series1, series2) << "Series are equal";
}