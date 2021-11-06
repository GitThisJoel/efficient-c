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

2. todo

3. todo

4. Is this valid C and what does it mean?
```
struct {
    int a : 1;
} s;
```
> It is valid C code.
> The colon ':' is a bit field and creates a sort of 'pointer' to a number of adjacent bits that can later be referenced and changed later.
>
>TODO: rewrite this, ask what happens if you have multiple types in the same struct

5. todo

39. What is the difference between `a = b++` and `a = ++b`?
> Post-increment `a = b++`
>
> `a` is assigned the value in `b` and then `b` is increased by 1.
>
> Pre-increment `a = ++b`
>
> The value in `b` is increased then it is assigned to `a`

40. What is returned from the function below?
```
#include <stdbool.h>
int f() {
      bool p = 1;
      return p + 1;
}
```
> The value returned for the function `f` is 2. The type `bool` is stored in 1 byte. Only 0 is interpreted as `false` when using `if`.  
