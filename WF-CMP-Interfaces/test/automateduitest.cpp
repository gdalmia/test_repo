// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "automateduitest.h"

using namespace Sense;
using namespace DLS;
using namespace Automation;

struct AutomatedUiTest::Private
{
	Private(Rect windowRect)
		: gpu(0),
		canvas(gpu, scheduler, CanvasBase::Animations::Disabled),
		output(canvas),
		user(canvas),
		window(windowRect)
	{
		canvas.size = windowRect.size();
		window.setOutput(&output);
	}

	SchedulerTest	scheduler;
	GpuTest			gpu;
	Canvas			canvas;
	OutputTest		output;
	UserTest		user;
	WindowTest		window;
};

class AutomatedUiTest::Singleton
{
public:
	static std::shared_ptr<Singleton> instance(SchedulerTest::Playback speed = SchedulerTest::Fast)
	{
		auto result = _instance.lock();
		if (!result)
		{
			result = std::make_shared<Singleton>(speed);
			_instance = result;
		}
		return result;
	}

	SchedulerTest::Playback automationSpeed;

	Singleton(SchedulerTest::Playback speed)
	{
		automationSpeed = speed;
	}

private:
	static std::weak_ptr<Singleton> _instance;
};

std::weak_ptr<AutomatedUiTest::Singleton> AutomatedUiTest::Singleton::_instance;

AutomatedUiTest::AutomatedUiTest(Sense::Automation::SchedulerTest::Playback speed)
	: s(Singleton::instance(speed))
	, p(nullptr)
{
}

AutomatedUiTest::AutomatedUiTest(Rect windowRect)
	: s(Singleton::instance())
	, p(std::make_shared<Private>(windowRect))
{
}

void AutomatedUiTest::SetUp()
{
	p->gpu.show();
	SchedulerTest::play(s->automationSpeed);
}

void AutomatedUiTest::TearDown()
{
	SchedulerTest::pause();
}

void AutomatedUiTest::TestBody()
{
}

Sense::DLS::Canvas & AutomatedUiTest::canvas() const
{
	return p->canvas;
}
