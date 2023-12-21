#include "Workflow/workloop.h"
#include <Trace.h>

#include <queue>
#include <condition_variable>
#include <future>
#include <Windows.h>

namespace Philips { namespace Workflow { 

	struct WorkLoop::Context
	{
		std::condition_variable					signal{};
		bool									running{ true };
		bool									idle{ true };
		std::mutex								mutex{};
		std::queue<std::function<void(void)>>	actions{};
	};

	WorkLoop::WorkLoop(const std::string& name, Priority priority)
		: context(std::make_shared<Context>())
		, thread(&WorkLoop::loop, name, context, priority)
	{
		TraceScope() << name;
	}

	WorkLoop::~WorkLoop()
	{
		{
			std::lock_guard<std::mutex> lock(context->mutex);
			context->running = false;
			while (!context->actions.empty()) context->actions.pop();
			context->actions.emplace([this] {});
			context->signal.notify_one();
		}

		thread.join();
	}

	void WorkLoop::post(const std::function<void(void)>& action)
	{
		std::lock_guard<std::mutex> lock(context->mutex);
		context->actions.emplace(action);
		context->signal.notify_one();
	}

	bool WorkLoop::idle() const
	{
		std::lock_guard<std::mutex> lock(context->mutex);
		return context->idle && context->actions.empty();
	}

	WorkLoop::Priority WorkLoop::priority() const
	{
		auto p = GetThreadPriority(thread.native_handle());
		if (p == THREAD_PRIORITY_NORMAL) return WorkLoop::Priority::Normal;

		return WorkLoop::Priority::Background;
	}

	void WorkLoop::loop(const std::string& name, std::shared_ptr<Context> context, Priority priority)
	{
		TraceScope() << name;
		if (priority == Priority::Background) setBackgroundPriority();
		while (true)
		{
			std::unique_lock<std::mutex> lock(context->mutex);
			if (!context->running) break;
			context->idle = true;
			if (context->actions.empty()) context->signal.wait(lock, [&context] { return !context->actions.empty(); });
			context->idle = false;
			auto work = std::move(context->actions.front());
			context->actions.pop();
			lock.unlock();

			work();
		}
		if (priority == Priority::Background) removeBackgroundPriority();
	}

	void WorkLoop::setBackgroundPriority()
	{
		TraceScope();

		if (SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN) == 0)
		{
			TraceWarning() << "Fail to set the thread priority to Background mode, the thread will run on normal priority. Error: " << GetLastError();
		}
	}

	void WorkLoop::removeBackgroundPriority()
	{
		TraceScope();

		if (SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_END) == 0)
		{
			TraceWarning() << "Fail to set the thread priority to end Background mode. Error: " << GetLastError();
		}
	}

}}
