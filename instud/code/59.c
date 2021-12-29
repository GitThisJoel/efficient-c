#include <stdio.h>
#include <string.h>

// int a[3] = {1,2,3};
// int b[3] = {4,5,6};

int a[3] = {1,2,3};
int b[3];

void f()
{
  // a = b;
  memcpy(b, a, 3*sizeof(int));
}

int main() {
	f();
	printf("a = %d %d %d\n", a[0], a[1], a[2]);
	printf("b = %d %d %d\n", b[0], b[1], b[2]);
	return 0;
}
