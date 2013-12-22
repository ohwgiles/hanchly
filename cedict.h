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

struct cedict_result_s;
typedef struct cedict_result_s cedict_result_t;

typedef struct {
	cedict_search_t type;
	void* search_from;
	int num_match;
	cedict_result_t* results;
} cedict_t;

cedict_t cedict_search(cedict_t param, const char* term);

#ifdef __cplusplus
}
#endif


#endif // _HANCHLY_CEDICT_H_
