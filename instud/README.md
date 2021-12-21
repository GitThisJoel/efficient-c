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

8. todo

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

18. todo

19. todo

20. todo

21. todo

22. todo

23. todo

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

34. todo

35. todo

36. todo

37. todo

38. todo

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
