// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include <string>
#include <memory>
#include <functional>
#include <thread>

namespace Philips { namespace Workflow { 

	class WorkLoop
	{
	public:
		enum class Priority { Normal, Background };
		WorkLoop(const std::string& name = "WorkLoop-Unnamed", Priority priority = Priority::Normal);
		~WorkLoop();

		WorkLoop(const WorkLoop&)				= delete;
		WorkLoop &operator=(const WorkLoop&)	= delete;
		WorkLoop(const WorkLoop&&)				= delete;
		WorkLoop &operator=(const WorkLoop&&)	= delete;

		void post(const std::function<void(void)>& action);
		bool idle() const;
		Priority priority() const;

	private:
		struct Context;
		static void loop(const std::string& name, std::shared_ptr<Context> ctx, Priority priority);
		static void setBackgroundPriority();
		static void removeBackgroundPriority();

		std::shared_ptr<Context>	context;
		mutable std::thread			thread;
	};

}}