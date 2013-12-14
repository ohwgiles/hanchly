#include <gtest/gtest.h>

#include "enpy.h"

TEST(Enpy, Functional) {
	EXPECT_TRUE(enpy("bang"));
	EXPECT_FALSE(enpy("hello"));
	EXPECT_FALSE(enpy("bang2"));
}

