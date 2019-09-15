/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 11:36:57 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/15 16:50:25 by eparisot         ###   ########.fr       */
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
