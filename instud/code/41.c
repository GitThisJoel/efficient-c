#include <stdio.h>

struct foo {int a; char b[2];} s;

typedef struct {
	double x;
	double y;
} point_t;

int main()
{
	int x = 1, y = 2;
	// The 97 in the compund literal is the last element in b
	s = ((struct foo) {x + y, 'a', 97});
	printf("s.a = %d, s.b = %s\n", s.a, s.b);

	struct foo s2;
	s2.a = 10;
	s2.b[0] = 'c';
	s2.b[1] = 'd';
	printf("s2.a = %d, s2.b = %s\n", s2.a, s2.b);

	point_t p1 = (point_t) { .y = 1.5, .x = 2.4 };
	printf("in p1, x = %lf, y = %lf", p1.x, p1.y);
	// print(&(point_t) {.x = 1.2, .y = 6.7})
	return 1;
}
