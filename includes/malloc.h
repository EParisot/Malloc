/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/23 17:48:14 by eparisot          #+#    #+#             */
/*   Updated: 2019/09/15 16:48:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# define TINY 60
# define LARGE 4056

# include "../libft/libft.h"
# include <unistd.h>
# include <sys/mman.h>
# include <errno.h>

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

t_header				*find_header(void *ptr);
t_header				*get_next_page(t_header *curr_header);
void					merge_chunks(t_header *header_l, t_header *header_r);
void					deallocate(t_header *curr_header);
void					free(void *ptr);
int						init_memory(size_t pagesize);
void					build_header(void *addr, void *curr_header, void *next_header);
t_header				*append_page(size_t pagesize, size_t size);
void					*allocate(size_t size, t_header *curr_header);
t_header				*find_space(size_t size);
void					*malloc(size_t size);
void					*realloc(void *ptr, size_t size);
void					show_alloc_mem(void);

#endif
