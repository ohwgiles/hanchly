
#include <gtest/gtest.h>

#include "pinyin.h"

TEST(Pinyin, syllables) {
	EXPECT_STREQ("jue0 ding0", pinyin_compose("jueding"));
}
