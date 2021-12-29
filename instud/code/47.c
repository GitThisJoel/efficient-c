#include <stdio.h>

void f(int* a) {
	*a = 10;
}

void g() {
	int a[5] = {1,2,3,4,5};
	double b[5] = {1.0,2.0,3.0,4.0,5.0};
	int *p0 = &(a[0]);
	int *p1 = &(a[1]);

	double *dp0 = &(b[0]);
	//&(a[1])-&(a[0])
	printf("%p\n%p\n", &(a[0]), p1-1);

	printf("%lf\n", *dp0);
	//printf("%ld\n", p0-dp0); <- does not work

	// f(&a);
}

int main() {
	g();
	return 0;
}
