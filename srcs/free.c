/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 11:36:57 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/13 15:32:29 by eparisot         ###   ########.fr       */
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

t_header		*is_empty()
{
	t_header	*curr_header;
	int			curr_id;
	int			empty_flag;

	curr_id = 0;
	empty_flag = 1;
	if (g_mem_start == NULL)
		return (NULL);
	curr_header = g_mem_start;
	//go to end of mem
	while (curr_header->next)
	{
		curr_header = curr_header->next;
		curr_id = curr_header->page_id;
	}
	//backward check if page is empty
	while (curr_header)
	{
		curr_id = curr_header->page_id;
		if (curr_header->is_free == 0)
			empty_flag = 0;
		if ((curr_header->prev == NULL || curr_header->prev->page_id != curr_id) && empty_flag == 1)
			return (curr_header);
		else if (curr_header->prev == NULL || curr_header->page_id != curr_id)
			empty_flag = 1;

		curr_header = curr_header->prev;
	}
	return (NULL);
}

t_header		*get_next_page(t_header *curr_header)
{
	int			curr_id;

	curr_id = 0;
	while (curr_header)
	{
		if (curr_header->page_id != curr_id)
			return (curr_header);
		curr_id = curr_header->page_id;
		curr_header = curr_header->next;
	}
	return (NULL);
}

void			free(void *ptr)
{
	printf("requested free at : %p\n", ptr);
	size_t		pagesize;
	t_header	*curr_header;
	t_header	*next_header;
	int			empty_page;

	pagesize = getpagesize();
	//find mem zone
	curr_header = find_header(ptr);
	if (curr_header)
		deallocate(curr_header);
	else
		ft_putstr("Error : not allocated\n");
	//clean
	while ((curr_header = is_empty()))
	{
		// find next page and link next to prev
		next_header = get_next_page(curr_header);
		if (next_header)
			next_header->prev = curr_header->prev;
		if (curr_header->prev)
			curr_header->prev->next = next_header;
		// and destroy curr page
		printf("cleaned page %d\n", curr_header->page_id);
		if (curr_header->page_id == 0)
			while (g_mem_start && g_mem_start->page_id == 0)
				g_mem_start = g_mem_start->next;
		if(munmap(curr_header, pagesize) != 0)
			ft_putstr("free Error");
	}
}
