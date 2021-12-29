#include <stdio.h>

int main(void) {
	int a;
	int b;
	b = 1;
	a = b++;
	printf("Post: a=%d, b=%d\n", a, b);

	b = 1;
	a = ++b;
	printf("Pre: a=%d, b=%d\n", a, b);
}
