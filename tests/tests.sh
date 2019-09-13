#! /bin/sh

make re CFLAGS=-g

gcc tests/test.c -L. -lft_malloc libft/libft.a -o tests/tests
valgrind ./tests/tests
