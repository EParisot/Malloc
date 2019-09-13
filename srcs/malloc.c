/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/23 17:46:38 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/13 02:20:55 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void			init_first_header(size_t pagesize)
{
	t_header	*first_header;

	first_header = mmap(NULL, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC, \
								MAP_ANON | MAP_PRIVATE, -1, 0);
	first_header->size = pagesize - sizeof(t_header);
	first_header->is_free = 1;
	first_header->next = NULL;
	g_mem_start = first_header;
	printf("init first header at : %p with %zu\n", g_mem_start, first_header->size);
}

void			build_header(void *addr, size_t size, void *next_header)
{
	t_header	*new_header;

	new_header = addr;
	new_header->size = size - sizeof(t_header);
	new_header->is_free = 1;
	new_header->next = next_header;
}

void			append_page(size_t pagesize)
{
	//TODO append new page and link it to curr page
}

void			*allocate(size_t size, t_header *curr_header)
{
	size_t		curr_size;
	void		*curr_next;

	curr_size = curr_header->size;
	curr_next = curr_header->next;
	curr_header->size = size;
	curr_header->is_free = 0;
	curr_header->next = curr_header + sizeof(t_header) + size;
	build_header(curr_header->next, curr_size - size, curr_next);
	return (curr_header + sizeof(t_header));
}

void			*find_space(size_t size)
{
	t_header	*curr_header;

	curr_header = g_mem_start;
	while (curr_header)
	{
		if (curr_header->is_free && curr_header->size >= size)
			return (curr_header);
		curr_header = curr_header->next;
	}
	return (NULL);
}

void			*malloc(size_t size)
{
	printf("requested malloc of size : %zu\n", size);
	size_t		pagesize;
	t_header	*curr_header;
	void		*addr;

	curr_header = NULL;
	addr = NULL;
	pagesize = getpagesize();
	//init mem list
	if (g_mem_start == NULL)
		init_first_header(pagesize);
	//find free space in curr page
	curr_header = find_space(size);
	if (curr_header)
	{
		printf("found space : %p\n", curr_header);
		addr = allocate(size, curr_header);
		printf("allocated %zu at %p\n", size, addr);
	}
	//else
	//	//or append a new page
	//	append_page(pagesize);
	//	addr = allocate(addr, size);
	return addr;
}
