//
// Copyright (c) 2014 Oliver Giles
// All rights reserved.
//
// This file is part of Hanchly (汉查利).
// Hanchly is free software; resdistribution and use are
// permitted under conditions stipulated in the file
// COPYING, which is distributed with Hanchly.
//

#include <gtest/gtest.h>

#include "pinyin.h"

TEST(Pinyin, SyllableBreaks) {
	EXPECT_STREQ("hao0 jiu0 bu0 jian0 le0", pinyin_compose("haojiubujianle"));
	EXPECT_STREQ("zha0 qie0 zi0 gai0 fan0", pinyin_compose("zhaqiezigaifan"));
	EXPECT_STREQ("a0 a0 a0 a0", pinyin_compose("aaaa"));
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

TEST(Pinyin, NonWildcardExactSearch) {
	char haystack[] = "this isn't pinyin, but ni3 hao3 is";
	char* nihao = pinyin_search(haystack, "ni3 hao3");
	EXPECT_EQ(&haystack[23], nihao);
}

TEST(Pinyin, NonWildcardInexactSearch) {
	char haystack[] = "this isn't pinyin, but ni3 hao3 is";
	char* nihao = pinyin_search(haystack, "Ni3 hAo3");
	EXPECT_EQ(&haystack[23], nihao);
}

TEST(Pinyin, WildcardExactSearch) {
	char haystack[] = "this isn't pinyin, but ni3 hao3 is";
	char* nihao = pinyin_search(haystack, "ni3 hao0");
	EXPECT_EQ(&haystack[23], nihao);
	nihao = pinyin_search(haystack, "ni0 hao3");
	EXPECT_EQ(&haystack[23], nihao);
	nihao = pinyin_search(haystack, "ni0 hao0");
	EXPECT_EQ(&haystack[23], nihao);
}

TEST(Pinyin, WildcardInexactSearch) {
	char haystack[] = "this isn't pinyin, but ni3 hao3 is";
	char* nihao = pinyin_search(haystack, "Ni0 haO0");
	EXPECT_EQ(&haystack[23], nihao);
}

TEST(Pinyin, SearchNoMatch) {
	char haystack[] = "gen1 han4 yu3 zai4 yi4 qi3 de english sentence";
	EXPECT_EQ(NULL, pinyin_search(haystack, "ni0 hao0"));
}

