//
// Copyright (c) 2014 Oliver Giles
// All rights reserved.
//
// This file is part of Hanchly (汉查利).
// Hanchly is free software; resdistribution and use are
// permitted under conditions stipulated in the file
// COPYING, which is distributed with Hanchly.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <alloca.h>

extern const char _binary_pinyin_syllables_start[];
extern const char _binary_pinyin_syllables_end[];

char* pinyin_compose(const char* input) {
	while(*input == ' ' || *input == '\'') input++;
	for(const char* p = _binary_pinyin_syllables_start; p < _binary_pinyin_syllables_end;) {
		int l = strlen(p);
		if(strncmp(p,input,l) == 0) {
			char* s = 0;
			char tone = '0';
			const char* next = &input[l];
			if(input[l] >= '1' && input[l] <= '5') {
				next++;
				tone = input[l];
			}

			if(*next == 0) {
				s = malloc(l+2);
				sprintf(s, "%s%c", p, tone);
				return s;
			}

			char* t = pinyin_compose(next);
			if(t) {
				if(asprintf(&s, "%s%c %s", p, tone, t) < 0)
					s = 0;
				free(t);
				return s;
			}
		}
		p = &p[l+1];
	}
	return 0;
}

char* pinyin_search(const char* haystack, const char* _needle) {
	// we need to modify this string locally.
	char* needle = strdupa(_needle);
	// replace all the zero characters in the search string
	// with real nulls. This will cause strcasestr to terminate
	// early, so we can check for tone wildcard matching
	char*end = strchrnul(needle, '0');
	while(*end) {
		*end = 0;
		end = strchrnul(end+1, '0');
	}
	// the end of the string is saved so we know when to
	// stop looking for wildcards

	// loop through the haystack checking for the truncated needle
	// we don't actually modify the string. However, to conform with
	// strstr and strcasestr, we must return a non-const char*
	char* candidate = (char*) haystack;
	while((candidate = strcasestr(candidate, needle))) {
		// if there are no wildcards in this string, the while loop
		// will not exectue and we will return immediately
		char* last_wildcard = rawmemchr(needle, '\0');
		while(last_wildcard < end) {
			// if there are wildcards, we loop through them with
			// rawmemchar( , '\0') until it finds the end of the
			// string we saved earlier
			char* next_wildcard = rawmemchr(last_wildcard+1, '\0');
			// test for matching wildcard and matching substring until
			// the next wildcard/end of string
			if(candidate[last_wildcard-needle] < '1' ||
				candidate[last_wildcard-needle] > '5' ||
				strncasecmp(&candidate[last_wildcard-needle+1], &last_wildcard[1], next_wildcard-last_wildcard-1)
			)
				goto no_match;
			last_wildcard = next_wildcard;
		}
		return candidate;
no_match:
		// advance the search by the length of the needle
		candidate += end-needle;
	}
	return 0;
}

