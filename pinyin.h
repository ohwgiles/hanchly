#ifndef _HANCHLY_PINYIN_H_
#define _HANCHLY_PINYIN_H_

#ifdef __cplusplus
extern "C" {
#endif

char* pinyin_compose(const char* s);

char* pinyin_search(const char* haystack, const char* needle);

#ifdef __cplusplus
}
#endif

#endif // _HANCHLY_PINYIN_H_
