// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "workflow.h"
#include "testmodelcreator.h"
#include "testmodelverifier.h"
#include <gtest\gtest.h>

using namespace Philips::Workflow;
using namespace Philips::Workflow::Test;
namespace Test
{
	//TICS -INT#027 -INT#008 -OAL#011 -OLC#009 -INT#002
	// Suppressed as warning is thrown for all Test cases related to override specifier, "new" inside smart pointers 
	// and members are used in TestCases, so made protected
	class SeriesNotificationTest : public testing::Test
	{
	public:

		SeriesNotificationTest() :_settings(std::experimental::filesystem::temp_directory_path(), false)
		{

		}

		~SeriesNotificationTest() = default;

		void SetUp() override
		{
		}


		bool subscribeNotificationAndUpdateSeries(Series& seriesToBeSubscribed, Series& seriesToBeUpdated)
		{
			bool flag = false;
			SscfTs1::EventLoop waitEventLoop;
			std::unique_ptr<ISeriesNotification> notification = studyrepo.subscribe(seriesToBeSubscribed);
			notification->seriesChanged.subscribe([&](const Series&)
			{
				flag = true;
				waitEventLoop.stop();
			});
			seriesToBeUpdated.description = "changed";
			studyrepo.update(seriesToBeUpdated);
			waitEventLoop.run(2000);// need to wait for series to be changed in database
			return flag;
		}

	protected:
		Settings                                    _settings;
		StudyRepository                             studyrepo;
	};

	TEST_F(SeriesNotificationTest, Given_Series_When_Changed_Then_SeriesChangedEventIsRaised)
	{
		SscfTs1::EventLoop waitEventLoop;
		auto procedure = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		studyrepo.add(study);
		procedure.parent = study;
		studyrepo.add(procedure);
		
		auto series = newSeries();
		series.parent = procedure;
		studyrepo.add(series);
		waitEventLoop.run(2000);// need to wait for series to be created in database
		ASSERT_TRUE(subscribeNotificationAndUpdateSeries(series, series));
		
	}
	
	TEST_F(SeriesNotificationTest, Given_Series_When_Changed_Then_SeriesChangedEventIsRaisedAndChangedSeriesIsSameAsUpdatedSeries)
	{
		auto procedure = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		studyrepo.add(study);
		procedure.parent = study;
		studyrepo.add(procedure);

		auto series = newSeries();
		series.parent = procedure;
		studyrepo.add(series);

		SscfTs1::EventLoop waitEventLoop;
		Series changedSeries;
		waitEventLoop.run(2000);// need to wait for series to be created in database
		const std::unique_ptr<ISeriesNotification> notification = studyrepo.subscribe(series);
		notification->seriesChanged.subscribe([&](const Series& series_)
		{
			changedSeries = series_;
			waitEventLoop.stop();
		});
		series.description = "changed";
		studyrepo.update(series);
		waitEventLoop.run(2000);// need to wait for series to be changed in database
		ASSERT_TRUE(verifySeriesInfo(changedSeries, series));
	}

	TEST_F(SeriesNotificationTest, Given_SeriesOneAndSeriesTwoOfSameprocedure_When_SeriesOneIsSelectedForNotificationAndSeriesTwoIsChanged_Then_SeriesChangedEventIsNotRaisedForSeriesOne)
	{
		auto procedure = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		studyrepo.add(study);
		procedure.parent = study;
		studyrepo.add(procedure);
		SscfTs1::EventLoop waitEventLoop;

		auto series1 = newSeries();
		auto series2 = Series("Key");
		series1.parent = procedure;
		series2.parent = procedure;
		studyrepo.add(series1);
		studyrepo.add(series2);
		waitEventLoop.run(2000);// need to wait for series to be created in database
		ASSERT_FALSE(subscribeNotificationAndUpdateSeries(series1, series2));
	}
	
	TEST_F(SeriesNotificationTest, Given_TwoDifferentSeriesOfDifferentProcedure_When_SeriesOneOfProcedureOneIsSelectedForNotificationAndSeriesTwoOfProcedureTwoChanged_Then_SeriesChangedEventIsNotRaisedForSeriesOne)
	{
		SscfTs1::EventLoop waitEventLoop;
		auto procedureOne = Procedure("procedureone");
		auto procedureTwo = Procedure("porceduretwo");
		auto study = Study("studyone");
		auto study1 = Study("studytwo");
		studyrepo.add(study);
		studyrepo.add(study1);
		procedureOne.parent = study;
		studyrepo.add(procedureOne);
		procedureTwo.parent = study1;
		studyrepo.add(procedureTwo);

		auto series1 = Series("Key1");
		auto series2 = Series("Key2");
		series1.parent = procedureOne;
		series2.parent = procedureTwo;
		studyrepo.add(series1);
		studyrepo.add(series2);
		waitEventLoop.run(2000); // need to wait for series to be created in database
		ASSERT_FALSE(subscribeNotificationAndUpdateSeries(series1, series2));
	}
}