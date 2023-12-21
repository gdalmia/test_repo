// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/application.h"
#include "Workflow/settings.h"
#include <Automation.h>
#include <fstream>

using namespace SscfTs1;

using namespace Sense::DLS;
using namespace Sense;

namespace Philips { namespace Workflow {

	Application::Settings Application::Settings::instance()
	{
		const auto settings = Workflow::Settings();

		Settings result;
		const auto settingsPath = settings.path() / "application.json";
		if (std::filesystem::exists(settingsPath))
		{
			std::ifstream(settingsPath) >> result;
		}
		else
		{
			std::ofstream(settingsPath) << result;
		}
		return result;
	}

	Application::Settings::Settings() : Serializable("settings") { }

	Application::Settings::Automation::Automation() : Serializable("automation")
	{
		enabled = false;
		host = "0.0.0.0";
	}

	Application::Test::Test(const std::string &urn)
		: _settings(Settings::instance())
		, _eventLoop(_settings.automation.enabled ? std::make_unique<EventLoop>() : nullptr)
		, _serverNode(_settings.automation.enabled 
			? std::make_unique<SscfTs1::ServerNode>(*_eventLoop, _settings.automation.host, urn)
			: nullptr)
	{
	}

	void Application::Test::start() const
	{
		if(_serverNode)
		{
			_serverNode->listen();
		}
	}

	Application::Application()
		: _settings(Settings::instance())
		, _scheduler(_settings.automation.enabled ? std::make_unique<SchedulerTest>() : std::make_unique<Scheduler>())
		, _gpu(0)
	{
	}

	Scheduler& Application::scheduler()
	{
		return *_scheduler;
	}

	Application::View& Application::addView(Sense::Size resolution, Sense::DLS::Canvas::TouchBehavior touch, int screen, View::Type type)
	{
		_views.emplace_back(new View(_gpu, *_scheduler, resolution, touch, screen, _settings, type)); //TICS !OAL#011: Cannot use std::make_unique here as the View constructor is private
		return *_views.back();
	}

	int Application::run()
	{
		for(auto& server: _testServers)
		{
			server->start();
		}
		
		_gpu.show();
		return _scheduler->run();
	}

	void Application::registerTest(std::unique_ptr<Test> server)
	{
		if (!_settings.automation.enabled)
		{
			auto deleteServer = std::move(server);
			return;
		}

		_testServers.emplace_back(std::move(server));
	}

	Application::View::~View() = default;

	Application::View::View(IGpu& gpu, IScheduler& scheduler, Size resolution, TouchBehavior touch, int screen, const Settings& settings, View::Type type)
		: Canvas(gpu, scheduler, settings.automation.enabled ? Animations::Disabled : Animations::Enabled, touch)
	{
		this->size = resolution;
		if (type == View::Type::Remote) return;

		_output = std::make_unique<OutputScreen>(*this, screen);
		if (settings.automation.enabled) _user = std::make_unique<UserTest>(*this);
		else _user = std::make_unique<UserMouseKeyboard>(*this);
	}

	void Application::View::onUserInteraction()
	{
		Canvas::onUserInteraction();
		interacted();
	}

}}