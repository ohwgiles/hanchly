//
// Copyright (c) 2014 Oliver Giles
// All rights reserved.
//
// This file is part of Hanchly (汉查利).
// Hanchly is free software; resdistribution and use are
// permitted under conditions stipulated in the file
// COPYING, which is distributed with Hanchly.
//
#ifndef _HANCHLY_PINYIN_H_
#define _HANCHLY_PINYIN_H_

#ifdef __cplusplus
extern "C" {
#endif

char* pinyin_compose(const char* s);

char* pinyin_search(const char* haystack, const char* needle);

#ifdef __cplusplus
}
#endif

#endif // _HANCHLY_PINYIN_H_
