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
#include "cedict.h"


TEST(Cedict, EnglishCaseInsensitiveSearch) {
	setlocale(LC_CTYPE,"");
	cedict_t c;
	c.num_matches = 1;
	c.type = HANCHLY_CEDICT_ENGLISH;
	c.exact_only = 0;
	c = cedict_search(c, "hEllO");
	ASSERT_EQ(1, c.num_matches);
	EXPECT_STREQ("你好", c.results[0].hanzi);
	EXPECT_STREQ("ni3 hao3", c.results[0].pinyin);
	EXPECT_STREQ("Hello!/Hi!/How are you?", c.results[0].english);
}

TEST(Cedict, ExactResultMatch) {
	cedict_t c;
	c.num_matches = 1;
	c.type = HANCHLY_CEDICT_ENGLISH;
	c.exact_only = 1;
	c = cedict_search(c, "Hello!");
	EXPECT_EQ(1, c.num_matches);
}

TEST(Cedict, InexactResultNoMatch) {
	cedict_t c;
	c.num_matches = 1;
	c.type = HANCHLY_CEDICT_ENGLISH;
	c.exact_only = 1;
	c = cedict_search(c, "Hello");
	EXPECT_EQ(0, c.num_matches);
}

TEST(Cedict, HanziSearch) {
	cedict_t c;
	c.num_matches = 1;
	c.type = HANCHLY_CEDICT_HANZI;
	c.exact_only = 1;
	c = cedict_search(c, "你好");
	ASSERT_EQ(1, c.num_matches);
	EXPECT_STREQ("你好", c.results[0].hanzi);
	EXPECT_STREQ("ni3 hao3", c.results[0].pinyin);
	EXPECT_STREQ("Hello!/Hi!/How are you?", c.results[0].english);
}

TEST(Cedict, PinyinWellFormedSearch) {
	cedict_t c;
	c.num_matches = 1;
	c.type = HANCHLY_CEDICT_PINYIN;
	c.exact_only = 1;
	c = cedict_search(c, "ni3 hao3");
	ASSERT_EQ(1, c.num_matches);
	EXPECT_STREQ("你好", c.results[0].hanzi);
	EXPECT_STREQ("ni3 hao3", c.results[0].pinyin);
	EXPECT_STREQ("Hello!/Hi!/How are you?", c.results[0].english);
}

TEST(Cedict, PinyinCaseInsensitiveWildcardSearch) {
	cedict_t c;
	c.num_matches = 1;
	c.type = HANCHLY_CEDICT_PINYIN;
	c.exact_only = 1;
	c = cedict_search(c, "ni0 hao0");
	ASSERT_EQ(1, c.num_matches);
	EXPECT_STREQ("你好", c.results[0].hanzi);
	EXPECT_STREQ("ni3 hao3", c.results[0].pinyin);
	EXPECT_STREQ("Hello!/Hi!/How are you?", c.results[0].english);
}

