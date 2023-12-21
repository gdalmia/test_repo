// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>
#include "automateduitest.h"

Sense::SchedulerTest::Playback getPlaybackSpeed(int argc, char**argv)
{
	for (auto i = 0; i < argc; i++)
	{
		if ("-p:slow" == std::string(argv[i]))	return Sense::SchedulerTest::Playback::Slow;
		else if ("-p:fast" == std::string(argv[i]))	return Sense::SchedulerTest::Playback::Fast;
		else if ("-p:turbo" == std::string(argv[i])) return Sense::SchedulerTest::Playback::Turbo;
	}
	return Sense::SchedulerTest::Playback::Fast;
}

int main(int argc, char**argv)
{
	testing::InitGoogleTest(&argc, argv);
	AutomatedUiTest test(getPlaybackSpeed(argc,argv));
	return RUN_ALL_TESTS();
}