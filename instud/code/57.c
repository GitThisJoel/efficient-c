#include <stdio.h>
int main()
{
	char* s = "hello";
	printf("%d\n", s[0]);
	char t[] = "hello";
	s[0] = 'H';
	t[0] = 'H';

	return 0;
}
