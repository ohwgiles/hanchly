#include <gtest/gtest.h>
#include "cedict.h"


TEST(Cedict, EnglishCaseInsensitiveSearch) {
	setlocale(LC_CTYPE,"");
	cedict_t c;
	c.num_matches = 1;
	c.type = HANCHLY_CEDICT_ENGLISH;
	c = cedict_search(c, "hEllO");
	ASSERT_EQ(1, c.num_matches);
	EXPECT_STREQ("你好", c.results[0].hanzi);
	EXPECT_STREQ("ni3 hao3", c.results[0].pinyin);
	EXPECT_STREQ("Hello!/Hi!/How are you?", c.results[0].english);
}

TEST(Cedict, HanziSearch) {
	cedict_t c;
	c.num_matches = 1;
	c.type = HANCHLY_CEDICT_HANZI;
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
	c = cedict_search(c, "ni0 hao0");
	ASSERT_EQ(1, c.num_matches);
	EXPECT_STREQ("你好", c.results[0].hanzi);
	EXPECT_STREQ("ni3 hao3", c.results[0].pinyin);
	EXPECT_STREQ("Hello!/Hi!/How are you?", c.results[0].english);
}

