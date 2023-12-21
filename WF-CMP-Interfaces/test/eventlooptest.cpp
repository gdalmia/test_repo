// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/eventloop.h"
#include "windowmessageprocessor.h"
#include <thread>
#include <gtest/gtest.h>
#include <Windows.h>

using namespace Philips::Workflow;

class EventLoopTest : public testing::Test
{
public:
	EventLoopTest() 
		: threadId(std::this_thread::get_id()) 
		, actionCalled(false)
	{ 
	}

	void action()
	{
		actionCalled = true;
		ASSERT_EQ(threadId, std::this_thread::get_id());
	}

	bool actionExecuted() const
	{
		return actionCalled;
	}

private:

	bool actionCalled;
	std::thread::id threadId;
};

TEST_F(EventLoopTest, whenPostIsCalledThenItCallTheCallbackOnSameThreadEventLoopIsCreatedOn)
{
	EventLoop loop;
	std::thread t([&] { loop.post([this] { action(); }); });
	t.join();
	const int timeoutInMs = 2000;
	ASSERT_TRUE(WindowMessageProcessor::processMessageWhile([this] { return !actionExecuted(); }, timeoutInMs));
}
