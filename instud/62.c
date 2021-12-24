#include <stdio.h>

#define ISDIGIT(c) ((c) >= '0' && (c) <= '9')
int main()
{
	int num; // value of number.
	int c; // a char.

  	num = 0;

	// printf("is \\n a digit, %d\n", ISDIGIT('\n'));

	c = getchar();
  	while (ISDIGIT(c)) {
	  	printf("char is %c, int of c is %d\n", c, c);
	  	num = 10 * num + (c - '0');
		c = getchar();
  	}

  	printf("the number is %d\n", num);

  	return 0;
}
