/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 11:36:57 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/15 11:27:28 by eparisot         ###   ########.fr       */
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

t_header		*is_empty_page(void)
{
	t_header	*curr_header;
	int			curr_id;
	int			empty_flag;

	empty_flag = 1;
	curr_header = g_mem_start;
	while (curr_header->next)
		curr_header = curr_header->next;
	while (curr_header->type == 1)
	{
		curr_id = curr_header->page_id;
		if (curr_header->is_free == 0)
			empty_flag = 0;
		if (curr_header->prev->page_id != curr_id && empty_flag == 1)
			return (curr_header);
		else if (curr_header->prev->page_id != curr_id)
			empty_flag = 1;
		curr_header = curr_header->prev;
	}
	return (NULL);
}

void			clean_pages(void)
{
	t_header	*curr_header;
	t_header	*next_header;

	while ((curr_header = is_empty_page()))
	{
		next_header = get_next_page(curr_header);
		if (next_header)
			next_header->prev = curr_header->prev;
		curr_header->prev->next = next_header;
		if (munmap(curr_header, curr_header->size) != 0)
			ft_putstr("free Error\n");
	}
}

t_header		*is_empty_mem(void)
{
	t_header	*curr_header;

	curr_header = g_mem_start;
	while (curr_header->next)
	{
		if (curr_header->is_free == 0)
			return (NULL);
		curr_header = curr_header->next;
	}
	return (curr_header);
}

void			clean_mem(size_t pagesize)
{
	t_header	*last_header;

	if ((last_header = is_empty_mem()) && last_header->next == NULL)
	{
		if (munmap(last_header, 100 * pagesize) != 0)
			ft_putstr("free Error\n");
		if (munmap(g_mem_start, 3 * pagesize) != 0)
			ft_putstr("free Error\n");
		g_mem_start = NULL;
	}
}

void			free(void *ptr)
{
	size_t		pagesize;
	t_header	*curr_header;

	if (ptr == NULL)
		return;
	pagesize = getpagesize();
	curr_header = find_header(ptr);
	if (curr_header)
		deallocate(curr_header);
	else
		ft_putstr("Error : not allocated\n");
	clean_pages();
	clean_mem(pagesize);
}
