/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 11:36:57 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/18 11:37:38 by eparisot         ###   ########.fr       */
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
	size_t		curr_type;

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

void			deallocate(t_header *curr_header)
{
	size_t		curr_type;

	curr_type = curr_header->type;
	curr_header->is_free = 1;
}
