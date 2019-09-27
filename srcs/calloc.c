/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 15:45:05 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/27 16:39:45 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void			*calloc(size_t nmemb, size_t size)
{
	void		*addr;
	size_t		tot_size;

	tot_size = nmemb * size;
	addr = malloc(tot_size);
	addr = ft_memset(addr, 0, tot_size);
	return (addr);
}
