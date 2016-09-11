#include "Engine/Engine.h"
#include "gtest/gtest.h"

TEST(Example, IntentionalSuccess) 
{
	// Do something
	Engine engine;

	// Make some assertions
	ASSERT_TRUE(true);
}

TEST(Example, IntentionalFailure)
{
	// Do something
	Engine engine;

	// Make some assertions
	ASSERT_FALSE(true);
}