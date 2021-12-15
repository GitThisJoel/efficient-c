#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define EPSILON 1e-6

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

struct node_t {
	int m;        /* Constraints. */
	int n;        /* Decision variables. */
	int k;        /* Parent branches on x_k */
	int h;        /* Branch in x_h */
	double   xh;  /* x_h */
	double   ak;  /* Parent a_k. */
	double   bk;  /* Parent b_k. */
	double  *min; /* Lower bounds. */
	double  *max; /* Upper bounds */
	double **a;   /* A. */
	double  *b;   /* b. */
	double  *x;   /* x. */
	double  *c;   /* c. */
	double   z;   /* z. */
};

struct set_t {
	struct list_t *first;
};

struct list_t {
	struct list_t *next;
	struct node_t *elem;
};

typedef struct node_t node_t;
typedef struct list_t list_t;
typedef struct set_t set_t;
typedef struct simplex_t simplex_t;

node_t *set_pop(set_t *set) {
	list_t *pop = set->first;
	set->first = pop->next;
	node_t *p = pop->elem;
	free(pop);

	return p;
}

void set_add(set_t *set, node_t *p) {
	list_t *o = calloc(1, sizeof(list_t));
	o->elem = p;
	if (set->first != NULL)
		o->next = set->first;
	set->first = o;
}

void free_set_t(set_t *set) {
	if (set->first != NULL) {
		list_t *l = set->first;
		list_t *next;
		while (l != NULL) {
			next = set->first->next;
			free(l);
			l = next;
		}
	}
	free(set);
}

int set_size(set_t *set) {
	if (set->first == NULL)
		return 0;
	list_t *l = set->first;
	int i = 0;

	while (l != NULL) {
		i++;
		l = l->next;
	}
	return i;
}

void free_node_t(node_t *p);

void set_rm_below_z(set_t *set, double z) {
	list_t *p, *prev, *next;
	if (set->first == NULL)
		return;

	while (set->first->elem->z < z) {
		p = set->first;
		set->first = p->next;
		//free_node_t(p->elem);
		//free(p);
	}

	prev = set->first;
	p = prev->next;
	while (p != NULL) {
		next = p->next;
		if (p->elem->z < z) {
			prev->next = p->next;
			// free_node_t(p->elem);
			// free(p);
		} else {
			prev = p;
		}
		p = next;
	}
}

void free_node_t(node_t *p) {
	free(p->min);
	free(p->max);

	for(int i = 0; i < p->m + 1; i++) {
		free(p->a[i]);
	}
	free(p->a);
	free(p->b);
	free(p->x);
	free(p->c);
	free(p);
}

double xsimplex(int m, int n, double **a, double *b, double *c, double *x, double y, int *var, int h);

int initial(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var);

int init(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var);

double simplex(int m, int n, double **a, double *b, double *c, double *x, double y);

node_t *initial_node(int m, int n, double **a, double *b, double *c) {
	node_t *p = calloc(1, sizeof(node_t));
	int i, j;

	p->a = calloc(m + 1, sizeof(double *));
	for (i = 0; i < m + 1; i++) {
		p->a[i] = calloc(n + 1, sizeof(double));
	}
	for (i = 0; i < m; i++) {
		for(j = 0; j < n; j++) {
			p->a[i][j] = a[i][j];
		}
	}

	p->b = calloc(m + 1, sizeof(double));
	p->c = calloc(n + 1, sizeof(double));
	p->x = calloc(n + 1, sizeof(double));

	p->min = calloc(n, sizeof(double));
	p->max = calloc(n, sizeof(double));
	p->m = m;
	p->n = n;

	memcpy(p->b, b, sizeof(double) * m);
	memcpy(p->c, c, sizeof(double) * n);

	for (i = 0; i < n; i++) {
		p->min[i] = -INFINITY;
		p->max[i] =  INFINITY;
	}
	return p;
}

node_t *extend(node_t *p, int m, int n, double **a, double *b, double *c, int k,
	       double ak, double bk) {
	int i, j, y;
	node_t *q = calloc(1, sizeof(node_t));

	q->k  = k;
	q->ak = ak;
	q->bk = bk;

	if (ak > 0 && p->max[k] < INFINITY) {
		q->m = p->m;
	} else if (ak < 0 && p->min[k] > 0) {
		q->m = p->m;
	} else
		q->m = p->m + 1;

	q->n = p->n;
	q->h = -1;
	q->a = calloc(q->m + 1, sizeof(double *));
	for (i = 0; i < q->m + 1; i++) {
		q->a[i] = calloc(q->n + 1, sizeof(double));
	}
	q->b = calloc(q->m + 1, sizeof(double));
	q->c = calloc(q->n + 1, sizeof(double));
	q->x = calloc(q->n + 1, sizeof(double));
	q->min = calloc(n, sizeof(double));
	q->max = calloc(n, sizeof(double));

	for (i = 0; i < p->n; i++) {
		q->min[i] = p->min[i];
		q->max[i] = p->max[i];
	}

	for (i = 0; i < m; i += 1) {
		for (y = 0; y < n; y += 1) {
			q->a[i][y] = a[i][y];
		}
		q->b[i] = b[i];
	}

	for (i = 0; i < n; i += 1) {
		q->c[i] = c[i];
	}

	if (ak > 0) {
		if (q->max[k] == INFINITY || bk < q->max[k]) {
			q->max[k] = bk;
		}
	} else if (q->min[k] == -INFINITY || -bk > q->min[k]){
		q->min[k] = -bk;
	}
	for (i = m, j = 0; j < n; j++) {
		if (q->min[j] > -INFINITY) {
			q->a[i][j] = -1;
			q->b[i] = -q->min[j];
			i++;
		}
		if (q->max[j] < INFINITY) {
			q->a[i][j] = 1;
			q->b[i] = q->max[j];
			i++;
		}
	}

	return q;
}

int is_integer(double *xp) {
	double x = *xp;

	double r = lround(x);

	if (fabs(r - x) < EPSILON) {
		*xp = r;
		return 1;
	} else
		return 0;
}

int integer(node_t *p) {
	for (int i = 0; i < p->n; i++) {
		if(!is_integer(&p->x[i]))
			return 0;
	}
	return 1;
}

void bound(node_t *p, set_t *h, double *zp, double *x) {
	if (p == NULL)
		return;
	if (p->z > *zp) {
		*zp = p->z;
		memcpy(x, p->x, sizeof(double) * p->n);
		/* remove and delete all nodes q in h with q.z < p.z */
		set_rm_below_z(h, p->z);
	}
}

int branch(node_t *q, double z) {
	double min, max;
	if (q->z < z)
		return 0;
	for (int h = 0; h < q->n; h++) {
		if (!is_integer(&q->x[h])) {
			if (q->min[h] == -INFINITY)
				min = 0;
			else
				min = q->min[h];
			max = q->max[h];
			if(floor(q->x[h]) < min || ceil(q->x[h]) > max)
				continue;
			q->h = h;
			q->xh = q->x[h];
			// for (int i = 0; i < q->m + 1; i++)
			// 	free(q->a[i]);
			// free(q->a);
			// free(q->b);
			// free(q->c);
			// free(q->x);
			return 1;
		}
	}
	return 0;
}

void succ(node_t *p, set_t *h, int m, int n, double **a, double *b, double *c, int k, double ak, double bk, double *zp, double *x) {
	node_t *q = extend(p, m, n, a, b, c, k, ak, bk);

	if (q == NULL)
		return;

	q->z = simplex(q->m, q->n, q->a, q->b, q->c, q->x, 0);

	if (isfinite(q->z)) {
		if (integer(q)) {
			bound(q, h, zp, x);
		} else if(branch(q, *zp)) {
			set_add(h, q);
			return;
		}
	}
	//free_node_t(q);
}

int select_nonbasic(simplex_t *s) {
	int i;
	for (i = 0; i < s->n; i++) {
		if (s->c[i] > EPSILON)
			return i;
	}
	return -1;
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

	// move all basic variables one steep to the right
	for (i = n + m; i > n; i--)
		s->var[i] = s->var[i - 1];

	// add non-basic x_m+n
	s->var[n] = m + n;
	n++;
	for (i = 0; i < m; i++)
		s->a[i][n - 1] = -1;

	// give new non-basic coefficient -1 in all rows
	s->x = calloc(m + n, sizeof(double));
	s->c = calloc(n, sizeof(double));
	s->c[n - 1] = -1;
	s->n = n;
	pivot(s, k, n - 1);
}

double xsimplex(int m, int n, double **a, double *b, double *c, double *x, double y, int *var, int h) {
	simplex_t s;
	int i, row, col;

	if (!initial(&s, m, n, a, b, c, x, y, var)) {
		free(s.var);
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
			free(s.var);
			return INFINITY;
		}
		pivot(&s, row, col);
	}

	if (h == 0) {
		for (i = 0; i < n; i++) {
			if (s.var[i] < n) {
				x[s.var[i]] = 0;
			}
		}
		for (i = 0; i < m; i++) {
			if (s.var[n + i] < n) {
				x[s.var[n + i]] = s.b[i];
			}
		}
		free(s.var);
	} else {
		for (i = 0; i < n; i++) {
			x[i] = 0;
		}
		for (i = n; i < n + m; i++) {
			x[i] = s.b[i - n];
		}
	}
	return s.y;
}

int initial(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var) {
	int i, j, k;
	double w;

	k = init(s, m, n, a, b, c, x, y, var);

	if (b[k] >= 0)
		return 1;
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
	}
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
				/* x_k is nonbasic, add c_k */
				t[j] = t[j] + s->c[k];
				goto next_k;
			}
		}
		for (j = 0; j < n; j++) {
			if (s->var[n + j] == k)
				break; /* x_k is at row j */
		}
		s->y = s->y + s->c[k] * s->b[j];
		for (i = 0; i < n; i++) {
			t[i] = t[i] - s->c[k] * s->a[j][i];
		}
		next_k:;
	}

	for (i = 0; i < n; i++) {
		s->c[i] = t[i];
	}

	free(t);
	free(s->x);
	return 1;
}

int init(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var) {
	int i, k;
	//*s = (simplex_t){ m, n, var, a, b, c, x, y };
	s->n = n;
	s->m = m;
	s->var = var;
	s->a = a;
	s->b = b;
	s->c = c;
	s->x = x;
	s->y = y;


	if (s->var == NULL) {
		s->var = calloc(m + n + 1, sizeof(int));
		for (i = 0; i < n + m; i++)
			s->var[i] = i;
	}
	for (k = 0, i = 1; i < m; i++) {
		if (b[i] < b[k])
			k = i;
	}
	return k;
}

double simplex(int m, int n, double **a, double *b, double *c, double *x, double y)
{
	return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}

double intopt(int m, int n, double** a, double* b, double* c, double* x)
{
	node_t *p = initial_node(m, n, a, b, c);

	//set h = {p}
	set_t *h = calloc(1, sizeof(set_t));
	h->first = calloc(1, sizeof(list_t));
	h->first->elem = p;

	double z = -INFINITY; //best integer solution so far
	p->z = simplex(p->m, p->n, p->a, p->b, p->c, p->x, 0);
	if (integer(p) || !isfinite(p->z)) {
		z = p->z;
		if (integer(p)) {
			memcpy(x, p->x, sizeof(double));
		}
		// free_node_t(p);
		// free_set_t(h);
		return z;
	}
	branch(p, z);
	while(h->first != NULL) {
		p = set_pop(h);
		succ(p, h, m, n, a, b, c, p->h, 1, floor(p->xh), &z, x);
		succ(p, h, m, n, a, b, c, p->h, -1, -ceil(p->xh), &z, x);
	}
	// free_set_t(h);
	if (z == -INFINITY) {
		return NAN;
	}
	return z;
}
