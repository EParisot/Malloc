/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/23 17:46:38 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/14 11:27:12 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void			init_memory(size_t pagesize)
{
	t_header	*first_header;
	t_header	*second_header;

	first_header = mmap(NULL, 3 * pagesize, PROT_READ | PROT_WRITE | PROT_EXEC,\
								MAP_ANON | MAP_PRIVATE, -1, 0);
	first_header->page_id = 0;
	first_header->type = 0;
	first_header->size = 3 * pagesize - sizeof(t_header);
	first_header->is_free = 1;
	first_header->prev = NULL;
	first_header->next = NULL;
	g_mem_start = first_header;
	printf("init first header at : %p with %zu\n", first_header, first_header->size);
	second_header = mmap(NULL, 100 * pagesize, PROT_READ | PROT_WRITE | \
								PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
	second_header->page_id = 1;
	second_header->type = 1;
	second_header->size = 100 * pagesize - sizeof(t_header);
	second_header->is_free = 1;
	second_header->prev = first_header;
	second_header->next = NULL;
	first_header->next = second_header;
	printf("init second header at : %p with %zu\n", second_header, second_header->size);
}

void			build_header(void *addr, void *curr_header, void *next_header)
{
	t_header	*new_header;

	new_header = addr;
	new_header->page_id = ((t_header*)addr)->page_id;
	new_header->type = ((t_header*)addr)->type;
	new_header->size = ((t_header*)addr)->size;
	new_header->is_free = 1;
	new_header->prev = curr_header;
	new_header->next = next_header;
}

t_header		*append_page(size_t pagesize, size_t size)
{
	t_header	*curr_header;
	t_header	*new_header;
	int			factor;

	factor = 1;
	//calc pages needed
	while (factor * pagesize < size + sizeof(t_header))
		++factor;
	//find last location
	curr_header = g_mem_start;
	while (curr_header->next)
		curr_header = curr_header->next;
	//append page
	new_header = mmap(NULL, factor * pagesize, PROT_READ | PROT_WRITE | \
								PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
	new_header->type = 2;
	new_header->page_id = curr_header->page_id + 1;
	new_header->size = factor * pagesize - sizeof(t_header);
	build_header(new_header, curr_header, NULL);
	curr_header->next = new_header;
	return (new_header);
}

void			*allocate(size_t size, t_header *curr_header)
{
	size_t		curr_size;
	t_header	*curr_next;

	//update curr
	curr_header->is_free = 0;
	//if space left after allocation
	if (curr_header->size >= size + sizeof(t_header) + TINY)
	{
		//build and append next header
		curr_size = curr_header->size;
		curr_next = curr_header->next;
		curr_header->size = size;
		curr_header->next = (void*)curr_header + sizeof(t_header) + size;
		curr_header->next->type = curr_header->type;
		curr_header->next->page_id = curr_header->page_id;
		curr_header->next->size = curr_size - size - sizeof(t_header);
		build_header(curr_header->next, curr_header, curr_next);
	}
	return ((void*)curr_header + sizeof(t_header));
}

t_header		*find_space(size_t size)
{
	t_header	*curr_header;
	int			curr_type;

	if (size < TINY)
		curr_type = 0;
	else if (size < LARGE)
		curr_type = 1;
	curr_header = g_mem_start;
	while (curr_header)
	{
		//if no TINY or SMALL, search in LARGES
		if (curr_header->type == 2)
			curr_type = 2;
		if (curr_header->is_free && curr_header->type == curr_type && \
			curr_header->size >= size + sizeof(t_header))
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
		init_memory(pagesize);
	//find free space in adapted page
	curr_header = find_space(size);
	if (size < LARGE && curr_header)
	{
		printf("found space : %p\n", curr_header);
		addr = allocate(size, curr_header);
		printf("allocated %zu at %p\n", size, addr);
	}
	//or append a new page
	else
	{
		printf("no space left, adding a page\n");
		curr_header = append_page(pagesize, size);
		addr = allocate(size, curr_header);
		printf("allocated %zu at %p\n", size, addr);
	}
	return addr;
}
