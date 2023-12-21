// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>
#include <Sense.h>
#include <DLS.h>
#include <Automation.h>
#include <memory>

#pragma once

class AutomatedUiTest : public testing::Test
{
public:
	AutomatedUiTest(Sense::Automation::SchedulerTest::Playback speed);

protected:
	explicit AutomatedUiTest(Sense::Rect windowRect);

	void SetUp() override;

	void TearDown() override;

	void TestBody() override;

	Sense::DLS::Canvas& canvas() const;

private:
	class Singleton;
	std::shared_ptr<Singleton> s;

	struct Private;
	std::shared_ptr<Private> p;
};

