#! /bin/sh
make re CFLAGS=-g && gcc test.c -L. -lft_malloc libft/libft.a
./a.out
