#include <stdlib.h>
#include <stdio.h>


void f(size_t n)
{
	int* p = calloc(n, sizeof(int));
	int* q = p + 1;

	printf("&p = %p, &q = %p\n", p, q);

	p = realloc(p, 2 * n * sizeof(int));
	if (p == NULL)
		return;

	printf("&p = %p, &q = %p\n", p, q);

	q[0] = 1;
	p[n] += 2;
	printf("&p[n] = %p, &q[0] = %p\n", &p[n], &q[0]);

	free(p);
}

void main() {
	f(10);
}
