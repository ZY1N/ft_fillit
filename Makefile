# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akikalia <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/26 02:23:08 by akikalia          #+#    #+#              #
#    Updated: 2019/03/06 15:48:55 by akikalia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := gcc
CFLAGS := -Wall -Wextra -Werror
NAME = fillit
SRCS = main.c input.c solve.c output.c
LIB = ./libft/ft_lstdel.c ./libft/ft_lstnew.c ./libft/ft_putchar.c \
	./libft/ft_putstr.c ./libft/ft_strnew.c \
	./libft/ft_lstdelone.c  ./libft/ft_memcpy.c \

all: $(NAME)
$(NAME):
	$(CC) $(CFLAGS) -I./includes/ $(LIB) $(SRCS) -o $(NAME)
clean:
fclean: clean
	/bin/rm -f $(NAME)
re: fclean all
