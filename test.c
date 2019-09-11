#include "includes/malloc.h"
#include "libft/srcs/libft.h"

int main(int argc, const char *argv[])
{
	char	*test_str;
	test_str = (char *)malloc(sizeof(char)*11);

	ft_strcpy(test_str, "HelloWorld\0");
	printf("%s\n", test_str);
	free(test_str);
	return 0;
}
