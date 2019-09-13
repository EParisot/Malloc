/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 11:43:43 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/13 12:21:26 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void			show_alloc_mem(void)
{
	t_header	*curr_header;
	void		*curr_pos;
	int			i;


	curr_header = g_mem_start;
	while (curr_header)
	{
		printf("header addr : %p, type : %d, is_free : %d, data size : %zu\n", \
		curr_header, curr_header->type, curr_header->is_free, curr_header->size);
		curr_pos = (void*)curr_header + sizeof(t_header);
		if (curr_header->is_free == 0)
		{
			i = 0;
			while (i < curr_header->size)
			{
				printf("%p : %s\n", curr_pos, curr_pos);
				++curr_pos;
				++i;
			}
		}
		curr_header = curr_header->next;
	}

}
