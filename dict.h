//
// Copyright (c) 2014 Oliver Giles
// All rights reserved.
//
// This file is part of Hanchly (汉查利).
// Hanchly is free software; resdistribution and use are
// permitted under conditions stipulated in the file
// COPYING, which is distributed with Hanchly.
//
#ifndef _HANCHLY_CEDICT_H_
#define _HANCHLY_CEDICT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	HANCHLY_CEDICT_HANZI,
	HANCHLY_CEDICT_PINYIN,
	HANCHLY_CEDICT_ENGLISH
} cedict_search_t;

typedef struct {
	char* hanzi;
	char* pinyin;
	char* english;
} cedict_result_t;

typedef struct {
	cedict_search_t type;
	void* search_from;
	int num_matches;
	int exact_only;
	cedict_result_t* results;
} cedict_t;

cedict_t cedict_search(cedict_t param, const char* term);

void cedict_free(cedict_t param);

#ifdef __cplusplus
}
#endif


#endif // _HANCHLY_CEDICT_H_
