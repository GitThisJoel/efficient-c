#include <stdio.h>

int a[10] = {0,1,2,3,4,5,6,7,8,9};
int b[10] = {10,11,12,13,14,15,16,17,18,19};
int* p = a;
int* q = b;
void f()
{
	printf("before: first elem in p %d\n", p[0]);
	p = q;
	printf("after: first elem in p %d\n", p[0]);
}

int main() {
	f();
	return 0;
}
