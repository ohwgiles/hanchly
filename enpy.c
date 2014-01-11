//
// Copyright (c) 2014 Oliver Giles
// All rights reserved.
//
// This file is part of Hanchly (汉查利).
// Hanchly is free software; resdistribution and use are
// permitted under conditions stipulated in the file
// COPYING, which is distributed with Hanchly.
//

#include <string.h>

#include "enpy.h"

extern char _binary_english_pinyin_start;
extern char _binary_english_pinyin_end;

int enpy(const char* input) {
	for(char* p = &_binary_english_pinyin_start; p < &_binary_english_pinyin_end; p = rawmemchr(p,'\0')+1)
		if(strcmp(p, input) == 0) return 1;
	return 0;
}

