/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 11:36:57 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/18 17:13:34 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

t_header		*is_empty_page(void)
{
	t_header	*curr_header;
	size_t			curr_id;
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
		if (munmap((void*)curr_header, curr_header->size + sizeof(t_header)))
			ft_putstr("free Error\n");
	}
}

t_header		*is_empty_mem(void)
{
	t_header	*curr_header;

	curr_header = g_mem_start;
	while (curr_header->next && curr_header->next->type < 2)
	{
		if (curr_header->page_id > 0 && curr_header->is_free == 0)
			return (NULL);
		curr_header = curr_header->next;
	}
	while (curr_header->prev && curr_header->prev->type == curr_header->type &&\
			curr_header->prev->page_id == curr_header->page_id)
		curr_header = curr_header->prev;
	if (curr_header->page_id != 0)
		return (curr_header);
	return (NULL);
}

void			clean_mem(size_t pagesize)
{
	t_header	*last_header;

	while ((last_header = is_empty_mem()))
	{
		if (last_header->type == 1)
		{
			last_header->prev->next = NULL;
			if (munmap((void*)last_header, 100 * pagesize))
				ft_putstr("free Error\n");
			continue ;
		}
		else if (last_header->type == 0)
		{
			if (last_header->prev)
				last_header->prev->next = NULL;
			if (munmap((void*)last_header, 3 * pagesize))
				ft_putstr("free Error\n");
		}
		if (last_header == g_mem_start)
		{
			g_mem_start = NULL;
			break ;
		}
	}
}

void			free(void *ptr)
{
	size_t		pagesize;
	t_header	*curr_header;

	pagesize = getpagesize();
	if (ptr == NULL)
		return ;
	curr_header = find_header(ptr);
	if (curr_header)
	{
		deallocate(curr_header);
		if (curr_header->type == 2)
			clean_pages();
		clean_mem(pagesize);
	}
}
