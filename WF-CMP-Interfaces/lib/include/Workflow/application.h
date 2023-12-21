// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "Workflow/event.h"
#include <sscfts1.h>
#include <net/object.h>

#include <Sense.h>
#include <DLS.h>

#include <memory>

namespace Philips { namespace Workflow { 

	//! Represents an automatable Sense application
	class Application
	{
	public:
		class IAutomate { public: virtual ~IAutomate() = default; };

		struct Settings : Net::Object::Serializable //TICS !OOP#013: Serializable is part of AppEngine library
		{
			static Settings instance();

			Settings();

			struct Automation : Serializable //TICS !OOP#013: Serializable is part of AppEngine library
			{
				Automation();

				Attribute<bool> enabled = { this , "enabled" };
				Attribute<std::string> host = { this , "host" };
			};

			Attribute<Automation> automation = { this, "automation" };
		};

		//! The test automation server host
		class Test
		{
		public:
			explicit Test(const std::string &urn);
			
			//! Starts the test server
			void start() const;

			//! Registers a test interface for test service
			//! \param server Server implementation of IInterface
			template<typename Interface>
			void registerTest(std::unique_ptr<IAutomate> server)
			{
				if (!_serverNode)
				{
					auto deleteServer = std::move(server);
					return;
				}
				
				_serverNode->registerSingleton<Interface>(dynamic_cast<Interface*>(server.get()));
				_servers.emplace_back(std::move(server));
			}

		private:
			Settings										_settings;
			std::unique_ptr<SscfTs1::EventLoop>				_eventLoop;
			std::unique_ptr<SscfTs1::ServerNode>			_serverNode;
			std::vector<std::unique_ptr<IAutomate>>			_servers;
		};

		//! Represents a view for the application.
		//! An application can have one view or have multiple views (e.g.: Main view, Cart view, Tsm View)
		class View final : public Sense::DLS::Canvas
		{
			friend class Application;
		public:
			//! Defines the type of view.
			enum class Type
			{
				Local = 0,	//!< View is displayed on the locally connected display 
				Remote		//!< View is displayed on the network connected display
			};

			//! Destructor
			~View();

			View(const View&) = delete;
			View& operator=(const View&) = delete;

			//! Raised when a mouse, keyboard or touch event occured.
			Event<>		   interacted;

		private:
			//! Creates an instance of application
			explicit View(Sense::IGpu& gpu, Sense::IScheduler& scheduler, Sense::Size resolution, Sense::DLS::Canvas::TouchBehavior touch, int screen, const Settings& settings, Type type);

			void onUserInteraction()		override;

			std::unique_ptr<Sense::IOutput>		_output;
			std::unique_ptr<Sense::UserBase>	_user;
		};
		
		//! Creates an instance of application
		Application();

		//! Destructor
		~Application() = default;

		//! Gets the application scheduler
		Sense::Scheduler& scheduler();

		//! Add another screen to the application
		Application::View& addView(Sense::Size resolution, Sense::DLS::Canvas::TouchBehavior touch = Sense::DLS::Canvas::TouchBehavior::Mouse, int screen = 0, View::Type type = View::Type::Local);

		//! Runs the application
		int run();

		//! Registers a test server in the application
		//! \param server Test automation server
		void registerTest(std::unique_ptr<Test> server);
		
	private:
		Settings								_settings;
		std::unique_ptr<Sense::Scheduler>		_scheduler;
		Sense::Gpu								_gpu;

		std::vector<std::unique_ptr<View>>		_views;
		std::vector<std::unique_ptr<Test>>		_testServers;
	};
}}
