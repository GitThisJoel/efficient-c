#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
int main(void)
{
	int m = 0, len = 0;
	char c = getchar();
	char *w = malloc(0);
	char *v = malloc(0);
	while(!feof(stdin)) {
		if (isalpha(c)>0) {
			len++;
			v = realloc(v, len*sizeof(char));
			v[len-1] = c;
		} else {
			if (len > m) {
				m = len;
				w = realloc(w, m*sizeof(char));
				strcpy(w, v);
			}
			len = 0;
			free(v);
			v = malloc(0*sizeof(char));
		}
		c = getchar();
	}
	printf("%d characters in longest word: %s\n", m, w);
	free(w);
	free(v);
	return 0;
}
