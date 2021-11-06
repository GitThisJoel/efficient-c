#include <stdbool.h>
#include <stdio.h>
int f() {
  bool p = 1;
  return p + 1;
}
int main(void) {
	printf("%d\n",f());
	printf("%ld\n", sizeof(bool));
	if (0) {
		printf("%d\n",0);
	}
	if (1) {
		printf("%d\n",1);
	}
	if (2) {
		printf("%d\n",2);
	}
}
