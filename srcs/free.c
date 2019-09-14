/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 11:36:57 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/14 19:54:18 by eparisot         ###   ########.fr       */
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

t_header		*is_empty_page()
{
	t_header	*curr_header;
	int			curr_type;
	int			empty_flag;

	curr_type = 0;
	empty_flag = 1;
	curr_header = g_mem_start;
	//go to end of mem
	while (curr_header->next)
	{
		curr_header = curr_header->next;
		curr_type = curr_header->type;
	}
	//backward check if page is empty
	while (curr_header)
	{
		curr_type = curr_header->type;
		if (curr_header->is_free == 0)
			empty_flag = 0;
		if ((curr_header->prev == NULL || curr_header->prev->type != curr_type) && empty_flag == 1)
			return (curr_header);
		else if (curr_header->prev == NULL || curr_header->prev->type != curr_type)
			empty_flag = 1;
		curr_header = curr_header->prev;
	}
	return (NULL);
}

t_header		*is_empty_mem()
{
	t_header	*curr_header;
	t_header	*last_page_header;

	curr_header = g_mem_start;
	while (curr_header)
	{
		if (curr_header->is_free == 0)
			return (NULL);
		curr_header = curr_header->next;
	}
	last_page_header = g_mem_start;
	while (last_page_header->type != 1)
		last_page_header = last_page_header->next;
	return (last_page_header);
}

t_header		*get_next_page(t_header *curr_header)
{
	int			curr_type;

	curr_type = curr_header->type;
	while (curr_header)
	{
		if (curr_header->type != curr_type)
			return (curr_header);
		curr_type = curr_header->type;
		curr_header = curr_header->next;
	}
	return (NULL);
}

void			merge_chunks(t_header *header_l, t_header *header_r)
{
	header_l->size += sizeof(t_header) + header_r->size;
	header_l->next = header_r->next;
	if (header_r->next)
		header_r->next->prev = header_l;
}

void			deallocate(t_header *curr_header)
{
	int			curr_type;

	curr_type = curr_header->type;
	curr_header->is_free = 1;
	//defragmentation
	while (curr_header->prev && curr_header->prev->type == curr_type && \
			curr_header->prev->is_free == 1)
	{
		merge_chunks(curr_header->prev, curr_header);
		curr_header = curr_header->prev;
	}
	while (curr_header->next && curr_header->next->type == curr_type && \
			curr_header->next->is_free == 1)
	{
		merge_chunks(curr_header, curr_header->next);
	}
}

void			clean_pages()
{
	t_header	*curr_header;
	t_header	*next_header;

	while ((curr_header = is_empty_page()) && curr_header->type > 1)
	{
		// find next page and link next to prev
		next_header = get_next_page(curr_header);
		if (next_header)
			next_header->prev = curr_header->prev;
		curr_header->prev->next = next_header;
		// and destroy curr page
		if(munmap(curr_header, curr_header->size) != 0)
			ft_putstr("free Error");
	}
}

void			clean_mem(size_t pagesize)
{
	t_header	*last_header;

	if ((last_header = is_empty_mem()))
	{
		if(munmap(last_header, 100 * pagesize) != 0)
			ft_putstr("free Error");
		if(munmap(g_mem_start, 3 * pagesize) != 0)
			ft_putstr("free Error");
	}
}

void			free(void *ptr)
{
	size_t		pagesize;
	t_header	*curr_header;

	pagesize = getpagesize();
	//find mem zone
	curr_header = find_header(ptr);
	if (curr_header)
		deallocate(curr_header);
	else
		ft_putstr("Error : not allocated\n");
	clean_pages();
	//clean tiny and small
	clean_mem(pagesize);
}
