#include <stdio.h>
#include <ctype.h>
#define N (10)
#define b break
#define g getchar()
int s[N], l = 1, o = 0, n, t, c;
int main() {
	for(;;) {
		c = g;
		if (isdigit(c)) {
			n = 0;
			while(isdigit(c))  {
				n = (n*10) + (c - '0');
				c = g;
			}
			if (o == N) {
				c = n + '0';
				goto e;
			}
			s[o++] = n;
		}
		if (c == EOF)
			b;
		if (o>1 && (c=='+' || c=='-' || c=='*' || c=='/')) {
			n = s[o-2]; t = s[o-1];
			switch (c) {
			case '+':
				n += t; b;
			case '-':
				n -= t; b;
			case '*':
				n *= t; b;
			case '/':
				if(!t)
					goto e;
				n /= t;
			}
			s[--o-1] = n;
		} else {
			if (!(c-' '))
				continue;
			switch (c) {
			case '\n':
				if (o == 1)
					printf("line %d: %d\n", l, s[o-1]);
				else
					printf("line %d: error at \\n\n", l);
				b;
			default:
e:
				printf("line %d: error at %c\n", l, c);
				while(c != '\n')
					c = g;
			}
			o = 0; l++;
		}
	}
}
