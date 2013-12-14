#include <stdio.h>
#include <string.h>

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
