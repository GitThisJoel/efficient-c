#include <stdio.h>

int main() {
	int a[5] = {1,2,3,4,5};
	int *p0 = &(a[0]);

	int i = 10;

	printf("%p\n", i-p0); // does not work
	return 0;
}
