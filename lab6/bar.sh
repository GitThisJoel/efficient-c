#!/bin/bash
clang -O3 matmul.c tbr.s timebase.c -mcpu=power8 -o matmul.clang
gcc -O3 matmul.c tbr.s timebase.c -mcpu=power8 -o matmul.gcc
xlc -O3 matmul.c tbr.s timebase.c -mcpu=power8 -o matmul.ibm
pgcc -O3 matmul.c tbr.s timebase.c -tp=pwr8 -o matmul.nvidia
echo -n "clang: "; ./matmul.clang
echo -n "gcc: "; ./matmul.gcc
echo -n "ibm: "; ./matmul.ibm
echo -n "nvidia: "; ./matmul.nvidia
