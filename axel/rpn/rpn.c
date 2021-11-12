#include <ctype.h>
#include <stdio.h>
#define N (10)

int s[N], sp, l = 1;
char c = '\n';
void k()
{
	printf("line %d: ", l++);
	sp = 0;
	s[0] = 0;
}
void e()
{
	if (c == '\n')
		printf("error at \\n\n");
	else
		printf("error at %c\n", c);
	while (c != '\n' && !feof(stdin))
		c = getchar();
}
int main(void)
{
	int i;
	for (;;) {
		i = 0;
		if (c == '\n')
			i += 2;
		if (isdigit(c))
			i += 1;

		c = getchar();
		if (feof(stdin))
			break;
		if (i & 2)
			k();

		switch (c) {
		case '+':
		case '-':
		case '*':
		case '/':
			if (i & 1) sp++;
			if (sp < 2)
				e();
			if (c == '+') s[sp - 2] += s[sp - 1];
			if (c == '-') s[sp - 2] -= s[sp - 1];
			if (c == '*') s[sp - 2] *= s[sp - 1];
			if (c == '/') {
				if (s[sp - 1] == 0) {
					e();
					break;
				} else s[sp - 2] /= s[sp - 1];
			}
			s[sp] = 0;
			sp--;
			break;
		case ' ':
			if (i & 1) sp++;
			s[sp] = 0;
			break;
		case '\n':
			if (i & 1) sp++;
			if (sp != 1)
				e();
			else
				printf("%d\n", s[0]);
			break;
		default:
			if (isdigit(c) && sp < N) {
				s[sp] = s[sp] * 10 + (c - '0');
			} else
				e();
		}
	}

	return 0;
}
