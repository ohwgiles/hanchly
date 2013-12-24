#include "cedict.h"
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

void usage(FILE* stream) {
	fprintf(stream,
		"Usage: hanchly-cli [options] search-term\n"
		"Hanchly 汉查利 Chinese Dictionary (Command Line)\n"
		"	-p, --pinyin		Search as pinyin\n"
		"	-h, --hanzi			Search as hanzi\n"
		"	-e, --english		Search as english\n"
		"	-n, --lines=N		Find the first N results (default 5)\n"
		"	-h, --help			Display this help message\n"
	);
}

int main(int argc, char** argv) {
	char* search = 0;

	cedict_t param;
	param.type = -1;
	param.num_matches = 5;

	for(int i = 1; i < argc; ++i) {
		if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
			return usage(stdout), 0;
		else if(strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--pinyin") == 0)
			param.type = HANCHLY_CEDICT_PINYIN;
		else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--hanzi") == 0)
			param.type = HANCHLY_CEDICT_HANZI;
		else if(strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--english") == 0)
			param.type = HANCHLY_CEDICT_ENGLISH;
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
			fprintf(stderr, "Multiple search term or unknown options\n");
			return usage(stderr), 1;
		} else
			search = argv[i];
	}

	if(!search) {
		fprintf(stderr, "No search term supplied\n");
		return usage(stderr), 1;
	}

	char* actual_search = search;
	if(param.type == -1) {
		if(isascii(search[0])) {
			char* py;
			if(py = pinyin_compose(search)) {
				param.type = HANCHLY_CEDICT_PINYIN;
				actual_search = py;
			} else
				param.type = HANCHLY_CEDICT_ENGLISH;
		} else
			param.type = HANCHLY_CEDICT_HANZI;
		fprintf(stderr, "Warning: no search type specified, inferring %s from search term\n",
			param.type == HANCHLY_CEDICT_PINYIN ? "pinyin" :
			param.type == HANCHLY_CEDICT_HANZI ? "hanzi" :
			param.type == HANCHLY_CEDICT_ENGLISH ? "english" : "");
	} else if(param.type == HANCHLY_CEDICT_PINYIN) {
		actual_search = pinyin_compose(search);
		if(actual_search == 0) {
			return fprintf(stderr, "%s is not recognised pinyin\n"), 1;
		}
	}

	char* old_locale = setlocale(LC_CTYPE, "en");
	param = cedict_search(param, actual_search);
	setlocale(LC_CTYPE, old_locale);

	if(actual_search != search)
		free(actual_search);

	for(int i = 0; i < param.num_matches; ++i) {
		printf("%s\t%s\t%s\n", param.results[i].hanzi, param.results[i].pinyin, param.results[i].english);
	}
	cedict_free(param);

	return 0;
}
