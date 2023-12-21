// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include <functional>
#include <map>
#include <cassert>

namespace Philips { namespace Workflow {

	//! Event enable a class or object (publisher) to notify other classes or objects (subscribers) when something of interest occurs.
	//! Handlers are invoked in the order of subscription when event is triggered
	//! \note The event is not thread safe
	template<typename... Arguments>
	class Event
	{
	public:
		typedef uint16_t Subscription;
		
		//! Creates an event
		//! \code
		//! /*Event with no arguments*/
		//! Event<> event;
		//!
		//! /*Event with argument*/
		//! struct MyStruct;
		//! Event<MyStruct> eventArg;
		//! \endcode
		Event() : _count(0)
		{
		}

		~Event() = default;

		//! Trigger the event
		//! \code
		//! event();
		//! MyStruct myStruct;
		//! eventArg(myStruct);
		//! \endcode
		void operator()(Arguments... params) const
		{
			call(params...);
		}

		//! Subscribes a function to this event.
		//! \param handler Handler to be invoked when the event is triggered
		//! \return A subscription token is returned which can be used to unsubscribe.
		//! \code
		//!
		//! /*Lambda subscription*/
		//! auto id = event.subscribe([this]() {  });
		//! auto id = eventArg.subscribe([this](MyStruct argument) { });
		//!
		//! /*Class Method subscription*/
		//! auto id = event.subscribe(std::bind(&MyClass::MyClassMethod, this));
		//! auto id = event.subscribe(std::bind(&MyClass::MyClassMethodWithArgument, this, std::placeholders::_1));
		//! \endcode
		//! Refer <a href = "https://en.cppreference.com/w/cpp/utility/functional/bind">std::bind</a> on how to bind class member functions to std::function.
		Subscription subscribe(const std::function<void(Arguments...)>& handler) const
		{
			assert(_count != SubscriptionLimit);

			const auto id{ _count++ };
			_map[id] = handler;
			return id;
		}

		//! Subscribes a function to this event. A subscription token is returned which can be used to unsubscribe.
		//! \param handler Handler to be invoked when the event is triggered
		//! \return A subscription token is returned which can be used to unsubscribe.
		//! \code
		//!
		//! /*Lambda subscription*/
		//!
		//! auto id = event+= [this]() {  };
		//! auto id = eventArg += [this](MyStruct argument) { };
		//!
		//! /*Class Method subscription*/
		//!
		//! auto id = event += std::bind(&MyClass::MyClassMethod, this);
		//! auto id = event+= std::bind(&MyClass::MyClassMethodWithArgument, this, std::placeholders::_1);
		//! \endcode
		//! Refer <a href = "https://en.cppreference.com/w/cpp/utility/functional/bind">std::bind</a> on how to bind class member functions to std::function.
		Subscription operator+=(const std::function<void(Arguments...)>& handler) const
		{
			return subscribe(handler);
		}

		//! Unsubscribes the function associated with the Subscription token from this event
		//! \param subscription Token returned when subscribed
		//! \code
		//! event.unsubscribe(id);
		//! \endcode
		void unsubscribe(const Subscription subscription) const
		{
			assert(_map.count(subscription) != 0);
			_map.erase(subscription);
		}

		//! Unsubscribes the function associated with the Subscription token from this event
		//! \param subscription Token returned when subscribed
		//! \code
		//! event -= id;
		//! \endcode
		void operator-=(const Subscription subscription) const
		{
			unsubscribe(subscription);
		}

		Event(const Event& other): _count(other._count), _map(other._map)
		{};
		
		Event(const Event&& other) noexcept : _count(std::move(other._count)), _map(std::move(other._map))
		{};

		Event& operator=(const Event& other)
		{
			if(this != &other)
			{
				this->_count = other._count;
				this->_map = other._map;
			}
			return *this;
		};

		Event& operator=(Event&& other) noexcept
		{
			if(this != &other)
			{
				this->_count = std::move(other._count);
				this->_map = std::move(other._map);
			}
			return *this;
		};

	private:
		static constexpr Subscription SubscriptionLimit = 10000;
		typedef std::map<Subscription, std::function<void(Arguments...)>> SubscriptionMap;
		
		void call(Arguments... params) const
		{
			const auto copy = _map;
			for (const auto& pair : copy)
			{
				pair.second(params...);
			}
		}

		mutable Subscription _count;
		mutable SubscriptionMap _map;
	};
}}
