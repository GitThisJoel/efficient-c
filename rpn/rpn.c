#include <stdio.h>

#define N (10)

int main(void)
{
	int stack[N];
	int on_stack = 0;
	char c;
	int line_nbr = 1;

	while (c > -1) { // kinda works to detect EOF
		while(c != '\n') {

			// if c isdigit => add to stack
			// if c is operator => take the top two numbers on stack
			// if c is \n => print out number if only one on stack else error
			// if c is undefined => print error

			printf("%c", c);
			c = getchar();
		}
	c = getchar();
	}
	return 0;
}
