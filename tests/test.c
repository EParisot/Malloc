#include "../includes/malloc.h"
#include "../libft/libft.h"

void test_str(void)
{
	char	*test_str;
	test_str = (char *)malloc(sizeof(char)*11);
	ft_strcpy(test_str, "HelloWorld\0");
	printf("%s\n", test_str);
	show_alloc_mem();
	free(test_str);
}

void test_double(void)
{
	char	*test1;
	char	*test2;
	test1 = (char *)malloc(sizeof(char)*42);
	test2 = (char *)malloc(sizeof(char)*42);
	ft_strcpy(test1, "Hello \0");
	ft_strcpy(test2, "World\0");
	printf("%s\n", test1);
	printf("%s\n", test2);
	show_alloc_mem();
	free(test1);
	free(test2);
}

void test_show(void)
{
	void	*test;
	test = malloc(sizeof(char)*11);
	ft_strcpy(test, "HelloWorld\0");
	printf("%s\n", test);
	show_alloc_mem();
	free(test);
}

void test_triple(void)
{
	char	*test1;
	char	*test2;
	char	*test3;
	test1 = (char *)malloc(sizeof(char)*6048);
	test2 = (char *)malloc(sizeof(char)*6048);
	test3 = (char *)malloc(sizeof(char)*6048);
	ft_strcpy(test1, "Hello \0");
	ft_strcpy(test2, "World\0");
	ft_strcpy(test3, "Bitch\0");
	printf("%s\n", test1);
	printf("%s\n", test2);
	printf("%s\n", test3);
	show_alloc_mem();
	free(test2);	show_alloc_mem();
	free(test1);	show_alloc_mem();
	free(test3);
}

void	test_empty_show()
{
	show_alloc_mem();
}

void	test_free_null()
{
	free(NULL);
}

void	test_bad_malloc()
{
	void	*test;
	//test = malloc(-42);
	test = malloc(SIZE_MAX + 1);
}

void test_smaller_realloc()
{
	char *test1;
	char *test2;

	test1 = malloc(59);
	ft_strcpy(test1, "Hello world, what the fuck can i say !\0");
	printf("%s\n", test1);
	test2 = realloc(test1, 16);
	ft_strcpy(test2, "Hello world\0");
	printf("%s\n", test2);
	free(test2);
}

void test_bigger_realloc()
{
	char *test1;
	char *test2;

	test1 = malloc(16);
	ft_strcpy(test1, "Hello world\0");
	printf("%s\n", test1);
	test2 = realloc(test1, 59);
	ft_strcpy(test2, "Hello world, what the fuck can i say !\0");
	printf("%s\n", test2);
	free(test2);
}


int main()
{
	//test_str();
	//test_double();
	//test_show();
	//test_triple();
	//test_empty_show();
	//test_free_null();
	//test_bad_malloc();
	test_smaller_realloc();
	test_bigger_realloc();
	//while (1);
	return 0;
}
