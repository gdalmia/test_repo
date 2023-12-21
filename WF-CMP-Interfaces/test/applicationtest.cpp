// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "iapplication.h"
#include <Workflow/application.h>
#include <Workflow/settings.h>
#include <Automation.h>
#include <gtest/gtest.h>
#include <fstream>

using namespace Philips::Workflow;

class ApplicationTest : public testing::Test
{
public:
	ApplicationTest():settings(std::filesystem::temp_directory_path(), true)
	{
	}
	
	void SetUp() override
	{
		std::filesystem::remove_all(settings.path());
		std::filesystem::create_directory(settings.path());
	}

	void TearDown() override
	{
		std::filesystem::remove_all(settings.path());
	}

	Application::Settings createApplicationSettings(bool testEnabled) const
	{
		Application::Settings appSettings;
		appSettings.automation.enabled = testEnabled;
		appSettings.automation.host = "127.0.0.1";

		const auto filePath = settings.path() / "application.json";
		std::ofstream(filePath) << appSettings;

		return appSettings;
	}
	
	Settings settings;
};

class ApplicationServer: public Philips::Workflow::Test::IApplication, public Application::IAutomate
{
public:
	ApplicationServer()
	{
		destructed = false;
	}
	
	~ApplicationServer()
	{
		destructed = true;
	}
	static bool destructed;
};

bool ApplicationServer::destructed = false;

class ApplicationClient
{
public:
	ApplicationClient(const std::string& host, const std::string& urn)
		: loop("test-client-thread")
	{
		loop.start();
		loop.post([this, host, urn]
		{
			node = std::make_unique<SscfTs1::ClientNode>(loop, host, urn);
		});
	}

	~ApplicationClient()
	{
		loop.send([this]()
		{
			node->disconnect();
			node.reset(nullptr);
		});
		loop.stop();
	}

	bool connect()
	{
		auto connected { false };
		loop.send([&connected, this]()
		{
			connected = node->connect();
		});
		return connected;
	}

private:
	SscfTs1::EventLoop loop;
	std::unique_ptr<SscfTs1::ClientNode> node;
};

TEST_F(ApplicationTest, Given_SettingsIsAvailable_WhenApplicationTestCreatedAndServerRegistered_Then_TestClientGetsConnected)
{
	const auto appSettings =  createApplicationSettings(true);

	const std::string urn = "application-test._itest._ph-appos-dh._tcp.local";
	Application::Test host(urn);
	host.registerTest<Philips::Workflow::Test::IApplication>(std::make_unique<ApplicationServer>());
	host.start();

	ApplicationClient client(appSettings.automation.host, urn);
	ASSERT_TRUE(client.connect());
}

TEST_F(ApplicationTest, Given_SettingsIsNotAvailable_WhenApplicationTestCreatedAndServerRegistered_Then_ServerGetsDeleted)
{
	const std::string urn = "application-test._itest._ph-appos-dh._tcp.local";
	Application::Test test(urn);
	test.registerTest<Philips::Workflow::Test::IApplication>(std::make_unique<ApplicationServer>());
	test.start();

	ASSERT_TRUE(ApplicationServer::destructed);
}

TEST_F(ApplicationTest, Given_SettingsIsNotAvailable_WhenApplicationCreatedAndServerRegistered_Then_TestServerGetsDeleted)
{
	const std::string urn = "application-test._itest._ph-appos-dh._tcp.local";
	auto test = std::make_unique<Application::Test>(urn);
	test->registerTest<Philips::Workflow::Test::IApplication>(std::make_unique<ApplicationServer>());
	
	Application application;
	application.registerTest(std::move(test));

	ASSERT_TRUE(ApplicationServer::destructed);
}

TEST_F(ApplicationTest, Given_SettingsIsAvailable_WhenApplicationCreatedAndServerRegistered_Then_ServerIsNotDestructed)
{
	const auto appSettings = createApplicationSettings(true);
	
	const std::string urn = "application-test._itest._ph-appos-dh._tcp.local";
	auto test = std::make_unique<Application::Test>(urn);
	test->registerTest<Philips::Workflow::Test::IApplication>(std::make_unique<ApplicationServer>());

	ASSERT_FALSE(ApplicationServer::destructed);
}

TEST_F(ApplicationTest, When_ViewOfSizeIsAdded_Then_ViewSetOnApplication)
{
	Application application;
	
	const auto size = Sense::Size(100, 200);
	auto& view = application.addView(size);

	ASSERT_EQ(view.size, size);
	ASSERT_EQ(view.touchBehavior(), Sense::DLS::Canvas::TouchBehavior::Mouse);
}


TEST_F(ApplicationTest, When_ViewOfSizeIsAddedWithTouchBehaviour_Then_ViewSetOnApplication)
{
	Application application;

	const auto size = Sense::Size(100, 200);
	auto& view = application.addView(size, Sense::DLS::Canvas::TouchBehavior::Touch, 1);

	ASSERT_EQ(view.size, size);
	ASSERT_EQ(view.touchBehavior(), Sense::DLS::Canvas::TouchBehavior::Touch);
}

TEST_F(ApplicationTest, When_MultipleViewOfSizeIsAdded_Then_ViewsSetOnApplication)
{
	Application application;

	const auto size1 = Sense::Size(100, 200);
	const auto size2 = Sense::Size(50, 300);
	auto& view1 = application.addView(size1, Sense::DLS::Canvas::TouchBehavior::Mouse, 0);
	auto& view2 = application.addView(size2, Sense::DLS::Canvas::TouchBehavior::Touch, 1);

	ASSERT_EQ(view1.size, size1);
	ASSERT_EQ(view1.touchBehavior(), Sense::DLS::Canvas::TouchBehavior::Mouse);

	ASSERT_EQ(view2.size, size2);
	ASSERT_EQ(view2.touchBehavior(), Sense::DLS::Canvas::TouchBehavior::Touch);
}

TEST_F(ApplicationTest, When_RemoteViewOfSizeIsAdded_Then_ViewsSetOnApplication)
{
	Application application;

	const auto size1 = Sense::Size(100, 200);
	auto& view1 = application.addView(size1, Sense::DLS::Canvas::TouchBehavior::Mouse, 0, Application::View::Type::Remote);

	ASSERT_EQ(view1.size, size1);
	ASSERT_EQ(view1.touchBehavior(), Sense::DLS::Canvas::TouchBehavior::Mouse);
}

TEST_F(ApplicationTest, When_ButtonPressed_Then_EventIsRaised)
{
	const auto appSettings = createApplicationSettings(true);
	bool flag = false;
	Application testApplication;

	const auto testSize = Sense::Size(100, 200);
	auto& testView = testApplication.addView(testSize);
	testView.interacted += [&flag]() { flag = true; };

	Sense::DLS::Button button(testView);
	Sense::Automation::Peer<Sense::DLS::Button> testButton(button);
	testButton.mouseClick();
	
	ASSERT_TRUE(flag);
}
