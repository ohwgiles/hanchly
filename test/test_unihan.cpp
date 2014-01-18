#include <gtest/gtest.h>
#include "unihan.h"

TEST(Unihan, Match) {
	unihan_entry_t* wo = unihan_find("我");
	ASSERT_TRUE(wo);
	EXPECT_EQ(7, wo->total_strokes);
	EXPECT_STREQ("HQI", wo->cangjie);
}

TEST(Unihan, NoMatch) {
	unihan_entry_t* crap = unihan_find("asdf");
	EXPECT_FALSE(crap);
}

