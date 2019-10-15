/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/23 17:48:14 by eparisot          #+#    #+#             */
/*   Updated: 2019/10/15 14:45:36 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# define TINY 60
# define T_NB 3
# define S_NB 60
# define LARGE 2048 - sizeof(t_header)

# include "../libft/libft.h"
# include <unistd.h>
# include <sys/mman.h>
# include <errno.h>

typedef struct			s_header
{
	size_t				type;
	size_t				page_id;
	size_t				size;
	struct s_header		*prev;
	struct s_header		*next;
	int					is_free;
	char*				align[4];
}						t_header;

t_header				*g_mem_start;

t_header			*find_header(void *ptr);
t_header			*get_next_page(t_header *curr_header);
void				deallocate(t_header *curr_header);
void				free(void *ptr);
int					init_memory(size_t pagesize);
t_header			*append_page(size_t pagesize, size_t size);
void				build_header(void *addr, void *curr_header, \
								void *next_header);
void				*allocate(size_t size, t_header *curr_header);
t_header			*find_space(size_t size);
void				*malloc(size_t size);
void				*realloc(void *ptr, size_t size);
void				show_alloc_mem(void);
void				*calloc(size_t nmemb, size_t size);

#endif
