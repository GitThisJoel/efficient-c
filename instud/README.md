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
> The difference between `a` and `b` is where in the memory the value is **stored** at.
>
> `a` is allocated on the heap and only if memory is available it will allocate it, otherwise it will return `NULL`.
>
> `b` will allocate memory regardless if there is enough space or not, this can lead to it overwriting other parts of the code.

2. topo

3. topo

4. Is this valid C and what does it mean?
```
struct {
    int a : 1;
} s;
```
> The colon ':' is a bit field and is used to define the size (in bits) of a variable. In this case a integer of size 1 bit is created.

5. topo
