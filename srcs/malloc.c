/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/23 17:46:38 by eparisot          #+#    #+#             */
/*   Updated: 2019/08/24 17:50:50 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void				*malloc(size_t size)
{
	printf("requested malloc of size : %zu\n", size);
	size_t	pagesize;
	void	*addr;

	pagesize = getpagesize();
	addr = mmap(NULL, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC, \
								MAP_ANON | MAP_PRIVATE, 0, 0);
	if (addr == MAP_FAILED)
		return NULL;
	printf("pagesize : %zu\n", pagesize);
	printf("mallocated %zu at %p\n", size, addr);
	return addr;
}
