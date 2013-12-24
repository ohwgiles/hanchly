
#include "cedict.h"

#include "pinyin.h"
#include <string.h>

extern char _binary_cedict_start;
extern char _binary_cedict_end;

const char RECORD_SEPARATOR = 0x1e;
const char FIELD_SEPARATOR = 0x0;

cedict_t cedict_search(cedict_t param, const char* term) {
	size_t len = strlen(term);
	const char* p = &_binary_cedict_start;

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

	while(n < param.num_matches && p < &_binary_cedict_end) {
		p = (*search_fn)(p, term);
		if(!p) { puts("no match"); break; }
		char* rs = memrchr(&_binary_cedict_start, 0x1e, p-&_binary_cedict_start);
		char* ps = rawmemchr(rs+1,0x1f);
		char* es = rawmemchr(ps+1,0x1f);
		char* end = rawmemchr(es+1,0x1f);
		int is_exact = (p[-1] == 0x1f || p[-1] == '/') && (p[len] == 0x1f || p[len] == '/');
		if(is_exact) puts("EXACT:");
		//puts(strndupa(rs+1, ps-rs-1));
		//puts(strndupa(ps+1, es-ps-1));
		//puts(strndupa(es+1, end-es-1));
		p++;
		results[n++] = (cedict_result_t) {
			strndup(rs+1, ps-rs-1),
			strndup(ps+1, es-ps-1),
			strndup(es+1, end-es-1)
		};
	}

	param.num_matches = n;
	param.results = results;
	return param;
}
