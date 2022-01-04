#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "error.h"
#include "poly.h"

struct poly_t {
	int n;
	int* exp;
	int* coeff;
};

typedef struct poly_t poly_t;

poly_t*	new_poly_from_string(const char* input) {
	poly_t* p;
	short ex, co, minus;
	int len, i, c, num, one;

	p = calloc(1, sizeof(poly_t*));
	p->n = 0;
	p->exp = calloc(100, sizeof(int));   // maybe dynamic
	p->coeff = calloc(100, sizeof(int)); // maybe dynamic

	len = strlen(input);

	i = 0;
	one = 1;

	ex = 0;
	co = 1;
	minus = 0;

	while(i < len) {
		c = input[i];

		if (isdigit(c)) {
			num = 0;
			while(i < len && isdigit(c))  {
				num = (num*10) + (c - '0');
				i++;
				c = input[i];
			}

			if (co){
				if (minus)
					num *= -1;
				p->coeff[p->n] = num;
				one = 0;
			}
			else if (ex) {
				p->exp[p->n] = num;
			}
		}

		switch (c) {
		case '^':
			co = 0;
			ex = 1;
			if (one) {
				p->coeff[p->n] = 1;
			}
			break;
		case 'x':
			p->exp[p->n] = 1;
			break;
		case '-':
			p->n++;
			minus = 1;
			co = 1;
			ex = 0;
			one = 1;
			break;
		case '+':
			p->n++;
			minus = 0;
			co = 1;
			ex = 0;
			one = 1;
			break;
		}

		i++;
	}
	p->exp[p->n] = 0;
	p->n++;

	/*
	printf("poly is %s\n", input);
	printf("coeffs: ");
	for (int i = 0; i < p->n; i++){
		printf("%d ", p->coeff[i]);
	}

	printf("\nexps: ");
	for (int i = 0; i < p->n; i++){
		printf("%d ", p->exp[i]);
	}
	printf("\n");
	*/
	return p;
}

void free_poly(poly_t* p) {
	free(p->exp);
	free(p->coeff);
	free(p);
}

poly_t*	mul(poly_t* p1, poly_t* p2) {
	poly_t* r;
	int i, j, k, c, e, n;
	// int re1, re2;

	r = calloc(1, sizeof(poly_t*));
	r->n = 0;
	r->exp = calloc(0, sizeof(int));
	r->coeff = calloc(0, sizeof(int));

	for (i = 0; i < p1->n; i++) {
		for (j = 0; j < p2->n; j++) {
			n = 1;
			c = p1->coeff[i] * p2->coeff[j];
			e = p1->exp[i] + p2->exp[j];

			for (k = 0; k < r->n; k++) {
				if (r->exp[k] == e) {
					r->coeff[k] += c;
					n = 0;
					break;
				}
			}

			if (n) {
				r->exp = realloc(r->exp, sizeof(int) * (r->n+1));
				r->coeff = realloc(r->coeff, sizeof(int) * (r->n+1));

				r->coeff[r->n] = c;
				r->exp[r->n] = e;
				r->n++;
			}
		}
	}

	int tmp;
	i = 0, j = 0;
	for (i = 0; i < r->n; i++) {
    		for (j = 0; j < r->n - i - 1; j++) {
			if (r->exp[j] < r->exp[j + 1]) {
	    			tmp = r->exp[j];
	    			r->exp[j] = r->exp[j + 1];
	    			r->exp[j + 1] = tmp;

				tmp = r->coeff[j];
	    			r->coeff[j] = r->coeff[j + 1];
	    			r->coeff[j + 1] = tmp;
			}
    		}
	}

	return r;
}

void print_poly(poly_t* p) {
	int i, c, e;
	int sign;

	c = p->coeff[0];
	e = p->exp[0];

	//c = (int) fabs(c);
	if (c == 0){}
	else if (c < 0) {
		if (e > 1) {
			if (c == 1) {
				printf("- x^%d ", e);
			} else {
				printf("- %dx^%d ", c, e);
			}
		} else if (e == 1) {
			printf("- %dx ", c);
		} else if (e == 0) {
			printf("- %d", c);
		}
	} else {
		if (e > 1) {
			if (c == 1) {
				printf("x^%d ", e);
			} else {
				printf("%dx^%d ", c, e);
			}
		} else if (e == 1) {
			printf("%dx ", c);
		} else if (e == 0) {
			printf("%d", c);
		}
	}


	for (i = 1; i < p->n; i++) {
		c = p->coeff[i];
		e = p->exp[i];

		if (c == 0)
			continue;

		sign = c < 0 ? '-' : '+';
		c = (int) fabs(c);
		if (e > 1) {
			if (c == 1) {
				printf("%c x^%d ", sign, e);
			} else {
				printf("%c %dx^%d ", sign, c, e);
			}
		} else if (e == 1) {
			printf("%c %dx ", sign, c);
		} else if (e == 0) {
			printf("%c %d", sign, c);
		}
	}
	printf("\n");
	return;
}
