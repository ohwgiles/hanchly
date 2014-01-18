//
// Copyright (c) 2014 Oliver Giles
// All rights reserved.
//
// This file is part of Hanchly (汉查利).
// Hanchly is free software; resdistribution and use are
// permitted under conditions stipulated in the file
// COPYING, which is distributed with Hanchly.
//

#include "unihan.h"
#include <stdint.h>

#define CHARS_IN_HANZI 4

extern char _binary_unihan_data_start[];
extern char _binary_unihan_idx_start[];
extern char _binary_unihan_idx_end[];
extern char _binary_char_freq_start[];
extern char _binary_char_freq_end[];

unihan_entry_t* unihan_find(const char* hanzi) {
	// first locate the hanzi in the frequency table
	uintptr_t hz_loc = (uintptr_t) memmem(
			_binary_char_freq_start,
			_binary_char_freq_end - _binary_char_freq_start,
			hanzi,
			CHARS_IN_HANZI);
	if(!hz_loc)
		return 0;

	// the offset into the frequency table is an index into the index table
	uintptr_t offset = (hz_loc - (uintptr_t) _binary_char_freq_start) / CHARS_IN_HANZI;

	int* index_table = (int*) _binary_unihan_idx_start;
	// the index table contains offsets into the data table
	int index = index_table[offset];

	// return the found index into the data table
	return &_binary_unihan_data_start[index];
}
