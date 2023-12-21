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
	// Suppressed as warning is thrown for all Test cases related to override specifier, "new" inside smart pointers 
	// and members are used in TestCases, so made protected
	class StudyNotificationTest : public testing::Test
	{
	public:

		StudyNotificationTest():_settings(std::experimental::filesystem::temp_directory_path(), false)
		{
		}

		~StudyNotificationTest() = default;

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		bool createProcedureAndSubscribeNotification(Study notificationStudy, Study nonNotificationStudy)
		{
			bool flag = false;
			SscfTs1::EventLoop waitEventLoop;
			const std::unique_ptr<IStudyNotification> notification = studyrepo.subscribe(notificationStudy);
			auto procedure = newProcedure(Procedure::Type::Coronary);
			notification->procedureAdded.subscribe([&](const Procedure&)
			{
				flag = true;
				waitEventLoop.stop();
			});
			procedure.parent = nonNotificationStudy;
			studyrepo.add(procedure);
			waitEventLoop.run(2000);
			return flag;
		}

		bool subscribeNotificationAndUpdateStudy(Study studyToBeNotified, Study studyToBeChanged)
		{
			bool flag = false;
			SscfTs1::EventLoop waitEventLoop;
			studyToBeChanged.accessionNumber = "changed";
			SscfTs1::EventLoop waitEventLoopForChanged;
			const std::unique_ptr<IStudyNotification> notification = studyrepo.subscribe(studyToBeNotified);
			notification->studyChanged.subscribe([&](const Study&)
			{
				flag = true;
				waitEventLoopForChanged.stop();
			});
			studyrepo.update(studyToBeChanged);
			waitEventLoopForChanged.run(2000);// need to wait for study to be changed in database
			return flag;
		}
		
	protected:
		Settings                                    _settings;
		StudyRepository                             studyrepo;
	};

	

	TEST_F(StudyNotificationTest, Given_Study_When_ProcedureAdded_Then_ProcedureAddedEventIsRaised)
	{
		bool flag = false;
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		SscfTs1::EventLoop waitEventLoop;
		const std::unique_ptr<IStudyNotification> notification = studyrepo.subscribe(study);
		auto procedure = newProcedure(Procedure::Type::Coronary);
		notification->procedureAdded.subscribe([&](const Procedure&)
		{
			flag = true;
			waitEventLoop.stop();
		});
		procedure.parent = study;
		studyrepo.add(procedure);
		ASSERT_EQ(waitEventLoop.run(2000), 0) << "Timeout. Failed to receive the Event.";      // Wait for the event.
		ASSERT_TRUE(flag);
	}

	TEST_F(StudyNotificationTest, Given_Study_When_Changed_Then_StudyChangedEventIsRaised)
	{
		SscfTs1::EventLoop waitEventLoopForChanged;
		auto study = newStudy();
		studyrepo.add(study);
		waitEventLoopForChanged.run(2000); // need to wait for study to be created in database
		ASSERT_TRUE(subscribeNotificationAndUpdateStudy(study, study));
	}

	TEST_F(StudyNotificationTest, Given_Study_When_Changed_Then_StudyChangedObjectAndUpdatedStudyObjectAreSame)
	{
		SscfTs1::EventLoop waitEventLoopForChanged;
		bool flag = false;
		auto study = newStudy();
		studyrepo.add(study);
		waitEventLoopForChanged.run(2000);// need to wait for study to be created in database
		study.accessionNumber = "changed";
		const std::unique_ptr<IStudyNotification> notification = studyrepo.subscribe(study);
		Study changedStudy;
		notification->studyChanged.subscribe([&](const Study& study_)
		{
			flag = true;
			changedStudy = study_;
			waitEventLoopForChanged.stop();
		});
		studyrepo.update(study);
		waitEventLoopForChanged.run(2000); // need to wait for study to be changed in database
		ASSERT_TRUE(flag);
		ASSERT_TRUE(verifyStudyInfo(changedStudy,study));
	}

	TEST_F(StudyNotificationTest, Given_TwoStudies_When_NotifiedStudyDoesNotChangeThen_StudyChangedEventIsNotRaised)
	{
		auto studyOne = newStudy();
		auto studyTwo = newStudy();
		studyrepo.add(studyOne);
		studyrepo.add(studyTwo);
		SscfTs1::EventLoop waitEventLoopForChanged;
		
		waitEventLoopForChanged.run(2000);// need to wait for study to be created in database
		ASSERT_FALSE(subscribeNotificationAndUpdateStudy(studyOne, studyTwo));
	}

	TEST_F(StudyNotificationTest, Given_Study_When_ProcedureRemoved_Then_ProcedureRemovedEventIsRaised)
	{
		bool flag = false;
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		SscfTs1::EventLoop waitEventLoopForRemove;
		const std::unique_ptr<IStudyNotification> notification = studyrepo.subscribe(study);

		notification->procedureRemoved.subscribe([&](IRepository::Model)
		{
			flag = true;
			waitEventLoopForRemove.stop();
		});
		auto procedure = newProcedure(Procedure::Type::Coronary);
		procedure.parent = study;
		studyrepo.add(procedure);

		studyrepo.remove(procedure);
		ASSERT_EQ(waitEventLoopForRemove.run(2000), 0) << "Timeout. Failed to receive remove Event.";      // Wait for the event.
		ASSERT_TRUE(flag);
	}

	TEST_F(StudyNotificationTest, Given_Study_When_ProcedureAdded_Then_VerifyParametersOfExpectedProcedureAndActualProcedureAreSame)
	{
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		SscfTs1::EventLoop waitEventLoop;
		const std::unique_ptr<IStudyNotification> notification = studyrepo.subscribe(study);
		auto actualProcedure = newProcedure(Procedure::Type::Coronary);
		Procedure expectedProcedure;
		notification->procedureAdded.subscribe([&](const Procedure& output)
		{
			expectedProcedure = output;
			waitEventLoop.stop();
		});
		actualProcedure.parent = study;
		studyrepo.add(actualProcedure);
		ASSERT_EQ(waitEventLoop.run(2000), 0) << "Timeout. Failed to receive the Event.";      // Wait for the event.
		ASSERT_EQ(verifyProcedureInfo(actualProcedure, expectedProcedure), true);
	}

	TEST_F(StudyNotificationTest, Given_Study_When_ProcedureRemoved_Then_VerifyExpectedModelAndActualModelAreSame)
	{
		auto study = newStudy();
		ASSERT_TRUE(studyrepo.add(study));
		SscfTs1::EventLoop waitEventLoopForRemove;
		const std::unique_ptr<IStudyNotification> notification = studyrepo.subscribe(study);
		IRepository::Model expectedModel;
		notification->procedureRemoved.subscribe([&](IRepository::Model model)
		{
			expectedModel = model;
			waitEventLoopForRemove.stop();
		});
		auto actualProcedure = newProcedure(Procedure::Type::Coronary);
		actualProcedure.parent = study;
		studyrepo.add(actualProcedure);

		studyrepo.remove(actualProcedure);
		ASSERT_EQ(waitEventLoopForRemove.run(2000), 0) << "Timeout. Failed to receive remove Event.";      // Wait for the event.
		ASSERT_EQ(expectedModel, IRepository::Model(actualProcedure));
	}

	TEST_F(StudyNotificationTest, Given_StudyOneAndStudyTwo_When_StudyOneSelectedForNotificationAndNewProcedureIsAddedToStudyTwo_Then_ProcedureAddedEventIsNotRaisedForStudyOne)
	{
		
		auto study1 = newStudy();
		auto study2 = newStudy();
		ASSERT_TRUE(studyrepo.add(study1));
		ASSERT_TRUE(studyrepo.add(study2));
		ASSERT_FALSE(createProcedureAndSubscribeNotification(study1,study2));
	}
	
	TEST_F(StudyNotificationTest, Given_StudyOneAndStudyTwo_When_StudyOneSelectedForNotificationAndNewProcedureIsAddedToStudyOne_Then_ProcedureAddedEventIsRaisedForStudyOne)
	{
		auto study1 = newStudy();
		auto study2 = newStudy();
		ASSERT_TRUE(studyrepo.add(study1));
		ASSERT_TRUE(studyrepo.add(study2));
		
		ASSERT_TRUE(createProcedureAndSubscribeNotification(study1, study1));
	}
}