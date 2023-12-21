#include <gtest/gtest.h>
#include "Workflow/uuid.h"

namespace Test
{
	class UuidTest : public ::testing::Test
	{
	};

	TEST_F(UuidTest, Then_GeneratesUniqueId)
	{
		ASSERT_NE(Philips::Workflow::Uuid::generate(), Philips::Workflow::Uuid::generate());
	}
	
}
