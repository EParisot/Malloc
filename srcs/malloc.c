/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/23 17:46:38 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/15 18:30:43 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

t_header		*append_tiny(size_t pagesize)
{
	t_header	*curr_header;
	t_header	*new_header;
	int			factor;
	int			type;

	type = 0;
	factor = 3;
	curr_header = g_mem_start;
	while (curr_header->next && curr_header->next->type < 1)
		curr_header = curr_header->next;
	if ((new_header = mmap(NULL, factor * pagesize, PROT_READ | PROT_WRITE | \
					PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
	new_header->type = type;
	new_header->page_id = curr_header->page_id + 1;
	new_header->size = factor * pagesize - sizeof(t_header);
	build_header(new_header, curr_header, curr_header->next);
	curr_header->next = new_header;
	return (new_header);
}

t_header		*append_small(size_t pagesize)
{
	t_header	*curr_header;
	t_header	*new_header;
	int			factor;
	int			type;

	type = 1;
	factor = 100;
	curr_header = g_mem_start;
	while (curr_header->next && curr_header->next->type < 2)
		curr_header = curr_header->next;
	if ((new_header = mmap(NULL, factor * pagesize, PROT_READ | PROT_WRITE | \
					PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
	new_header->type = type;
	new_header->page_id = curr_header->page_id + 1;
	new_header->size = factor * pagesize - sizeof(t_header);
	build_header(new_header, curr_header, curr_header->next);
	curr_header->next = new_header;
	return (new_header);
}

t_header		*append_large(size_t pagesize, size_t size)
{
	t_header	*curr_header;
	t_header	*new_header;
	int			factor;
	int			type;

	type = 2;
	factor = 1;
	while (factor * pagesize < size + sizeof(t_header))
		++factor;
	curr_header = g_mem_start;
	while (curr_header->next)
		curr_header = curr_header->next;
	if ((new_header = mmap(NULL, factor * pagesize, PROT_READ | PROT_WRITE | \
					PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
	new_header->type = type;
	new_header->page_id = curr_header->page_id + 1;
	new_header->size = factor * pagesize - sizeof(t_header);
	build_header(new_header, curr_header, curr_header->next);
	curr_header->next = new_header;
	return (new_header);
}

t_header		*append_page(size_t pagesize, size_t size)
{
	t_header	*new_header;

	if (size < TINY)
		new_header = append_tiny(pagesize);
	else if (size < LARGE)
		new_header = append_small(pagesize);
	else
		new_header = append_large(pagesize, size);
	return (new_header);
}

void			*malloc(size_t size)
{
	size_t		pagesize;
	t_header	*curr_header;
	void		*addr;
	int			ret;

	pagesize = getpagesize();
	if (g_mem_start == NULL)
		if ((ret = init_memory(pagesize)) < 0)
		{
			if (ret == -2)
				free(g_mem_start + sizeof(t_header));
			errno = ENOMEM;
			return (NULL);
		}
	curr_header = find_space(size);
	if (size < LARGE && curr_header)
		addr = allocate(size, curr_header);
	else
	{
		if (!(curr_header = append_page(pagesize, size)))
		{
			errno = ENOMEM;
			return (NULL);
		}
		addr = allocate(size, curr_header);
	}
	return (addr);
}
