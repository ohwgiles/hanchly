#include <stdio.h>
#include <string.h>
#include <alloca.h>

extern char _binary_pinyin_syllables_start;
extern char _binary_pinyin_syllables_end;

char* pinyin_compose(const char* input) {
	while(*input == ' ' || *input == '\'') input++;
	for(char* p = &_binary_pinyin_syllables_start; p < &_binary_pinyin_syllables_end;) {
		int l = strlen(p);
		if(strncmp(p,input,l) == 0) {
			char* s = 0;
			char tone = '0';
			char* next = &input[l];
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
				asprintf(&s, "%s%c %s", p, tone, t);
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
	char*z = strchrnul(needle, '0');
	while(*z) {
		*z = 0;
		z = strchrnul(z, '0');
	}
	// z is saved as the end of the string so we know when to
	// stop looking for wildcards

	// loop through the haystack checking for the truncated needle
	char* p = haystack;
	while(p = strcasestr(p, needle)) {
		char* q = rawmemchr(needle, '\0');
		while(q < z) {
			if(!(p[q-needle] >= '1' && p[q-needle] <= '5' && strcasecmp(&p[q-needle+1], &q[1]) == 0))
				goto no_match;
			q = rawmemchr(q+1, '\0');
		}
		return p;
no_match:
		(void) 0;
	}
	return 0;
}

