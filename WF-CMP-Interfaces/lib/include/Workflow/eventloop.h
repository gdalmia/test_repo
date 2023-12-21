// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include <string>
#include <memory>
#include <functional>

namespace Philips { namespace Workflow {

	//! Post messages to the thread on which this object is created.
	class EventLoop final
	{
	public:
		EventLoop();
		~EventLoop() = default;

		EventLoop(const EventLoop&) = delete;
		EventLoop(const EventLoop&&) = delete;
		EventLoop &operator=(const EventLoop&) = delete;
		EventLoop &operator=(const EventLoop&&) = delete;

		//! Execute action on the thread this object is created on.
		void post(const std::function<void(void)>& action);
	private:
		class Singleton;
		std::shared_ptr<Singleton> instance;
	};

}}