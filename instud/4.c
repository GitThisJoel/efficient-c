#include <stdio.h>

struct {
	int a : 1;
} s;
int main(void) {
	printf("size of s: %ld", sizeof(s));
}
