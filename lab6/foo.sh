#!/bin/bash

for x in s 0 1 2 3
do
	gcc -O$x -o $x mipsx.c tbr.s timebase.c
	./$x
done
