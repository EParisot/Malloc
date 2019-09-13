#include "../includes/malloc.h"
#include "../libft/libft.h"

void test_str(void)
{
	char	*test_str;
	test_str = (char *)malloc(sizeof(char)*11);
	ft_strcpy(test_str, "HelloWorld\0");
	printf("%s\n", test_str);
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
	test1 = (char *)malloc(sizeof(char)*2048);
	test2 = (char *)malloc(sizeof(char)*2048);
	test3 = (char *)malloc(sizeof(char)*2048);
	ft_strcpy(test1, "Hello \0");
	ft_strcpy(test2, "World\0");
	ft_strcpy(test3, "Bitch\0");
	printf("%s\n", test1);
	printf("%s\n", test2);
	printf("%s\n", test3);
	free(test2);
	free(test1);
	free(test3);
}

int main()
{
	//test_str();
	//test_double();
	//test_show();
	test_triple();
	//while (1);
	return 0;
}
