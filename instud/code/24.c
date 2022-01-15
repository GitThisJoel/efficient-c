#include <stdio.h>

typedef struct {
	double x;
	double y;
} point_t;

int main() {
	point_t p1, p2;

	p1 = (point_t) {1.23, 4.56};
	p2 = (point_t) {.y = 4.56, .x = 1.23};

	printf("p1: x = %lf and y = %lf\n", p1.x, p1.y);
	printf("p2: x = %lf and y = %lf\n", p2.x, p2.y);
	return 0;
}
