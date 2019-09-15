/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 11:43:43 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/15 11:58:27 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void			show_headers(t_header *curr_header)
{
	if (curr_header == g_mem_start || \
			(curr_header->prev && curr_header->prev->type != curr_header->type))
	{
		if (curr_header == g_mem_start || curr_header->type == 0)
		{
			ft_putstr("TINY : ");
			ft_putnbr_hex_p((uintmax_t)curr_header);
			ft_putchar('\n');
		}
		else if (curr_header->type == 1)
		{
			ft_putstr("SMALL : ");
			ft_putnbr_hex_p((uintmax_t)curr_header);
			ft_putchar('\n');
		}
		else if (curr_header->type == 2)
		{
			ft_putstr("LARGE : ");
			ft_putnbr_hex_p((uintmax_t)curr_header);
			ft_putchar('\n');
		}
	}
}

void			show_alloc_mem(void)
{
	t_header	*curr_header;
	size_t		tot;

	tot = 0;
	curr_header = g_mem_start;
	while (curr_header)
	{
		show_headers(curr_header);
		if (curr_header->is_free == 0)
		{
			ft_putnbr_hex_p((uintmax_t)(curr_header + sizeof(t_header)));
			ft_putstr(" - ");
			ft_putnbr_hex_p((uintmax_t)(curr_header + sizeof(t_header) + \
					curr_header->size));
			ft_putstr(" : ");
			ft_putnbr(curr_header->size);
			ft_putstr(" octets\n");
		}
		if (curr_header->is_free == 0)
			tot += curr_header->size;
		curr_header = curr_header->next;
	}
	ft_putstr("Total : ");
	ft_putnbr(tot);
	ft_putchar('\n');
}
