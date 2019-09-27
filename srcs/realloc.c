/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 11:39:01 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/27 16:50:26 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void				*fresh_allocate(void *ptr, t_header *curr_header, \
								size_t size, size_t ptr_size)
{
	void			*addr;
	size_t			pagesize;

	addr = NULL;
	pagesize = getpagesize();
	curr_header = find_space(size);
	if (size < LARGE && curr_header)
	{
		addr = allocate(size, curr_header);
		ft_memmove(addr, ptr, ptr_size);
		free(ptr);
	}
	else
	{
		if (!(curr_header = append_page(pagesize, size)))
		{
			errno = ENOMEM;
			return (NULL);
		}
		addr = allocate(size, curr_header);
		ft_memmove(addr, ptr, ptr_size);
		free(ptr);
	}
	return (addr);
}

void				*realloc(void *ptr, size_t size)
{
	t_header		*curr_header;
	void			*addr;
	size_t			ptr_size;

	if (g_mem_start == NULL)
		return (NULL);
	curr_header = find_header(ptr);
	while (size % 16)
		++size;
	if (curr_header)
	{
		ptr_size = curr_header->size;
		if (size <= curr_header->size)
			addr = allocate(size, curr_header);
		else
			addr = fresh_allocate(ptr, curr_header, size, ptr_size);
	}
	else if (ptr == NULL)
		addr = malloc(size);
	else
		return (NULL);
	return (addr);
}
