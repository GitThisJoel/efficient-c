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

41. What is a compound literal?

> It is simply an anonymous variable initialized using special syntax, can be used to assign e.g. structs or literal types. Since the compound is a normal object the address of it can be used. \
See more in examples below and in `41.c`.

```
void print(point_t*); // some print function

// creating structs
typedef struct {
    double x;
    double y;
} point_t;

point_t p1 = (point_t) { 1.5, 2.4 };

print(&(point_t) {.x = 1.2, .y = 6.7});
print(&(point_t) {.y = 5.7, .x = 2.1});

// literal types
int* p = (int[]){ 1, 2, 3 };
int a = (int){ 1 };
```

42. What is the type of `1`?

> int? unknown ?

43. What is the type of `1ULL`?

> ULL = Unsigned Long Long int

44. What is the type of `1.0`?

> Float or double?

45. What is wrong with `099`?

> C interprets the number as an 8-based number, in that particular base 8 and 9 are not allowed. \
`error: invalid digit "9" in octal constant`

46. What does `alloca` do?

> Allocate memory that is automatically freed. Allocates memory in a stack frame of a caller, it is freed when the function that called `alloca` returns to its caller. \
If the allocation causes a stack overflow, program behavior is undefined.

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

62. Why does the code below, which tries to read a number from stdin, not work?

```
#incluce <stdio.h>
#define ISDIGIT(c) ((c) >= ’0’ && (c) <= ’9’)
int main()
{
  int num; // value of number.
  int c; // a char.

  num = 0;

  while (ISDIGIT(c = getchar()))
    num = 10 * num + c - ’0’;

  printf("the number is %d\n", num);

  return 0;
}
```

> In the while statement a macro is used, this will be expanded to `ISDIGIT(c = getchar()) => ((c = getchar()) >= '0' && (c = getchar()) <= '9')` and such each iteration reads input from stdin twice! To verify this, the program is ran with the input `1234`, the only number that are read are `2` and `4`. If the input would have been `1a` the return value should be `1` but because `a` is not between `0` and `9` the program will not return anything.
>
> If the input is instead read in the while loop it is correct.
> ```
> // ...
> c = getchar();
> while (ISDIGIT(c)) {
>   num = 10 * num + c - ’0’;
>   c = getchar();  
> }
> //...
> ```

63. Why is the following code invalid?

```
void f()
{
  int a = 1;
  a = 2 * ++a;
}
```

>  The reason why this code is invalid is because the variable `a` is changed more than once between two `;` (sequence points), this in C is illegal. The correct this a needs to be increased by 1 in a line above this.

64. What is the value of `-1 / 2U > 4`?

> The value of this expression is `1`. This is because when mixing signed and unsigned integers the unsigned `-1` is interpreted as $(2^{32}-1)$ is `UMAX_INT` = $2^{32}-1$, and thus the division becomes $(2^{32}-1)$ $/$ $2$ $=$ $2147483647$ which is greater than $4$.  

65. todo

66. todo

67. todo

68. todo

69. (nice) How can you measure how many times each source code line is executed? Do you need to compile the program with some special flag and why in that case?

> You can use the coverage testing tool `gcov` (only wroks with `GCC`). it can  be usesd to:
 - how often each line of code executes
 - what lines of code actaully executed
 - how much computing time each selection of code uses.
>
> To use this program you have to compile you code with `GCC` and the flag `--coverage`, which is synonym for `-fprofile-arcs -ftest-coverge`.
- `-fprofile-arcs`: generates the information indicating how many times each branch of your program is taken, i.e. it generates additional data relative to its execution. The information is stored into a `.gcda` file.
- `-ftest-coverge`: creates a file that contains control flow information, which `gcov` uses to create a human readable `.gcov` file. To make this file the flag uses the information created by `-fprofile-arcs` and generates a `.gcno` file. \
Example of usage:

```
gcc -fprofile-arcs -ftest-coverage file_name.c
```

70. todo


71. What is valgrind and what can it do for you?

> Valgrind is collection of tools for debugging and profiling programs. It can e.g. help finding memory leaks.

72. todo

73. todo

74. Why is the following code suboptimal and what can you do about it? How will your modification affect the program execution?

```
#define N (1000)
double a[N][N], b[N][N], c[N][N];
void matmul(void)
{
  size_t i, j, k;
  for (i = 0; i < N; i += 1) {
    for (j = 0; j < N; j += 1) {
      a[i][j] = 0;
      for (k = 0; k < N; k += 1)
        a[i][j] += b[i][k] * c[k][j];
    }
  }
}
```

> When accessing memory like this in a iterable type we want to make as small jumps as possible. In the case above, it jumps row instead of columns. This is bad because when allocating the memory the elements in rows are placed next to each other while the separate rows can be spread out. So when the last for-loop interates over `k` in c, it jumps between rows which is much more ineffective than iterating through a row.
>
> To make the code more efficient we change can write TODO

75. Explain how the following function tests if the parameter a is a power of two. Assume a > 0, i.e., a = 2k for some k ≥ 0.

```
int is_power_of_two(unsigned int a)
{
return (a & (a-1)) == 0;
}
```

> If `a` is a power of two, its binary form is `1` followed by `0`. Let say `a = 8 = 0b1000` which means that `a-1 = 7 = 0b0111`. If the bitwise AND operand is performed between the two the result is `0`. This only happens when `a` is a power of ´2´. A power of two is the only number where the previous is complete different.

76. todo

77. todo

const - read only
