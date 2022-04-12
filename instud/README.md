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
> `a` is allocated on the heap and only if memory is available it will allocate it, otherwise it will return `NULL`. `a` is also a pointer.
>
> `b` will allocate memory regardless if there is enough space or not, this can lead to it overwriting other parts of the code. `b` is not a pointer.

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
> Because `a` is an int, i.e. signed, the value of a will either be `-1` or `1` when interpreted as an int.
>
>TODO: rewrite this, ask what happens if you have multiple types in the same struct

5. Why do C compilers use a stack pointer in the machine code they produce? Is it always used in every function or do some functions not need it?

. todo

6. Does pipelining reduce the number of clock cycles to execute an instruction, or what is the purpose of pipelining?

> Pipelining does not necessarily effect the number of clock cycles of an instruction, but rather it starts multiple instructions each clock cycle so that the program execution is done faster. The pipelining can do some smart optimizations like change name of register to make fewer dependencies between instructions.

7. What is the purpose of a reorder buffer in a superscalar processor and what is it?

> A reorder buffer is a FIFO queue that controls that instructions finish in the same order as they were dispatched.
>
> If one of the instructions in the queue-structure is cancelled, the following instructions must be updated so that none of them gets the value from the cancelled instruction.
>
> TODO: example of usage

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

> In the second code the last 3 lines can be ran in parallel with the first 3 lines, this is achieved by changing from `r1` to `r2` in the last 3.

9. What is the purpose of branch prediction in a superscalar processor?

> Branch prediction is when hardware fetches instructions from memory where it guesses the program will go. Usually it predicts the right but if not, i.e. misprediction is detected, all wrong path instructions must be marked as such.
>
> TODO: example of usage

10. What can a reasonable cache block size be?

> `power.cs.lth.se` transferes 128 bytes at a time, that can be one reasonable cache block size.

11. What is meant by cache associativity and why is that useful?

> If a words is put in a row in a cache it implies that every row must be checked to see if the address matches. This is called a fully-associative cache.

TODO: make this more clear

12. Why are not fully associative data caches used?

> They are expensive (time).

TODO: more?


13. Which two types of locality of references are exploited with caches to reduce the execution times of programs? Give examples of C code fragments in which each type of locality can be exploited.

> Temporal locality: if a program has accessed a particular location *A* in memory, it is likely that is will accesses it again.
> Examples of use:
>  - The instructions in a loop are accessed next iteration as well.
> - The loop index variable is usually accessed frequently.
The stack space is often accessed again when a new function is called (since that space is reused).
> - An object is typically accessed for a while and then the program is done with it.
>
> Spatial locality: is a program have accessed a word at memory location *A* it is likely that it will access a word a *A+1*.
> Examples of use:
> - The instructions are accessed one after the other — until there is a branch.
> - Elements of an array are often accessed one after the other.
> - Often several variables in an object is accessed and if these are put close together (by the programmer) then there will be spatial locality.
>
> Code example:

```
int* A;
A = calloc(10, sizeof(int));
for (int i = 0; i < 10; i++) { // i is accessed often!
  A[i] = ...; // A+1 will be next!
}
```

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

> cannot really find an answer for this...

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

TODO, more info? Code example

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

24. What does designated initializer mean? Give an example?

> It means that elements in a compound literal can be given in random order as long as a index or struct member name is specified. See example below.

```
int a[6] = {[4] = 29, [2] = 15 }; // and
int a[6] = {[4]29 , [2]15 };

// is equivalent to
int a[6] = {0 0 15 0 29 0};

// not can also use
int a[] = {[0 ... 9] = 1, [10 ... 99] = 2, [100] = 3 };
```
> *source: https://www.geeksforgeeks.org/designated-initializers-c/*

> Elements in the last line will have a `1` between indices 0 and 9, a `2` between 10 and 99 and a `3` at 100.
>
> Another example with structs
```
typedef struct {
  double x;
  double y;
} point_t;

point_t p1, p2;

p1 = (point_t) { 1.23, 4.56 };
p2 = (point_t) { .y = 4.56, .x = 1.23 };

// p1 and p2 have the same values in x and y
```

25. What does implementation-defined behavior mean? Give an example.  

> In C the compiler and the standard library that comes with it is referred as the implementation. An implementation is free to make certain decisions about the behavior which it must follow consistently and document it. This is called implementation defined behavior. Portable programs should avoid using language constructs with implementation defined behavior. Some example of these are:
> - The size and precision of various types.
> - How bit-fields are layed out in memory.
> - Whether right shift of an signed integer is arithmetic or logical.
> - Whether the register keyword has any effect on performance

26. What does unspecified behavior mean? Give an example.

> Unspecified behavior lets the implementation decide the behavior and it does not have to document it cause it can vary "randomly" each compilation or implementations. This can occur of e.g. optimization, and should be avoided if it can affect observable behavior. Examples of unspecified behavior:
> - The order of evaluation in `+` is unspecified:
> ```
> int a = 12, b = 13;
> int f() {return a;};
> int g() {return b;};
> int main () { f() + g(); return 0; }
> ```
> - The order of evaluation of arguments in function calls.
> - Whether two identical string literals share memory.
> - Whether `setjmp` is a macro or identifier with external linkage; `&setjmp` is bad.

27. What does undefined behavior mean? Give an example.

> Even worse than implementation-defined or unspecified behavior is undefined behavior (ugly form of a bug). The implementation is permitted to do anything including:
> - Terminating compilation with an error message.
> - Continuing without understanding what happened.
> - Continuing possibly with a warning message.
>
> Examples of when a undefined behvaior can occur is:
> - A requirement which is not a Constraint is violated.
> - An invalid pointer is dereferenced.
> - An invalid pointer is dereferenced.
> - A stack variable is used before it was given a value.
> - Divide by zero.
> - Array index out of range

28. What does sequence point mean? Give an example.

> A sequence point in C determines when a side affects have been performed, the most common sequence point in C is the semicolon. The most important side effect is the modification of variables. A variable may not be modified more than once between two sequence points. The following is invalid:
> - `a = a = 1`
> - `b = b++`
> - `++c * c--`
>
> A variable may not be read after a modification before the next sequence point, thus `b = (a = 1) + a * 2` is also invalid.
>
> The code is invalid if the left operand of the add is evaluated first — which it may be since the evaluation order is unspecified.

29. What does static storage duration mean?

> Static storage duration us when a variable is not located on the stack but among global variables, it will then preserve its value across function calls.

30. What does integer promotion mean?

> When a type than does not lose any information when converted to an integer is converted to an integer. One example of this is when a `char` is printed as a integer to `stdout`.

```
char a = 30, b = 40, c = 10;
char d = a * b / c;
printf("%d", d); // integer promotion.
```
> *source: https://www.geeksforgeeks.org/integer-promotions-in-c/*

31. What does the operator ? : do?

> It is a if-else declaration one-liner.

```
int a = 10, b = 12;

int max = a ? a > b : b;

// same as
int max;
if (a > b)
  max = a;
else
  max = b;
```


32. How can you trigger a compile time error if your assumption about the size of an `int` does not hold? (hint: see previous question

> One can do as the code below follows:

```
int number_of_bits = ...;
int array[ sizeof(int) == number_of_bits ? 1 : -1 ];
```

> The compiler should complain if the number of elements is equal to -1, i.e. if our assumption of bit size of an `int`

33. What does internal linkage mean?

> A function with internal linkage, i.e. declared with static.

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

47. Can you add two pointers? If so, are there any restrictions?

> Can't add two pointers, but the values they point to can be added (if the type allowes). One can however subtract two pointers. Then how many of the type the pointer is referring to that can be stored between the two. The two pointers need to be pointing to the same type.

48. Can you subtract two pointers? If so, are there any restrictions?

> Yes you can, see answer to previous question.

TODO: restrictions?

49. Can you add an integer and a pointer `(i+p)`, and what would that mean?

> Yes you can, it would be like telling the pointer to move by `i` bytes. This `i` should be a multiple of the size (in bytes) of the type it is pointing two, otherwise bad things can occur.

50. an you subtract an integer from a pointer `(p-i)`, and what would that mean?

> Yes you can, it is the opposite of the answer in the previous answer.

51. Can you subtract a pointer from an integer `(i-p)`, and what would that mean?

> You cannot subtract a pointer from an integer.

52. What is dangerous with using a variable length array with a size `n` where `n` is read from input. Why is that not such a big problem when using `malloc`?

> When a VLA allocate memory there is no way of knowing if it succeeded or not. The VLA is located on the stack and if it allocates too much memory it can write over memory outside of the stack(?).
>
> If we instead would have used `malloc` it returns `NULL` if the allocation did not succeed.

TODO: what does VLA do if too much memory is allocated?

53. Why is alloca different (in addition to alloca not being ISO C) from using the VLA in the following loop?

```
void f(int n)
{
  int i;
  int* p;
  for (i = 0; i < n; i += 1) {
    int a[i];
    p = alloca(i * sizeof(int));
    /* use a and p here... */
  }
}
```

> The difference between the two is that after the function `f` returns, the list `p` is automatically freed/deallocated. The memory used by the VLA `a` is not freed/deallocated until the program is done.

54. Are there any restrictions on how inline may be used and if so why?

> Inlining a function means copying the statements of a function into the calling function instead of doing the call.
> There are restrictions when using inline functions using external linkage, they are:
> - An inline function with external linkage may not define modifiable data with static storage duration.
> - An inline function with external linkage may not reference any identifier with internal linkage.

TODO: example of usage

55. What do `setjmp` and `longjmp` do?

> Nonlocal goto functions.
>
> `setjmp`: saves a current environment that into a variable for later use by `longjmp`. After the invocation the value returned is zero, but after a return from `longjmp`, the value returned is the value passed to `lonjmp` (the second argument).\
> *source: https://www.tutorialspoint.com/c_standard_library/c_macro_setjmp.htm*
>
> `longjmp`: restores the environment saved by the most recent call to `setjmp`. The call to `longjmp` will result in another return from `setjmp`. \
> *source: https://www.tutorialspoint.com/c_standard_library/c_function_longjmp.htm*

56. Which three of the following lines always work, which may crash, and why?

```
int main()
{
  char* s = "hello";
  char t[] = "hello";
  s[0] = ’H’;
  t[0] = ’H’;
}
```

> The third row is the uncertain one. Because the type `char*` is of read only we cannot change a value in it. The type is called a string literal and is in most compilers stored in read only memory. The C standard says that a string literal have static storage duration, trying to modify the literal will cause undefined behavior. \
TODO: correct?
>
> Side note, the list `t` is of size 6 even though the word "hello" is 5 characters long. This is done because the compiler automatically adds a `\0`

57. Is returning a value `x` from `main` equivalent to calling `exit(x)` and why?

> From `man return`:
```
return halts a currently running function.
The exit status is set to STATUS if it is given.
```
> If the function is `main` there is no difference between the two key words.

TODO: correct?

58. Can you copy a struct with an assignment statement like this?

```
struct { int a; } s, t;
void f()
{
  s = t;
}
```

> Yes, the value of `a.a` is now the same as in `t.a`.

59. Can you copy an array with an assignment statement like this?

```
int a[10];
int b[10];
void f()
{
  a = b;
}
```

> No, instead use `memcpy`, see below.

```
void f()
{
  memcpy(b, a, 3*sizeof(int));
}
```

60. What happens, if anything, to the elements of the array `a` in the statement in `f` ?

```
int a[10];
int b[10];
int* p = a;
int* q = b;
void f()
{
  p = q;
}
```

> Both `a` and `b` are located on the stack and will thus be deleted once the program is done.
>
>The stack stores temporary variables created by a function, these variables are declared, stored and initialized in runtime. The stack is only temporary storage. When the computer task is done, the memory of the variables will be automatically deleted.
>
>  The stack section mostly contains methods, local variable, and reference variables.

61. Why is the following code invalid C and would it, according to ISO C, help to use const?

```
int n = 10;
int a[n];
int main()
{
  return 0;
}
```

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

**Speculative execution** (F06): instructions can start execute before it is known that they really should, but they are not permitted to permanently modify (destroy) either memory or registers. Works on superscalar processor. 


const - read only
