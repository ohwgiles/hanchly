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

#include "enpy.h"

TEST(Enpy, Functional) {
	EXPECT_TRUE(enpy("bang"));
	EXPECT_FALSE(enpy("hello"));
	EXPECT_FALSE(enpy("bang2"));
}

