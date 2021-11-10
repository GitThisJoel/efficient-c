#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define N (10)

void read_to_end(char c) {
	while(c != '\n')
		c = getchar();
}

int stack[N];
int calc_line(int line_nbr) {
	int on_stack = 0;
	char c;
	char prev_c;
	int n;
	while(1) {

		// if c isdigit => add to stack
		// if c is operator => take the top two numbers on stack
		// if c is \n => print out number if only one on stack else error
		// if c is undefined => print error

		c = getchar();
		if (isdigit(c)) {
			n = 0;
			while(isdigit(c))  {
				n = (n*10) + (c - '0');
				c = getchar();
			}
			if (on_stack < N) {
				stack[on_stack] = n;
				on_stack++;
			} else {
				printf("line %d: error at %d\n", line_nbr, n);
				read_to_end(c);
				return 1;
			}
		}

		if (c == EOF) { // Will this terminate at EOF?
			return -1;
		} else if (c == '\n'){
			if (on_stack == 1)
				printf("Line %d: %d\n", line_nbr, stack[on_stack-1]);
			else
				printf("Line %d: error at \\n\n", line_nbr);
			return 1;
		} else {
			switch (c) {
				case '+':
					if (on_stack>=2) {
						stack[on_stack-2] = stack[on_stack-2]+stack[on_stack-1];
						on_stack--;
					} else {
						printf("line %d: error at %c\n", line_nbr, c);
						read_to_end(c);
						return 1;
					}
					break;
				case '-':
					if (on_stack>=2) {
						stack[on_stack-2] = stack[on_stack-2]-stack[on_stack-1];
						on_stack--;
					} else {
						printf("line %d: error at %c\n", line_nbr, c);
						read_to_end(c);
						return 1;
					}
					break;
				case '*':
					if (on_stack>=2) {
						stack[on_stack-2] = stack[on_stack-2]*stack[on_stack-1];
						on_stack--;
					} else {
						printf("line %d: error at %c\n", line_nbr, c);
						read_to_end(c);
						return 1;
					}
					break;
				case '/':
					if (on_stack>=2) {
						if (stack[on_stack-1] != 0) {
							stack[on_stack-2] = stack[on_stack-2]/stack[on_stack-1];
							on_stack--;
						} else {
							printf("line %d: error at %c\n", line_nbr, c);
							read_to_end(c);
							return 1;
						}
					} else {
						printf("line %d: error at %c\n", line_nbr, c);
						read_to_end(c);
						return 1;
					}
					break;
				case ' ':
					break;
				default:
					printf("line %d: error at %c\n", line_nbr, c);
					read_to_end(c);
					return 1;
			}
		}
		prev_c = c;
	//for(int i = 0; i < on_stack; i++)
      	//	printf(" %d", stack[i]);
	//printf("\n");
	}


}

int main(void)
{
	int line_nbr = 1;
	while(calc_line(line_nbr)>0)
		line_nbr++;
	return 0;
}
