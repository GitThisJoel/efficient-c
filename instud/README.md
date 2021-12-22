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

2. Suppose you have a single-linked list `h`, the nodes have a field next, when `h` is empty, `h` is a null pointer, and you would like to write a function `f` to add a node `q` to the beginning of the list (so that the `h` in `g` below points to it), how would you call `f` in C (especially the arguments) and how could `f` be implemented? `f` should have two parameters `h` and `p` where `p` has type `node_t* p` and you should decide the type of the parameter `h`.

```
typedef struct node_t node_t;

struct node_t {
  node_t* next;
  /* other declarations. */
};

void f(node_t *h, node_t *p) {
  // h can be empty, then p->next = NULL
  p->next = h;
}

double g(void)
{
  node_t* h;
  node_t* q;
  /* more code. */
  f(h,q)
}
```

3. Same as above but we would like to add `q` to the end of the list.

```
void f(node_t *h, node_t *p) {
  if (h == NULL) {
    h = p;
    return;
  }
  node_t *prev;
  while(h != NULL) {
    prev = h;
    h = h->next;
  }
  prev->next = p;
}
```
TODO: is this correct?  


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

6. todo

7. todo

8. What is the purpose of rename registers in a superscalar processor?

> The rename registers are used when data dependencies occur between instructions. When an instruction is waiting for another it can sometimes be solved by renaming a register to an unused one. See example below.

```
r1 := m[1024]
r1 := r1 + 2
m[1032] := r1
r1 := m[2048] // r1 is waiting for r1 above
r1 := r1 + 4
m[2056] := r1
```

```
r1 := m[1024]
r1 := r1 + 2
m[1032] := r1
r2 := m[2048]
r2 := r2 + 4
m[2056] := r2
```

In the second code the last 3 lines can be ran in parallel with the first 3 lines, this is achieved by changing from `r1` to `r2` in the last 3.

9. todo

10. todo

11. todo

12. todo

13. todo

14. What does `#undef` mean?

> `#undef` removes the current definition of a definition, i.e. let say we have `#define a 10` with `#undef a` the definition of `a` cannot after that be used.

15. What does `#error` mean?

> `#error` it's a preprocessor directive indicates an error. The compiler gives fatal error if the directive is found and skips further compilation process.

16. What does `#ifndef` mean?

> Checks whether a token has been `#define` earlier in the file or in an included file. If not, it runs the code between it self and `#else`. If the token is defined then code between `#else` and `#endif`.

```
#ifndef <token>
/* code if token is NOT defined*/
#else
/* code to include if the token IS defined */
#endif
```

17. todo

18. What does `##` mean?

> Each argument passed to a macro is a token, sometimes it is wanted to paste arguments together then the paste macro `##` is used between two things to put together.

```
#define BUILD_FIELD(name) my_struct.field_##name

BUILD_FIELD(field1) => my_struct.field_field1
```

19. What does `volatile` mean?

> Indicates that the value might have been changed between accesses, even if it does not appear to be changed.

> It can be used to tell the compiler not to optimize away a variable (e.g. in a while condition) because it do not seem to be changed.

```
#include <signal.h>
volatile int x;
void catch_ctrl_c(int sig) {// called when you hit CTRL−C.
  x = 0; }

int main() {
  // below line will tell your computer to call the
  // function catch_ctrl_c when you hit CTRL−C.
  signal(SIGINT, catch_ctrl_c);
  x = 1;

  // when compiling with optimisation and without
  // volatile , GCC thinks X cannot change!
  while (x) ;
  }
```

20. What does `restrict` mean?

> It informs the compiler that two parameters can not point to the same memory area.

TODO, more info??

21. What does `continue` mean?

> Skip the reset of the code and go to the next iteration.

```
for(int i = 0; i < 5; i++) {
  if (i % 2 == 0)
    continue; // every even number will not be printed below
  printf("i is now = %d", i);
}
```

22. What does `case` mean?

> In a switch-case statement the `case` matches a specific value to the controlling expression in the `switch`. \
> The case value must be a integer or character constant. \
> The case statement can only be used inside the switch statement.

23. What does `default` mean?

>  In a switch-case statement, if none of the values in the cases matches the controlling expression, the statement will go to `default`.

24. todo

25. todo

26. todo

27. todo

28. todo

29. todo

30. todo

31. todo

32. todo

33. todo

34. What does the operator `~` do?

> Performs bitwise NOT on a variable.

```
0101 1101 //  original
1010 0010 // ~original
```

35. What does the operator `&` do?

> Performs bitwise AND between two variable.

```
A = 0011 1100
B = 0000 1101

(A & B) = 0000 1100
```

36. What does the operator `|` do?

> Performs bitwise OR between two variable.

```
A = 0011 1100
B = 0000 1101

(A | B) = 0011 1101
```

37. What does the operator `>>` do?

> Binary right shift. Moves a binary number certain number of steps.

```
A = 0011 1100
A >> 2 = 0000 1111
```

38. What does the operator `<<` do?

> Binary left shift.

```
A = 0011 1100
A << 2 = 1111 0000
```

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

41. todo

42. todo

43. todo

44. todo

45. todo

46. todo

47. todo

48. todo

49. todo

50. todo

51. todo

52. todo

53. todo

54. todo

55. todo

56. todo

57. todo

58. todo

59. todo

60. todo

61. todo

62. todo

63. todo

64. todo

65. todo

66. todo

67. todo

68. todo

69. todo

70. todo

71. todo

72. todo

73. todo

74. todo

75. todo

76. todo

77. todo

const - read only
