//
// Copyright (c) 2014 Oliver Giles
// All rights reserved.
//
// This file is part of Hanchly (汉查利).
// Hanchly is free software; resdistribution and use are
// permitted under conditions stipulated in the file
// COPYING, which is distributed with Hanchly.
//
#ifndef _HANCHLY_UNIHAN_H_
#define _HANCHLY_UNIHAN_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char frequency;
	char total_strokes;
	char hsk_grade;
	char radical;
	char additional_strokes;
	char cangjie[5];
	char definition[];
} unihan_entry_t;

unihan_entry_t* unihan_find(const char* hanzi);

#ifdef __cplusplus
}
#endif

#endif // _HANCHLY_UNIHAN_H_
