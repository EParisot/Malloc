/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 11:39:01 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/15 17:55:28 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void				*fresh_allocate(void *ptr, t_header *curr_header, \
									size_t size, size_t pagesize)
{
	void			*addr;

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
	free(ptr);
	return (addr);
}

void				*realloc(void *ptr, size_t size)
{
	size_t			pagesize;
	t_header		*curr_header;
	void			*addr;

	pagesize = getpagesize();
	if (g_mem_start == NULL)
		return (NULL);
	curr_header = find_header(ptr);
	if (curr_header)
	{
		if (size <= curr_header->size)
		{
			addr = allocate(size, curr_header);
		}
		else
			addr = fresh_allocate(ptr, curr_header, size, pagesize);
	}
	else
		return (NULL);
	return (addr);
}
