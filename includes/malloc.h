/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/23 17:48:14 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/15 00:33:56 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# define TINY 60
# define LARGE 4056

# include "../libft/libft.h"
# include <unistd.h>
# include <sys/mman.h>

typedef struct			s_header
{
	int					type;
	int					page_id;
	size_t				size;
	int					is_free;
	struct s_header		*prev;
	struct s_header		*next;
}						t_header;

t_header				*g_mem_start;

void					free(void *ptr);
void					*malloc(size_t size);
void					*realloc(void *ptr, size_t size);
void					show_alloc_mem(void);

#endif
