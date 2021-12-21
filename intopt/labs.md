# LABS 🚗

### lab4
1. code
2. code
3. code
4. big test
5. most of  the time was spent in pivot, especially when dividing.
6. we can see how much of the time is spent in a certain fnction as well as number of calls to it etc. Can also see how much of the time is spent in a certain function and how much time we spend in subcalls of the functions.
7. measures our code coverage, the numbers to the left indicate the number of times a line were executed. The option -b is branch probabilities, it shows the number of times a part of the code were executed / "jumped over".
8. total instructions = I: 322223, D: 98049, LL: 4454, cache miss rate = I1: 0.52%, D1: 2.8% LL: 0.7%.

### lab5
1. init
2. copy
3. Our guess is 400 cycles (2 read, 1 add, 1 write 100 times)
4. trace file
5. disassemble
6. copy
7. loop
8. scrollpv
9. first lfdx: cycle 49, last lfdx: 707, main returning: 714. Per cycle: ~7~ 3.
10. first lfdx: cycle 51, last lfdx: 704, main returning: 711. Per cycle: ~7~ 6.
11. scrollpv
12. div takes a long time (~20 cycles), which makes efficient pipelining very hard.
13. If multiple instructions are dependant on the same register but not data dependant between them the register is renamed to another unused one.

### lab6
4.

- T = the symbol is in the text (code) section
- U = the symbols is undefined
- G = the symbols is initialized data section for small objects
- (D = the symbol is in the initialized data section)
- C = the symbol is common

-Os = optimize for size

file1 and file2 are the same

5. generally O0 < Os < O1 < O2 <~= O3
6. stvx: 10, vmaddfp: 0, vperm: 0, lvx: 6
7. faster in a lot of cases, not for fft, tree or bubble. we got an error from fprofile
8. ibm > nvidia > gcc > clang
