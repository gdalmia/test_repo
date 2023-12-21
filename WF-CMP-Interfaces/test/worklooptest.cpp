// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/workloop.h"
#include <gtest/gtest.h>

#include "Windows.h"

using namespace Philips::Workflow;

class WorkLoopTest : public testing::Test
{
public:
	WorkLoopTest()
		: actionCalled(false)
		, threadId(std::this_thread::get_id())
	{
	}

	void action()
	{
		actionCalled = true;
		ASSERT_NE(threadId, std::this_thread::get_id());
	}

	bool actionExecuted() const
	{
		return actionCalled;
	}

private:

	bool actionCalled;
	std::thread::id threadId;
};

TEST_F(WorkLoopTest, whenPostIsCalledThenItCallTheCallbackOnWorkerThread)
{
	WorkLoop worker;
	worker.post([this]
	{
		action();
	});
	while (!worker.idle());
	ASSERT_TRUE(actionExecuted());
}

TEST_F(WorkLoopTest, whenWorkLoopIsConstructedWithBackgroundPriorityThenCallbackIsRunOnBackgroundPriority)
{
	WorkLoop worker("test", WorkLoop::Priority::Background);
	worker.post([this, &worker] 
	{
		ASSERT_EQ(worker.priority(), WorkLoop::Priority::Background);
		action(); 
	});
	while (!worker.idle());
	ASSERT_TRUE(actionExecuted());
}

TEST_F(WorkLoopTest, whenWorkLoopIsConstructedWithNormalPriorityThenCallbackIsRunOnNormalPriority)
{
	WorkLoop worker("test", WorkLoop::Priority::Normal);
	worker.post([this, &worker]
	{
		ASSERT_EQ(worker.priority(), WorkLoop::Priority::Normal);
		action();
	});
	while (!worker.idle());
	ASSERT_TRUE(actionExecuted());
}