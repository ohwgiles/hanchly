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
#include "unihan.h"
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

void usage(FILE* stream) {
	fprintf(stream,
		"Usage: hanchly-cli [options] search-term\n"
		"Hanchly 汉查利 Chinese Dictionary (Command Line)\n"
		"	-u, --unihan		Search for character in Unihan database\n"
		"	-p, --pinyin		Search CEDICT for pinyin\n"
		"	-h, --hanzi			Search CEDICT for hanzi\n"
		"	-e, --english		Search CEDICT for english\n"
		"	-n, --lines=N		Find the first N results (default 5)\n"
		"	-h, --help			Display this help message\n"
	);
}

int main(int argc, char** argv) {
	char* search = 0;

	// arbitrary consts not in HANCHLY_CEDICT
	const int SEARCH_GUESS = 100;
	const int SEARCH_UNIHAN = 101;

	int type = SEARCH_GUESS;

	cedict_t param;
	param.num_matches = 5;
	param.exact_only = 1;
	
	for(int i = 1; i < argc; ++i) {
		if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
			return usage(stdout), 0;
		else if(type == SEARCH_GUESS && (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--unihan") == 0))
			type = SEARCH_UNIHAN;
		else if(type == SEARCH_GUESS && (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--pinyin") == 0))
			type = HANCHLY_CEDICT_PINYIN;
		else if(type == SEARCH_GUESS && (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--hanzi") == 0))
			type = HANCHLY_CEDICT_HANZI;
		else if(type == SEARCH_GUESS && (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--english") == 0))
			type = HANCHLY_CEDICT_ENGLISH;
		else if(strncmp(argv[i], "-n", 2) == 0 && (
				(param.num_matches = atoi(&argv[i][2])) || 
				(i+1 < argc && (param.num_matches = atoi(argv[i+1])) && i++)
		))
			(void) 0;
		else if(strncmp(argv[i], "--lines=", 8) == 0 &&
			(param.num_matches = atoi(&argv[i][9])))
			(void) 0;
		else if(argv[i][0] == '-') {
			fprintf(stderr, "Unknown option %s\n", argv[i]);
			return usage(stderr), 1;
		} else if(search) {
			fprintf(stderr, "Invalid arguments\n");
			return usage(stderr), 1;
		} else
			search = argv[i];
	}

	if(!search) {
		fprintf(stderr, "No search term supplied\n");
		return usage(stderr), 1;
	}

	char* actual_search = search;
	if(type == SEARCH_GUESS) {
		if(isascii(search[0])) {
			char* py;
			if(py = pinyin_compose(search)) {
				param.type = HANCHLY_CEDICT_PINYIN;
				actual_search = py;
			} else
				param.type = HANCHLY_CEDICT_ENGLISH;
		} else {
			if(strlen(search) == 3)
				type = SEARCH_UNIHAN;
			else
				param.type = HANCHLY_CEDICT_HANZI;
		}
		fprintf(stderr, "Warning: no search type specified, inferring %s from search term\n",
			type == SEARCH_UNIHAN ? "unihan" :
			param.type == HANCHLY_CEDICT_PINYIN ? "pinyin" :
			param.type == HANCHLY_CEDICT_HANZI ? "hanzi" :
			param.type == HANCHLY_CEDICT_ENGLISH ? "english" : "");
	} else if(type == HANCHLY_CEDICT_PINYIN) {
		actual_search = pinyin_compose(search);
		if(actual_search == 0) {
			return fprintf(stderr, "%s is not recognised pinyin\n"), 1;
		}
	}

	if(type == SEARCH_UNIHAN) {
		// do a unihan search
		const unihan_entry_t* record = unihan_find(search);
		printf("Frequency\t%d\nTotal Strokes\t%d\nHSK Grade\t%d\nRadical Number\t%d\nExtra Strokes\t%d\nCangjie Code\t%s\nDefinition\t%s\n", record->frequency, record->total_strokes, record->hsk_grade, record->radical, record->additional_strokes, record->cangjie, record->definition);

	} else {
		// do a CEDICT search
		param.type = type;
		int requested_results = param.num_matches;
		char* old_locale = setlocale(LC_CTYPE, "en");
		param = cedict_search(param, actual_search);

		for(int i = 0; i < param.num_matches; ++i) {
			printf("%s\t%s\t%s\n", param.results[i].hanzi, param.results[i].pinyin, param.results[i].english);
		}

		// if there weren't enough matches, try an inexact search
		if(param.num_matches < requested_results) {
			puts("searching inexact");
			param.num_matches = requested_results - param.num_matches;
			param.exact_only = 0;
			param = cedict_search(param, actual_search);

			for(int i = 0; i < param.num_matches; ++i) {
				printf("%s\t%s\t%s\n", param.results[i].hanzi, param.results[i].pinyin, param.results[i].english);
			}
		}

		setlocale(LC_CTYPE, old_locale);

		if(actual_search != search)
			free(actual_search);

		cedict_free(param);
	}

	return 0;
}
