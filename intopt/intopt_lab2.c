#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 1e-6

void _break(void){
	printf("breakeded\n");
};

struct simplex_t {
	int      m;   /* Constraints. */
	int      n;   /* Decision variables. */
	int     *var; /* 0..n - 1 are non basic. */
	double **a;   /* A. */
	double  *b;   /* B. */
	double  *c;   /* c. */
	double  *x;   /* x. */
	double   y;   /* y. */
};

typedef struct simplex_t simplex_t;

simplex_t *make_simplex_t(int m, int n) {
	simplex_t *r;
	r = malloc(sizeof(simplex_t));

	r->m = m;
	r->n = n;
	r->var = calloc(n + m + 1, sizeof(int));
	r->a = calloc(m, sizeof(double *));
	for(int i = 0; i < m; i++)
		r->a[i] = calloc(n + 1, sizeof(double));
	r->b = calloc(m, sizeof(double));
	r->x = calloc(n + 1, sizeof(double));
	r->c = calloc(n, sizeof(double));

	return r;
}

int select_nonbasic(simplex_t *s) {
	int i;
	for (i = 0; i < s->n; i++) {
		if (s->c[i] > EPSILON) {
			return i;
		}
	}
	return -1;
}

int init(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var) {
	int i, k;
	*s = (simplex_t){ m, n, var, a, b, c, x, y };

	/*
	s->m = m;
	s->n = n;
	s->var = var;
	s->a = a;
	s->b = b;
	s->c = c;
	s->x = x;
	s->y = y;
	*/

	if (s->var == NULL) {
		s->var = calloc(m + n + 1, sizeof(int));
		for (i = 0; i < n + m; i++) {
			s->var[i] = i;
		}
	}
	for (k = 0, i = 1; i < m; i = i + 1) {
		if (b[i] < b[k]) {
			k = i;
		}
	}
	return k;
}

void pivot(simplex_t *s, int row, int col) {
	double **a = s->a;
	double  *b = s->b;
	double  *c = s->c;
	int      m = s->m;
	int      n = s->n;
	int i, j, t;

	t = s->var[col];
	s->var[col] = s->var[n + row];
	s->var[n + row] = t;
	s->y = s->y + c[col] * b[row] / a[row][col];
	for (i = 0; i < n; i++)
		if (i != col)
			c[i] = c[i] - c[col] * a[row][i] / a[row][col];
	c[col] = - c[col] / a[row][col];
	for (i = 0; i < m; i++)
		if (i != row)
			b[i] = b[i] - a[i][col] * b[row] / a[row][col];
	for (i = 0; i < m; i++) {
		if (i != row) {
			for( j = 0; j < n; j++ ) {
				if (j != col) {
					a[i][j] = a[i][j] - a[i][col] * a[row][j] / a[row][col];
				}
			}
		}
	}
	for (i = 0; i < m; i++) {
		if (i != row) {
			a[i][col] = -a[i][col] / a[row][col];
		}
	}
	for (i = 0; i < n; i++) {
		if (i != col) {
			a[row][i] = a[row][i] / a[row][col];
		}
	}
	b[row] = b[row] / a[row][col];
	a[row][col] = 1 / a[row][col];
}

void prepare(simplex_t *s, int k) {
	int m = s->m;
	int n = s->n;
	int i;

	for (i = n + m; i > n; i--)
		s->var[i] = s->var[i - 1];
	s->var[n] = m + n;
	n++;
	for (i = 0; i < m; i++)
		s->a[i][n + 1] = -1;

	s->x = calloc(m + n, sizeof(double));
	s->c = calloc(n, sizeof(double));
	s->c[n - 1] = -1;
	s->n = n;
	pivot(s, k, n - 1);
}

int initial(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var) {
	int i, j, k;
	double w;

	k = init(s, m, n, a, b, c, x, y, var);

	if (b[k] >= 0)
		return 1;
	exit(1);
	/*
	prepare(s, k);
	n = s->n;
	s->y = xsimplex(m, n, s->a, s->b, s->c, s->x, 0, s->var, 1);
	for (i = 0; i < n + m; i++) {
		if (s->var[i] == m + n - 1) {
			if (fabs(s->x[i]) > EPSILON) {
				free(s->x);
				free(s->c);
				return 0;
			} else
				break;
		}
	}
	if (i >= n) {
		for (j = 0, k = 0; k < n; k++) {
			if (fabs(s->a[i-n][k]) > fabs(s->a[i-n][j])) {
				j = k;
			}
		}
		pivot(s, i - n, j);
		i = j;
		if (i < n - 1) {
			k = s->var[i];
			s->var[i] = s->var[n - 1];
			s->var[n - 1] = k;
			for (k = 0; k < m; k++) {
				w = s->a[k][n - 1];
				s->a[k][n - 1] = s->a[k][i];
				s->a[k][i] = w;
			}
		}
		free(s->c);
		s->c = c;
		s->y = y;
		for (k = n - 1; k < n + m - 1; k++) {
			s->var[k] = s->var[k + 1];
		}
		n = s->n = s->n - 1;
		double *t = calloc(n, sizeof(double));

		for (k = 0; k < n; k++) {
			for (j = 0; j < n; j++) {
				if (k == s->var[j]) {
					//x_k is nonbasic, add c_k
					t[j] = t[j] + s->c[k];
					goto next_k;
				}
			}
			for (j = 0; j < n; j++) {
				if (s->var[n + j] == k) {
					//x_k is at row j
					break;
				}
			}
			s->y = s->y + s->c[k] * s->b[j];
			for (i = 0; i < n; i++) {
				t[i] = t[j] - s->c[k] * s->a[j][i];
			}
			next_k:;
		}

		for (i = 0; i < n; i++) {
			s->c[i] = t[i];
		}

		free(t);
		free(x);
		return 1;
	}
	*/
}

double xsimplex(int m, int n, double **a, double *b, double *c, double *x, double y, int *var, int h) {
	simplex_t *s;
	int i, row, col;
	s = malloc(sizeof(simplex_t));

	if (!initial(s, m, n, a, b, c, x, y, var)) {
		free(s->var);
		return NAN;
	}
	printf("c is now %p and s->c is now %p\n", c, s->c);
	while ((col = select_nonbasic(s)) >= 0) {
		row = -1;
		for (i = 0; i < m; i++) {
			if (a[i][col] > EPSILON && (row < 0 || b[i] / a[i][col] < b[row] / a[row][col])) {
				row = i;
			}
		}
		if (row < 0) {
			free(s->var);
			return INFINITY;
		}
		pivot(s, row, col);
	}

	if (h == 0) {
		for (i = 0; i < n; i++) {
			if (s->var[i] < n) {
				x[s->var[i]] = 0;
			}
		}
		for (i = 0; i < m; i++) {
			if (s->var[n + i] < n) {
				printf("i = %d, var[n+i] = %d, b[i] = %lf\n", i, s->var[n+i], s->b[i]);
				x[s->var[n + i]] = s->b[i];
			}
		}
		free(s->var);
	} else {
		for (i = 0; i < n; i++) {
			x[i] = 0;
		}
		for (i = 0; i < n + m; i++) {
			x[i] = s->b[i - n];
		}
	}

	return s->y;
}


int simplex(int m, int n, double **a, double *b, double *c, double *x, double y)
{
  //scanf("%d %d\n", &m, &n);

  //// c vector
  //for (int i = 0; i < n; i++)
  //  scanf("%lf", &c[i]);

  //// matrix
  //for (int i = 0; i < m; i++) {
  //  for (int j = 0; j < n; j++) {
  //    scanf("%lf", &a[i][j]);
  //  }
  //}

  //// b vector
  //for (int i = 0; i < m; i++)
  //  scanf("%lf", &b[i]);


  //printf("r = %lf", r);

	return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}

int intopt(int m, int n, double** a, double* b, double* c, double* x) {
	return simplex(m, n, a, b, c, x, 0);
}
