# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/10/30 19:40:33 by eparisot          #+#    #+#              #
#    Updated: 2019/09/15 16:49:42 by eparisot         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME	=	libft_malloc_$(HOSTTYPE).so
LNK		=	libft_malloc.so

SRCS	=	srcs/free.c \
			srcs/free_utils.c \
			srcs/malloc_utils.c \
			srcs/malloc.c \
			srcs/realloc.c \
			srcs/show_alloc_mem.c
INC		=	includes/malloc.h

OBJS	=	$(SRCS:.c=.o)
LIBS	=	libft/libft.a \

CFLAGS	=	-Wall -Wextra -Werror
LDFLAGS =	-fPIC -shared

RM		=	rm -f

all		:	$(LIBS) $(NAME) $(LNK)

$(LNK)	:	$(NAME)
	[ -L $(LNK) ] || (ln -s $(NAME) $(LNK))

$(NAME)	:	$(OBJS) $(INC)
	gcc $(CFLAGS) ${LDFLAGS} $(OBJS) $(LIBS) -o $(NAME)

$(LIBS)	:
	@$(MAKE) -C libft

clean	:
	$(RM) $(OBJS) && $(MAKE) clean -C libft

fclean	:	clean
	$(RM) $(NAME) $(LNK) libft/libft.a

re		:	fclean all
