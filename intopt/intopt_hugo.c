#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define epsilon 1e-6
#define PRINT 0
#define MAIN 1 /* Set to 0 for forsete compatiblity */

#if PRINT
#define pr(...)                       \
    do                                \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
    } while (0)
#else
#define pr(...) /* no effect at all */
#endif

typedef struct simplex_t simplex_t;
typedef struct node_t node_t;
typedef struct list_t list_t;

struct simplex_t
{
    int m;
    int n;
    int *var;
    double **a;
    double *b;
    double *x;
    double *c;
    double y;
};

struct node_t
{
    int m;       /* Constraints  */
    int n;       /* Decision variables */
    int k;       /* Parent branches on x_k */
    int h;       /* Branch on x_h */
    double xh;   /* x_h */
    double ak;   /* Parent a_k */
    double bk;   /* Parent b_k */
    double *min; /* Lower bounds */
    double *max; /* Upper bounds */
    double **a;
    double *b;
    double *x;
    double *c;
    double z;
};

struct list_t
{
    node_t *node;
    list_t *prev;
    list_t *next;
};

/* Prints a simplex_t struct in slack form */
void print_struct(simplex_t *s)
{
    if (PRINT)
    {
        printf("max z = ");
        for (int i = 0; i < s->n; i++)
            printf("%10.1lfx_%d +", s->c[i], s->var[i]);
        printf("%10.1lf\n", s->y);

        for (int i = 0; i < s->m; i++)
        {
            printf("  x_%d = %10.1lf - (", s->var[s->n + i], s->b[i]);
            for (int j = 0; j < s->n; j++)
            {
                printf("%10.1lfx_%d%s", s->a[i][j], s->var[j], j != s->n - 1 ? " + " : "");
            }
            printf(")\n");
        }
        printf("\n");
    }
}

double **make_matrix(int m, int n)
{
    double **a;
    int i;
    a = calloc(m, sizeof(double *));
    for (i = 0; i < m; i += 1)
        a[i] = calloc(n, sizeof(double));
    return a;
}

void copy_matrix(double **dest, double **src, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        memcpy(dest[i], src[i], sizeof(double) * n);
    }
}

void free_matrix(double **a, int m)
{
    for (int i = 0; i < m; i++)
        free(a[i]);

    free(a);
}

void free_node(node_t *node, int m)
{
    free(node->min);
    free(node->max);
    free_matrix(node->a, m);
    free(node->b);
    free(node->x);
    free(node->c);
    //free(node);
}

int init(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var)
{
    int i, k;
    s->m = m;
    s->n = n;
    s->a = a;
    s->b = b;
    s->c = c;
    s->x = x;
    s->y = y;
    s->var = var;

    if (s->var == NULL)
    {
        s->var = calloc(m + n + 1, sizeof(int));
        for (i = 0; i < m + n; i++)
        {
            s->var[i] = i;
        }
    }
    // finds k with smallest b
    for (k = 0, i = 1; i < m; i++)
    {
        if (s->b[i] < s->b[k])
        {
            k = i;
        }
    }
    return k;
}

int select_nonbasic(simplex_t *s)
{
    for (int i = 0; i < s->n; i++)
    {
        if (s->c[i] > epsilon)
            return i;
    }
    return -1;
}

void pivot(simplex_t *s, int row, int col)
{
    double **a = s->a;
    double *b = s->b;
    double *c = s->c;
    int m = s->m;
    int n = s->n;

    // Swap place col (non-basic -> basic) and row (basic -> non-basic)
    int t = s->var[col];
    s->var[col] = s->var[n + row];
    s->var[n + row] = t;
    s->y = s->y + c[col] * b[row] / a[row][col];

    // Change all coefficients in object function (c vector)
    for (int i = 0; i < n; i++)
    {
        if (i != col)
        {
            c[i] = c[i] - c[col] * a[row][i] / a[row][col];
        }
    }
    c[col] = -c[col] / a[row][col];

    // Change constant term b[i] in all basic equations
    for (int i = 0; i < m; i++)
    {
        if (i != row)
        {
            b[i] = b[i] - a[i][col] * b[row] / a[row][col];
        }
    }

    // Change all non-col coefficients a[i][j] in all basic equations expect row
    for (int i = 0; i < m; i++)
    {
        if (i != row)
        {
            for (int j = 0; j < n; j++)
            {
                if (j != col)
                {
                    a[i][j] = a[i][j] - a[i][col] * a[row][j] / a[row][col];
                }
            }
        }
    }

    // Change col coefficient a[i][col] for all rows except row
    for (int i = 0; i < m; i++)
    {
        if (i != row)
        {
            a[i][col] = -a[i][col] / a[row][col];
        }
    }

    // Change all non-col coefficients in row (a[row][i])
    for (int i = 0; i < n; i++)
    {
        if (i != col)
        {
            a[row][i] = a[row][i] / a[row][col];
        }
    }

    // Change last coefficients (b[row] and a[row][col])
    b[row] = b[row] / a[row][col];
    a[row][col] = 1 / a[row][col];
}

void prepare(simplex_t *s, int k)
{
    int m = s->m;
    int n = s->n;

    // move all basic variables one steep to the right
    for (int i = m + n; i > n; i--)
    {
        s->var[i] = s->var[i - 1];
    }

    // add non-basic x_m+n
    s->var[n] = m + n;
    n = n + 1;

    // give new non-basic coefficient -1 in all rows
    for (int i = 0; i < m; i++)
    {
        s->a[i][n - 1] = -1;
    }

    s->x = calloc(m + n, sizeof(double));
    s->c = calloc(n, sizeof(double));
    s->c[n - 1] = -1;
    s->n = n;

    pivot(s, k, n - 1); // Pivoting the new non-basic makes all elements in b positive
}

double xsimplex(int m, int n, double **a, double *b, double *c, double *x, double y, int *var, int h);

int initial(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var)
{
    int i = 0, j = 0, k = 0;
    double w;

    k = init(s, m, n, a, b, c, x, y, var);

    pr("Original system:\n");
    print_struct(s);

    if (s->b[k] >= 0)
        return 1;

    prepare(s, k); // Add new non-basic and pivot it with k
    n = s->n;      // n++ in prepare
    pr("Prepared system:\n");
    print_struct(s);

    pr("Computing y\n");
    s->y = xsimplex(m, n, s->a, s->b, s->c, s->x, 0, s->var, 1); // No recursion since b is positive after prepare
    pr("Simplex complete\n");

    // Check if x_{m+n} == 0, then system have a solution
    for (i = 0; i < m + n; i++)
    {
        if (s->var[i] == m + n - 1)
        {
            if (fabs(s->x[i]) > epsilon)
            {
                free(s->x);
                free(s->c);
                return 0;
            }
            else
            {
                break;
            }
        }
    }

    // if x_{m+n} is basic, make non-basic by performing a pivot
    if (i >= n)
    {
        for (j = k = 0; k < n; k++)
        {
            if (fabs(s->a[i - n][k]) > fabs(s->a[i - n][j]))
            {
                j = k;
            }
        }
        pivot(s, i - n, j);
        i = j;
    }

    if (i < n - 1)
    {
        k = s->var[i];
        s->var[i] = s->var[n - 1];
        s->var[n - 1] = k;

        for (k = 0; k < m; k++)
        {
            w = s->a[k][n - 1];
            s->a[k][n - 1] = s->a[k][i];
            s->a[k][i] = w;
        }
    }

    // Reset system to original c and y
    free(s->c);
    s->c = c;
    s->y = y;

    // Remove the additional non-basic variable
    for (k = n - 1; k < n + m - 1; k++)
    {
        s->var[k] = s->var[k + 1];
    }

    n = --s->n;
    double *t = calloc(n, sizeof(double));

    for (k = 0; k < n; k++)
    {
        for (j = 0; j < n; j++)
        {
            if (k == s->var[j])
            {
                t[j] = t[j] + s->c[k];
                goto next_k;
            }
        }

        for (j = 0; j < m; j++)
        {
            if (s->var[n + j] == k)
                break;
        }

        s->y = s->y + s->c[k] * s->b[j];
        for (i = 0; i < n; i++)
        {
            t[i] = t[i] - s->c[k] * s->a[j][i];
        }

    next_k:;
    }
    for (i = 0; i < n; i++)
    {
        s->c[i] = t[i];
    }

    free(t);
    free(s->x);
    return 1;
}

double xsimplex(int m, int n, double **a, double *b, double *c, double *x, double y, int *var, int h)
{
    simplex_t s;
    int row, col;

    if (!initial(&s, m, n, a, b, c, x, y, var))
    {
        free(s.var);
        return NAN;
    }
    pr("Starting simplex algorithm for:\n");
    print_struct(&s);

    while ((col = select_nonbasic(&s)) >= 0)
    {
        row = -1;

        // Select most constraining basic variable
        for (int i = 0; i < m; i++)
        {
            if (a[i][col] > epsilon && (row < 0 || b[i] / a[i][col] < b[row] / a[row][col]))
            {
                row = i;
            }
        }
        if (row < 0)
        {
            free(s.var);
            return NAN;
        }
        pivot(&s, row, col);
        pr("Performing pivot with row = x_%d, col = x_%d, gives:\n", row, col);
        print_struct(&s);
    }

    if (h == 0)
    {
        // Set non-basic coordinates to 0
        for (int i = 0; i < n; i++)
        {
            if (s.var[i] < n)
            {
                x[s.var[i]] = 0;
            }
        }

        // set basic coordinates to b
        for (int i = 0; i < m; i++)
        {
            if (s.var[n + i] < n)
            {
                x[s.var[n + i]] = s.b[i];
            }
        }

        free(s.var);
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            x[i] = 0;
        }

        for (int i = n; i < n + m; i++)
        {
            x[i] = s.b[i - n];
        }
    }

    return s.y;
}

double simplex(int m, int n, double **a, double *b, double *c, double *x, double y)
{
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}

/* Branch and bound below */

node_t *initial_node(int m, int n, double **a, double *b, double *c)
{
    node_t *p = calloc(1, sizeof(node_t));
    p->a = make_matrix(m + 1, n + 1);
    p->b = calloc(m + 1, sizeof(double));
    p->c = calloc(n + 1, sizeof(double));
    p->x = calloc(n + 1, sizeof(double));
    p->min = calloc(n, sizeof(double));
    p->max = calloc(n, sizeof(double));
    p->m = m;
    p->n = n;

    copy_matrix(p->a, a, m, n);
    memcpy(p->b, b, sizeof(double) * m);
    memcpy(p->c, c, sizeof(double) * n);

    for (int i = 0; i < n; i++)
    {
        p->min[i] = -INFINITY;
        p->max[i] = INFINITY;
    }

    return p;
}

node_t *extend(node_t *p, int m, int n, double **a, double *b, double *c, int k, double ak, double bk)
{
    node_t *q = calloc(1, sizeof(node_t));
    int i, j;
    q->k = k;
    q->ak = ak;
    q->bk = bk;

    if (ak > 0 && p->max[k] < INFINITY)
        q->m = p->m;
    else if (ak < 0 && p->min[k] > 0)
        q->m = p->m;
    else
        q->m = p->m + 1;

    q->n = p->n;
    q->h = -1;
    q->a = make_matrix(q->m + 1, q->n + 1);
    q->b = calloc(q->m + 1, sizeof(double));
    q->c = calloc(q->n + 1, sizeof(double));
    q->x = calloc(q->n + 1, sizeof(double));
    q->min = calloc(n, sizeof(double));
    q->max = calloc(n, sizeof(double));

    memcpy(q->min, p->min, sizeof(double) * p->n);
    memcpy(q->max, p->max, sizeof(double) * p->n);
    copy_matrix(q->a, a, m, n);
    memcpy(q->b, b, sizeof(double) * m);
    memcpy(q->c, c, sizeof(double) * n);

    if (ak > 0)
    {
        if (q->max[k] = INFINITY || bk < q->max[k])
            q->max[k] = bk;
    }
    else if (q->min[k] = -INFINITY || -bk > q->min[k])
    {
        q->min[k] = -bk;
    }

    for (i = m, j = 0; j < n; j++)
    {
        if (q->min[j] > -INFINITY)
        {
            q->a[i][j] = -1;
            q->b[i] = -q->min[j];
            i++;
        }

        if (q->max[j] < INFINITY)
        {
            q->a[i][j] = 1;
            q->b[i] = q->max[j];
            i++;
        }
    }
    return q;
}

bool is_integer(double *xp)
{
    double x = *xp;
    double r = round(x);

    if (fabs(r - x) < epsilon)
    {
        *xp = r;
        return true;
    }
    else
    {
        return false;
    }
}

int integer(node_t *p)
{
    int i;

    for (i = 0; i < p->n; i++)
    {
        if (!is_integer(&p->x[i]))
            return 0;
    }
    return 1;
}

void bound(node_t *p, list_t **h, double *zp, double *x)
{
    list_t *q = *h;

    if (p->z > *zp)
    {
        *zp = p->z;
        memcpy(x, p->x, sizeof(double) * p->n); // save best x

        while (q != NULL)
        {
            if (q->node->z < p->z)
            {
                // Check if first
                if (q->prev == NULL)
                {
                    *h = q->next;
                    if (q->next != NULL)
                        q->next->prev = NULL;
                }
                else
                {
                    q->prev->next = q->next;

                    // Checks if last
                    if(q->next != NULL) {
                        q->next->prev = q->prev;
                    }
                }

                list_t *tmp = q->next;
                free(q->node->min);
                free(q->node->max);
                free(q->node);
                free(q);
                q = tmp;
            }
            else
            {
                q = q->next;
            }
        }
    }
}

node_t *pop(list_t **h)
{
    if (*h != NULL)
    {
        list_t *result = *h;
        node_t *node = result->node;
        *h = result->next;
        if (result->next != NULL)
            result->next->prev = NULL;

        free(result);
        return node;
    }
    return NULL;
}

void push(list_t **h, node_t *node)
{
    list_t *q = calloc(1, sizeof(list_t));
    q->node = node;
    q->next = *h;
    if (*h != NULL)
        (*h)->prev = q;
    *h = q;
}

int branch(node_t *q, double z)
{
    double min, max;

    if (q->z < z)
        return 0;

    for (int h = 0; h < q->n; h++)
    {
        if (!is_integer(&q->x[h]))
        {
            if (q->min[h] = -INFINITY)
                min = 0;
            else
                min = q->min[h];

            max = q->max[h];
            if (floor(q->x[h]) < min || ceil(q->x[h]) > max)
                continue;

            q->h = h;
            q->xh = q->x[h];
            free_matrix(q->a, q->m + 1);
            free(q->b);
            free(q->c);
            free(q->x);
            return 1;
        }
    }
    return 0;
}

void succ(node_t *p, list_t **h, int m, int n, double **a, double *b, double *c, int k, double ak, double bk, double *zp, double *x)
{
    node_t *q = extend(p, m, n, a, b, c, k, ak, bk);

    if (q == NULL)
        return;

    q->z = simplex(q->m, q->n, q->a, q->b, q->c, q->x, 0);
    if (isfinite(q->z))
    {
        if (integer(q))
        {
            bound(q, h, zp, x);
        }
        else if (branch(q, *zp))
        {
            push(h, q);
            return;
        }
    }
    free(q->min);
    free(q->max);
    free_matrix(q->a, q->m+1);
    free(q->b);
    free(q->c);
    free(q->x);
    free(q);
}

double intopt(int m, int n, double **a, double *b, double *c, double *x)
{
    double z = -INFINITY;
    node_t *p = initial_node(m, n, a, b, c);
    list_t *h = NULL;
    push(&h, p);

    p->z = simplex(p->m, p->n, p->a, p->b, p->c, p->x, 0);

    if (integer(p) || !isfinite(p->z))
    {
        z = p->z;
        if (integer(p))
            memcpy(x, p->x, sizeof(double) * p->n);

        free(h);
        free_node(p, p->m + 1);
        free(p);
        return z;
    }

    branch(p, z);
    while ((p = pop(&h)) != NULL)
    {
        succ(p, &h, m, n, a, b, c, p->h, 1, floor(p->xh), &z, x);
        succ(p, &h, m, n, a, b, c, p->h, -1, -ceil(p->xh), &z, x);

        free(p->min);
        free(p->max);
        free(p);
    }

    if (z == -INFINITY)
        return NAN;
    else
        return z;
	/*
	#if MAIN
	int main(void)
	{
		int m;
		int n;

		scanf("%d %d", &m, &n);
		printf("m = %d, n = %d\n\n", m, n);

		double **a = make_matrix(m, n + 1);
		double *b = calloc(m, sizeof(*b));
		double *c = calloc(n, sizeof(*c));
		double *x = calloc(n + 1, sizeof(double));

		for (int i = 0; i < n; i++)
		scanf("%lf", &c[i]);
		for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		scanf("%lf", &a[i][j]);

		for (int i = 0; i < m; i++)
		scanf("%lf", &b[i]);

		printf("z = %.1lf\n", intopt(m, n, a, b, c, x));
		// printf("z = %.1lf\n", simplex(m, n, a, b, c , x, 0));

		free_matrix(a, m);
		free(b);
		free(c);
		free(x);

		return 0;
	}
	#endif
	*/
}
