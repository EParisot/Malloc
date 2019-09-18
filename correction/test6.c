#include "../includes/malloc.h"

int main()
{
	void *test;

	test = malloc(1024);
	test = malloc(1024 * 32);
	test = malloc(1024 * 1024);
	test = malloc(1024 * 1024 * 16);
	test = malloc(1024 * 1024 * 128);
	show_alloc_mem();
	return (0);
} 
