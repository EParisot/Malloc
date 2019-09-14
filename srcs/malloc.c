/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/23 17:46:38 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/14 20:00:42 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

int				init_memory(size_t pagesize)
{
	t_header	*first_header;
	t_header	*second_header;

	if ((first_header = mmap(NULL, 3 * pagesize, PROT_READ | PROT_WRITE | \
					PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (-1);
	first_header->type = 0;
	first_header->size = 3 * pagesize - sizeof(t_header);
	first_header->is_free = 1;
	first_header->prev = NULL;
	first_header->next = NULL;
	g_mem_start = first_header;
	if ((second_header = mmap(NULL, 100 * pagesize, PROT_READ | PROT_WRITE | \
					PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (-1);
	second_header->type = 1;
	second_header->size = 100 * pagesize - sizeof(t_header);
	second_header->is_free = 1;
	second_header->prev = first_header;
	second_header->next = NULL;
	first_header->next = second_header;
	return (0);
}

void			build_header(void *addr, void *curr_header, void *next_header)
{
	t_header	*new_header;

	new_header = addr;
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
	int			type;

	if (size < TINY)
	{
		type = 0;
		factor = 3;
		curr_header = g_mem_start;
		while (curr_header->next->type < 1)
			curr_header = curr_header->next;
	}
	else if (size < LARGE)
	{
		type = 1;
		factor = 100;
		curr_header = g_mem_start;
		while (curr_header->next->type < 2)
			curr_header = curr_header->next;
	}
	else
	{
		type = 2;
		factor = 1;
		while (factor * pagesize < size + sizeof(t_header))
			++factor;
		curr_header = g_mem_start;
		while (curr_header->next)
			curr_header = curr_header->next;
	}
	if ((new_header = mmap(NULL, factor * pagesize, PROT_READ | PROT_WRITE | \
					PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
	new_header->type = type;
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
	else
		curr_type = 2;
	curr_header = g_mem_start;
	while (curr_header->type <= curr_type)
	{
		if (curr_header->is_free && curr_header->type == curr_type && \
				curr_header->size >= size)
			return (curr_header);
		curr_header = curr_header->next;
	}
	return (NULL);
}

void			*malloc(size_t size)
{
	size_t		pagesize;
	t_header	*curr_header;
	void		*addr;

	curr_header = NULL;
	addr = NULL;
	pagesize = getpagesize();
	//init mem list
	if (g_mem_start == NULL)
		if (init_memory(pagesize) < 0)
			free(g_mem_start + sizeof(t_header));
	//find free space in adapted page
	curr_header = find_space(size);
	if (size < LARGE && curr_header)
		addr = allocate(size, curr_header);
	//or append a new page
	else
	{
		if (!(curr_header = append_page(pagesize, size)))
			return (NULL);
		addr = allocate(size, curr_header);
	}
	return (addr);
}
