/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 11:36:57 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/13 12:26:42 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

t_header		*find_header(void *ptr)
{
	t_header	*curr_header;

	curr_header = g_mem_start;
	while (curr_header)
	{
		if (curr_header->is_free == 0 && \
			(void*)curr_header + sizeof(t_header) == ptr)
			return (curr_header);
		curr_header = curr_header->next;
	}
	return (NULL);
}

void			deallocate(t_header *curr_header)
{
	curr_header->is_free = 1;
	printf("free %zu at %p\n", curr_header->size, (void*)curr_header + sizeof(t_header));
	//TODO defragmentation
}

int				is_empty()
{
	t_header	*curr_header;

	curr_header = g_mem_start;
	while (curr_header)
	{
		if (curr_header->is_free == 0)
			return (0);
		curr_header = curr_header->next;
	}
	return (1);
}

void			free(void *ptr)
{
	printf("requested free at : %p\n", ptr);
	size_t			pagesize;
	t_header		*curr_header;

	pagesize = getpagesize();
	curr_header = find_header(ptr);
	if (curr_header)
		deallocate(curr_header);
	else
		ft_putstr("Error : not allocated\n");
	if (is_empty())
	{
		if(munmap(g_mem_start, pagesize) != 0)
			ft_putstr("free Error");
		printf("cleaned mem\n");
	}
}
