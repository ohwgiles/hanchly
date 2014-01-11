//
// Copyright (c) 2014 Oliver Giles
// All rights reserved.
//
// This file is part of Hanchly (汉查利).
// Hanchly is free software; resdistribution and use are
// permitted under conditions stipulated in the file
// COPYING, which is distributed with Hanchly.
//
#include "cedict.h"
#include "pinyin.h"
#include <string.h>

extern char _binary_cedict_start;
extern char _binary_cedict_end;

const char RECORD_SEPARATOR = 0x1e;
const char FIELD_SEPARATOR = 0x1f;

cedict_t cedict_search(cedict_t param, const char* term) {
	size_t len = strlen(term);
	const char* p = &_binary_cedict_start-1;

	int n = 0;

	char* (*search_fn)(const char*, const char*);
	switch(param.type) {
	case HANCHLY_CEDICT_PINYIN:
		search_fn = pinyin_search;
		break;
	case HANCHLY_CEDICT_ENGLISH:
		search_fn = strcasestr;
		break;
	case HANCHLY_CEDICT_HANZI:
	default:
		search_fn = strstr;
		break;
	}

	cedict_result_t* results = (cedict_result_t*) malloc(param.num_matches * sizeof(cedict_result_t));

	char* last_record = 0;
	while(n < param.num_matches && p < &_binary_cedict_end) {
		p = (*search_fn)(p+1, term);
		// bail out if there are no more matches
		if(!p) break;
		// find the beginning of this entry
		char* rs = memrchr(&_binary_cedict_start, RECORD_SEPARATOR, p-&_binary_cedict_start);
		// if it's the same entry as the last match, discard it
		if(rs == last_record)
			continue;
		// find the field delimiters
		char* sep1 = rawmemchr(rs+1,FIELD_SEPARATOR);
		char* sep2 = rawmemchr(sep1+1,FIELD_SEPARATOR);
		char* end = rawmemchr(sep2+1,FIELD_SEPARATOR);
		// determine result type by testing in which field the match was found
		cedict_search_t result_type = (p < sep1 ? HANCHLY_CEDICT_HANZI : p < sep2 ? HANCHLY_CEDICT_PINYIN : HANCHLY_CEDICT_ENGLISH);
		// ignore matches in incorrect fields
		if(result_type != param.type)
			continue;
		// if we are only supposed to match exact results, test for this
		if(param.exact_only && !(
			// condition for exact match
			(p[-1] == FIELD_SEPARATOR || p[-1] == RECORD_SEPARATOR || p[-1] == '/') && (p[len] == FIELD_SEPARATOR || p[len] == '/')
		)) continue;
		// otherwise we're good, continue
		results[n++] = (cedict_result_t) {
			strndup(rs+1, sep1-rs-1),
			strndup(sep1+1, sep2-sep1-1),
			strndup(sep2+1, end-sep2-1)
		};
		last_record = rs;
	}

	param.num_matches = n;
	param.results = results;
	return param;
}

void cedict_free(cedict_t param) {
	for(int i = 0; i < param.num_matches; ++i) {
		free(param.results[i].hanzi);
		free(param.results[i].pinyin);
		free(param.results[i].english);
	}
	free(param.results);
}



