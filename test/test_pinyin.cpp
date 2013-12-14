
#include <gtest/gtest.h>

#include "pinyin.h"

TEST(Pinyin, SyllableBreaks) {
	EXPECT_STREQ("hao0 jiu0 bu0 jian0 le0", pinyin_compose("haojiubujianle"));
	EXPECT_STREQ("zha0 qie0 zi0 gai0 fan0", pinyin_compose("zhaqiezigaifan"));
}

TEST(Pinyin, ForcedBreak) {
	EXPECT_STREQ("xian0", pinyin_compose("xian"));
	EXPECT_STREQ("xi0 an0", pinyin_compose("xi'an"));
}

TEST(Pinyin, ToneNumbers) {
	EXPECT_STREQ("ni3 hao3", pinyin_compose("ni3hao3"));
	EXPECT_FALSE(pinyin_compose("ni7hao3"));
}

TEST(Pinyin, NonSyllables) {
	EXPECT_FALSE(pinyin_compose("hello"));
}

