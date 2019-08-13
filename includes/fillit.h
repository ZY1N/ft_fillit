/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akikalia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 01:49:10 by akikalia          #+#    #+#             */
/*   Updated: 2019/04/18 13:14:59 by yinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILLIT_H
# define FILLIT_H
# include "libft.h"
# include <fcntl.h>

typedef struct		s_point
{
	int				i;
	int				n;
}					t_point;

int					usage(void);
int					check_bounds(char bl[5][5], int i, int n, int *blnum);
int					test_input(char block[5][5]);
void				ft_lstappend(t_list **head, t_list *new);
int					split_test(int **arr, char buf[22], int l, int i);
int					get_input(int fd, t_list **list);
void				create_board(char ***str, int size);
int					add(t_list *list, char ***str, t_point p, int size);
void				rem(t_list *list, char ***str, t_point p);

void				del(void *content, size_t content_size);
int					error_output(t_list **list);
void				free_arr(char ***arr);
void				print_res(char **res);

#endif
