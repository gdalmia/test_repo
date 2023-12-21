// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "workflow.h"
#include "testmodelcreator.h"
#include "testmodelverifier.h"
#include <gtest\gtest.h>

using namespace Philips::Workflow;
using namespace Philips::Workflow::Test;
namespace Test
{
	//TICS -INT#027 -OAL#011 -OLC#009 -INT#002
	// Suppressed as warning is thrown for all Testcases related to override specifier, "new" inside smart pointers 
	// and members are used in TestCases, so made protected
	class ProcedureNotificationTest : public testing::Test
	{
	public:
		
		ProcedureNotificationTest():_settings(std::experimental::filesystem::temp_directory_path(), false)
		{

		}

		~ProcedureNotificationTest() = default;

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		bool createSeriesAndSubscribeProcedureForNotification(Procedure procedureToBeNotified, Procedure procedureToWhichSeriesAdded)
		{
			bool flag = false;
			SscfTs1::EventLoop waitEventLoop;
			const std::unique_ptr<IProcedureNotification> notification = studyrepo.subscribe(procedureToBeNotified);
			notification->seriesAdded.subscribe([&](const Series&)
			{
				flag = true;
				waitEventLoop.stop();
			});
			auto series = newSeries();
			series.parent = procedureToWhichSeriesAdded;
			studyrepo.add(series);
			waitEventLoop.run(2000);
			return flag;
		}

		bool subscribeNotificationAndUpdateProcedure(Procedure& procedureToBeNotified, Procedure& procedureToBeChanged)
		{
			bool flag = false;
			SscfTs1::EventLoop waitEventLoop;
			std::unique_ptr<IProcedureNotification> notification = studyrepo.subscribe(procedureToBeNotified);
			notification->procedureChanged.subscribe([&](const Procedure&)
			{
				flag = true;
				waitEventLoop.stop();
			});
			procedureToBeChanged.description = "changed";
			studyrepo.update(procedureToBeChanged);
			waitEventLoop.run(2000);
			return flag;
		}

	protected:
		Settings                                    _settings;
		StudyRepository                             studyrepo;
	};

	TEST_F(ProcedureNotificationTest, Given_Procedure_When_SeriesAdded_Then_SeriesAddedEventIsRaised)
	{
		bool flag = false;
		auto procedure = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure.parent = study;
		ASSERT_TRUE(studyrepo.add(procedure));
		SscfTs1::EventLoop waitEventLoop;
		const std::unique_ptr<IProcedureNotification> notification = studyrepo.subscribe(procedure);
		notification->seriesAdded.subscribe([&](const Series&)
		{
			flag = true;
			waitEventLoop.stop();
		});
		auto series = newSeries();
		series.parent = procedure;
		ASSERT_TRUE(studyrepo.add(series));
		ASSERT_EQ(waitEventLoop.run(2000), 0) << "Timeout. Failed to receive the Event.";      // Wait for the event.
		ASSERT_TRUE(flag);
	}

	TEST_F(ProcedureNotificationTest, Given_Procedure_When_Changed_Then_ProcedureChangedEventIsRaised)
	{
		SscfTs1::EventLoop waitEventLoop;
		auto procedure = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		studyrepo.add(study);
		procedure.parent = study;
		studyrepo.add(procedure);
		waitEventLoop.run(2000);// need to wait for procedure to be created in database
		
		ASSERT_TRUE(subscribeNotificationAndUpdateProcedure(procedure, procedure));
	}

	TEST_F(ProcedureNotificationTest, Given_Procedure_When_Changed_Then_ProcedureChangedObjectAndUpdatedProcedureObjectAreSame)
	{
		SscfTs1::EventLoop waitEventLoop;
		bool flag = false;
		auto procedure = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		studyrepo.add(study);
		procedure.parent = study;
		studyrepo.add(procedure);
		waitEventLoop.run(2000);// need to wait for series to be created in database
		Procedure changedProcedure;
		const std::unique_ptr<IProcedureNotification> notification = studyrepo.subscribe(procedure);
		notification->procedureChanged.subscribe([&](const Procedure& procedure_)
		{
			flag = true;
			changedProcedure = procedure_;
			waitEventLoop.stop();
		});
		procedure.state = Procedure::State::InProgress;
		studyrepo.update(procedure);
		waitEventLoop.run(2000);
		ASSERT_TRUE(verifyProcedureInfo(changedProcedure, procedure));
		ASSERT_TRUE(flag);
	}

	TEST_F(ProcedureNotificationTest, Given_TwoProceduresOfAStudy_When_NotifiedProcedureDoesNotChangeThen_ProcedureChangedEventIsNotRaised)
	{
		SscfTs1::EventLoop waitEventLoop;
		auto procedureOne = Procedure("procedure1");
		auto procedureTwo = Procedure("procedure2");
		auto study = Study("study");
		studyrepo.add(study);
		procedureOne.parent = study;
		procedureTwo.parent = study;
		studyrepo.add(procedureOne);
		studyrepo.add(procedureTwo);
		waitEventLoop.run(2000);// need to wait for procedure to be created in database

		ASSERT_FALSE(subscribeNotificationAndUpdateProcedure(procedureOne, procedureTwo));
	}

	TEST_F(ProcedureNotificationTest, Given_TwoProceduresOfDifferentStudies_When_NotifiedProcedureDoesNotChangeThen_ProcedureChangedEventIsNotRaised)
	{
		SscfTs1::EventLoop waitEventLoop;
		auto procedureOne = newProcedure(Procedure::Type::Coronary);
		auto procedureTwo = newProcedure(Procedure::Type::Coronary);
		auto studyOne = newStudy();
		auto studyTwo = newStudy();
		studyrepo.add(studyOne);
		studyrepo.add(studyTwo);
		procedureOne.parent = studyOne;
		procedureTwo.parent = studyTwo;
		studyrepo.add(procedureOne);
		studyrepo.add(procedureTwo);
		waitEventLoop.run(2000);// need to wait for procedure to be created in database

		ASSERT_FALSE(subscribeNotificationAndUpdateProcedure(procedureOne, procedureTwo));
	}
	

	TEST_F(ProcedureNotificationTest, Given_Procedure_When_SeriesRemoved_Then_SeriesRemovedEventIsRaised)
	{
		bool flag = false;
		auto procedure = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure.parent = study;
		ASSERT_TRUE(studyrepo.add(procedure));
		SscfTs1::EventLoop waitEventLoopForRemove;
		const std::unique_ptr<IProcedureNotification> notification = studyrepo.subscribe(procedure);

		notification->seriesRemoved.subscribe([&](IRepository::Model)
		{
			flag = true;
			waitEventLoopForRemove.stop();
		});
		auto series = newSeries();
		series.parent = procedure;
		ASSERT_TRUE(studyrepo.add(series));

		studyrepo.remove(series);
		ASSERT_EQ(waitEventLoopForRemove.run(2000), 0) << "Timeout. Failed to receive remove Event.";      // Wait for the event.
		ASSERT_TRUE(flag);
	}

	TEST_F(ProcedureNotificationTest, Given_Procedure_When_SeriesAdded_Then_VerifyExpectedSeriesAndActualSeriesAreSame)
	{
		auto procedure = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure.parent = study;
		ASSERT_TRUE(studyrepo.add(procedure));
		SscfTs1::EventLoop waitEventLoop;
		const std::unique_ptr<IProcedureNotification> notification = studyrepo.subscribe(procedure);
		Series expectedSeries;
		notification->seriesAdded.subscribe([&](const Series& output)
		{
			expectedSeries = output;
			waitEventLoop.stop();
		});
		auto actualSeries = newSeries();
		actualSeries.parent = procedure;
		ASSERT_TRUE(studyrepo.add(actualSeries));
		ASSERT_EQ(waitEventLoop.run(2000), 0) << "Timeout. Failed to receive the Event.";      // Wait for the event.
		ASSERT_EQ(verifySeriesInfo(actualSeries, expectedSeries), true);
	}

	TEST_F(ProcedureNotificationTest, Given_Procedure_When_SeriesRemoved_Then_VerifyExpectedModelAndActualModelAreSame)
	{
		auto procedure = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure.parent = study;
		ASSERT_TRUE(studyrepo.add(procedure));
		SscfTs1::EventLoop waitEventLoopForRemove;
		const std::unique_ptr<IProcedureNotification> notification = studyrepo.subscribe(procedure);
		IRepository::Model expectedModel;
		notification->seriesRemoved.subscribe([&](IRepository::Model model)
		{
			expectedModel = model;
			waitEventLoopForRemove.stop();
		});
		auto actualSeries = newSeries();
		actualSeries.parent = procedure;
		ASSERT_TRUE(studyrepo.add(actualSeries));

		studyrepo.remove(actualSeries);
		ASSERT_EQ(waitEventLoopForRemove.run(2000), 0) << "Timeout. Failed to receive remove Event.";      // Wait for the event.
		ASSERT_EQ(expectedModel, IRepository::Model(actualSeries));
	}

	TEST_F(ProcedureNotificationTest, Given_ProcedureOneAndProcedureTwo_When_ProcedureOneIsSelectedToNotifyAndNewSeriesIsAddedToProcedureTwo_Then_SeriesAddedEventIsNotRaisedForProcedureOne)
	{
		auto procedure1 = newProcedure(Procedure::Type::Coronary);
		auto procedure2 = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure1.parent = study;
		procedure2.parent = study;
		ASSERT_TRUE(studyrepo.add(procedure1));
		ASSERT_TRUE(studyrepo.add(procedure2));
		ASSERT_FALSE(createSeriesAndSubscribeProcedureForNotification(procedure1, procedure2));
	}

	TEST_F(ProcedureNotificationTest, Given_ProcedureOneAndProcedureTwo_When_ProcedureOneIsSelectedToNotifyAndNewSeriesIsAddedToProcedureOne_Then_SeriesAddedEventIsRaisedForProcedureOne)
	{
		auto procedure1 = newProcedure(Procedure::Type::Coronary);
		auto procedure2 = newProcedure(Procedure::Type::Coronary);
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		procedure1.parent = study;
		procedure2.parent = study;
		ASSERT_TRUE(studyrepo.add(procedure1));
		ASSERT_TRUE(studyrepo.add(procedure2));
		
		ASSERT_TRUE(createSeriesAndSubscribeProcedureForNotification(procedure1, procedure1));
	}
}