#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 10e-6


struct simplex_t {
	int      m;   /* Constraints. */
	int      n;   /* Decision variables. */
	int     *var; /* 0..n - 1 are non basic. */
	double **a;   /* A. */
	double  *b;   /* B. */
	double  *x;   /* x. */
	double  *c;   /* c. */
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

double** make_matrix(int m, int n) {
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

void free_matrix(double** a, int m) {
  for (int i = 0; i < m; i++){
    free(a[i]);
  }
  free(a);
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

double xsimplex(int m, int n, double **a, double *b, double *c, double *x, double y, int *var, int h) {
	simplex_t s;
	int i, row, col;

	if (!init(&s, m, n, a, b, c, x, y, var)) {
		free(s->var);
		return NAN;
	}
	while ((col = select_nonbasic(&s)) >= 0) {
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
		pivot(&s, row, col);
	}
	if (h == 0) {
		for (i = 0; i < n; i++) {
			if (s->var[i] < n) {
				x[s->var[i]] = 0;
			}
		}
		for (i = 0; i < m; i++) {
			if (s->var[n + i] < n) {
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

int main(void)
{
  int m;
  int n;
  double** a;
  double* c;
  double* b;

  scanf("%d %d\n", &m, &n);

  // c vector
  c = calloc(n, sizeof(double));
  for (int i = 0; i < n; i++)
    scanf("%lf", &c[i]);

  // matrix
  a = make_matrix(m, n);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      scanf("%lf", &a[i][j]);
    }
  }

  // b vector
  b = calloc(m, sizeof(double));
  for (int i = 0; i < m; i++)
    scanf("%lf", &b[i]);

  double r = xsimplex(m, n, a, b, c, x, y, null, 0);

  printf("r = %lf", r);

  free_matrix(a,m);
  free(b);
  free(c);
}
