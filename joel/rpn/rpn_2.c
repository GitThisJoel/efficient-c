#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define N (10)
#define b break
void rte(char c){
	while(c != '\n')
		c = getchar();
}

int s[N];
int l = 1, end = 0;
int main(void)
{
	while(end == 0){
		int os = 0, cl = 0, n;
		char c;
		while (cl == 0) {
			c = getchar();
			if (isdigit(c)) {
				n = 0;
				while(isdigit(c))  {
					n = (n*10) + (c - '0');
					c = getchar();
				}
				if (os < N) {
					s[os] = n;
					os++;
				} else {
					printf("line %d: error at %d\n", l, n);
					rte(c);
					cl = 1;
				}
			}
			if (feof(stdin)) {
				end = 1;
				b;
			}
			if(os>=2 && (c=='+' || c=='-' || c=='*' || c=='/')){
				switch (c) {
				case '+':
					s[os-2] = s[os-2]+s[os-1];
					b;
				case '-':
					s[os-2] = s[os-2]-s[os-1];
					b;
				case '*':
					s[os-2] = s[os-2]*s[os-1];
					b;
				case '/':
					if (s[os-1] != 0)
						s[os-2] = s[os-2]/s[os-1];
					else {
						printf("line %d: error at %c\n", l, c);
						rte(c);
						cl = 1;
					}
					b;
				}
				os--;
			} else if (c=='+' || c=='-' || c=='*' || c=='/') {
				printf("line %d: error at %c\n", l, c);
				rte(c);
				cl = 1;
			} else {
				switch (c) {
				case '\n':
					if (os == 1)
						printf("line %d: %d\n", l, s[os-1]);
					else
						printf("line %d: error at \\n\n", l);
					cl = 1;
					b;
				case ' ':
					b;
				default:
					printf("line %d: error at %c\n", l, c);
					rte(c);
					cl = 1;
				}
			}
		}
		l++;
	}
	return 0;
}
