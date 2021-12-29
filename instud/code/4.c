#include <stdio.h>

struct {
	int a : 1;
	int b : 31;
	int c : 1;
} s;
int main(void) {
	printf("size of s: %ld\n", sizeof(s));
	printf("size of int: %ld", sizeof(int));
}
