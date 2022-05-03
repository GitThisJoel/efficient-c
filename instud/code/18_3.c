#include <stdlib.h>
typedef struct list_t list_t;

struct list_t {
	void data;
	list_t* succ;
	list_t* pred;
};


/* (2p) create a new list node with this data. */
list_t* new_list(void* data)
{
	list_t* first = malloc(sizeof(list_t));

	first->data = data;
	first->succ = first;
	first->pred = first;

	return first;
}

/* (3p) deallocate entire list but not any data pointer. */
void free_list(list_t* list)
{
	list_t* node = list->succ;
	while (node->succ != list)
	{
		node = node->succ;
		free(node->pred);
	}
	free(node->succ);
	free(node);
}

/* (1p) return the number of nodes in the list. */
size_t length(list_t* list)
{
	if (list == NULL)
		return 0;

	size_t l = 1; // first elem
	list_t* node = list->succ;
	while(node != list) {
		l++;
		node = node->succ;
	}

	return l;
}

/* (3p) insert data first in the list. */
void insert_first(list_t** list, void* data)
{
	list_t* new_first = new_list(data);
	list_t* first = *list; // is this right??
	list_t* last = first->pred;

	first->pred = new_first;
	new_first->succ = new_first;

	last->succ = new_first;
	new_first->pred = last;
}

/* (3p) insert data last in the list. */
void insert_last(list_t** list, void* data)
{
	list_t* new_last = new_list(data);
	list_t* first = *list; // is this right??
	list_t* last = first->pred;

	first->succ = new_last;
	new_last->pred = new_last;

	last->pred = new_last;
	new_last->succ = last;
}

/* (3p) if the list is empty, return NULL, otherwise
* remove (and free) the first node in the list
* and return its data pointer.
*/
void* remove_first(list_t** list)
{
	if (length(list) == 0)
		reuturn NULL;

	void* data;

	list_t* first = *list; // is this right??
	list_t* last = first->pred;

	data = first->data;

	last->succ = first->succ;
	first->succ->pred = last;

	free(first);

	return data;
}

/*
* (5p) Use malloc to allocate memory for an array and
* return a pointer to an array with the contents (data
* pointers) of the list, and write the length of the
* list in the variable pointed to by size. If the size
* is zero, NULL should be returned.
*
* Note: the word array above is used in the sense that
* memory should be allocated for a number of elements
* in contiguous memory locations and not as in array
* declaration (i.e. not like  this: int a[10];)
*/
void** list_to_array(list_t* list, size_t* size)
{
	size_t l = lenght(list);
	if (l == 0)
		return NULL;

	a = malloc();
}
