#include <stdio.h>
#include <stdlib.h>
typedef struct node_t node_t;


struct node_t {
  int a;
  node_t* next;
};

void free_all(node_t* h) 
{
  if (h->next != NULL)
    free_all(h->next);
  free(h);
}

void print_node(node_t* h) 
{
  printf("a is %d\n", h->a);
  printf("next node is %p\n", h->next);
  if (h->next != NULL) 
  {
    printf("\nnext node is:\n");
    print_node(h->next);
  }
}

node_t* f(node_t* h, node_t* p) 
{
  node_t* temp;
  if (h == NULL)
  {
    h = malloc(sizeof(node_t));
    h->a = 0;
    h->next = p;
  }
  else
  {
    temp = h->next;
    h->next = p;
    p->next = temp;
  }

  return h;
}

void g(void)
{
  node_t* h;
  node_t* q;

  h = NULL;
  h = malloc(sizeof(node_t));
  h->a = 0;
  q = malloc(sizeof(node_t));
  q->a = 1;

  h = f(h,q);
  print_node(h);

  free_all(h);
}

int main(void) {
  g();
  return 0;
}

