#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define EPSILON 1e-6

struct node_t {
	int z;
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
		if (set->first == NULL)
			return;
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

void print_set(set_t *set) {
	printf("set: ");
	list_t *p = set->first;
	while (p != NULL) {
		printf("%d ", p->elem->z);
		p = p->next;
	}
	printf("\n");
}

void main() {

	//set h = {p}
	set_t *h = calloc(1, sizeof(set_t));

	for(int i = 0; i < 10; i++) {
		node_t *q = calloc(1, sizeof(node_t));
		q->z = i;
		set_add(h, q);
	}
	
	print_set(h);
	set_rm_below_z(h, 4);
	print_set(h);
}
