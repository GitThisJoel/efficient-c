#include <stdio.h>

struct { int a; } s, t;
void f()
{
	t.a = 1;
	printf("addresses before s = %p, t = %p\n", &s, &t);
	s = t;
	printf("addresses after s = %p, t = %p\n", &s, &t);
}
int main() {
	f();
	printf("now s.a is %d\n", s.a);
	return 0;
}
