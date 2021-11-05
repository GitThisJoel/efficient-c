# Example oral exam questions for EDAG01 Efficient C

1. What is the difference between `a` and `b` below? What can you do with them and when is the memory
for the arrays deallocated?
```
int* f(int n)
{
  int* a = calloc(n, sizeof(int));
  int b[n];
}
```
> answer
