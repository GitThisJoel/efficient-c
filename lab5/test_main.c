#include <stdio.h>
#include <stdlib.h>

#include "intopt.h"

//task 9
double** make_matrix_2(int m, int n) {
	double** a;
	a = calloc(m, sizeof(double*)); // obs '*'
	// a = calloc(m, sizeof(double*)-5);
	for (int i = 0; i < m; i++)
		a[i] = calloc(n, sizeof(double));
	return a;
}

void print_max(double* c, int n) {
	printf("max z = %10.3lfx_0", c[0]);
	for (int i = 1; i < n; i++)
		printf(" + %10.3lfx_%d", c[i], i);
	printf("\n");
}

void print_constaints(double** a, double* b, int m, int n) {
	for (int i = 0; i < m; i++) {
		printf("%10.3lfx_0", a[i][0]);
		for (int j = 1; j < n; j++) {
			printf(" + %10.3lfx_%d", a[i][j], j);
		}
		printf(" \u2264 %10.3lf\n", b[i]);
	}
}

void print_matrix(double** a, int m, int n){
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			printf("max z = %10.3lf ", a[i][j]);
		}
		printf("\n");
	}
}

void free_matrix_2(double** a, int m) {
	for (int i = 0; i < m; i++){
		free(a[i]);
	}
	free(a);
}

int main(void)
{
	int m;
	int n;
	double** a;
	double* c;
	double* b;
	double* x;

	scanf("%d %d\n", &m, &n);

	// c vector
	c = calloc(n, sizeof(double));
	for (int i = 0; i < n; i++)
		scanf("%lf", &c[i]);

	// a matrix
	a = make_matrix_2(m, n);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%lf", &a[i][j]);
		}
	}

	// b vector
	b = calloc(m, sizeof(double));
	for (int i = 0; i < m; i++)
		scanf("%lf", &b[i]);

	x = calloc(n+1, sizeof(double));
	// task 10
	print_max(c,n);
	print_constaints(a,b,m,n);

	// test run intopt

	printf("We got this: %lf\n", intopt(m, n, a, b, c, x));

	free(x);

	free_matrix_2(a,m);
	free(b);
	free(c);
}
