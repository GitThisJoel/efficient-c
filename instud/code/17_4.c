#include <stdio.h>

int main() {
	signed int a = -128;
	signed int b = -2;
	a = a >> 4;
	b >>= 4;
	printf("%d\n", b);

	return 0;
}
