/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yinzhang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 13:36:12 by yinzhang          #+#    #+#             */
/*   Updated: 2019/05/01 22:13:03 by yinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "./includes/fillit.h"
#include <stdlib.h>

typedef struct 	s_tetris
{
	int array[9];
	struct	s_tetris *next;
}		t_tetris;

int touches(char *s);
int validsymbols(char *buff);
int isvalidpiece(char *buff);
int		xyator(int **array, char buff[22]);
t_tetris	*extractpiece(int fd, t_tetris **list);
void	free_grid(char **grid, int size);


void	ft_memdel(void **ap)
{
	if (ap)
	{
		free(*ap);
		*ap = NULL;
	}
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char const *s)
{
	int i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		ft_putchar(s[i]);
		i++;
	}
}

char	*ft_strcpy(char *dst, const char *src)
{
	int i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = src[i];
	return (dst);
}

void	ft_strdel(char **as)
{
	ft_memdel((void **)as);
}


void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		((char *)dst)[i] = ((char *)src)[i];
		i++;
	}
	return (dst);
}


size_t	ft_strlen(const char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

/*==================================================================*/

int		count_tetris(t_tetris *list)
{
	while (list->next != NULL)
		list = list->next;
	return(list->array[8]);
}

int		min_size(t_tetris *list)
{
	int		size;
	int		pieces;

	size = 2;
	pieces = (count_tetris(list) * 4);
	while (pieces > (size * size))
		size++;
	return (size);
}

char	*gen_line(int size)
{
	char	*line;
	int		i;

	i = 0;
	if (!(line = (char *)malloc(sizeof(char) * size + 1)))
		return (NULL);
	while (i < size)
	{
		line[i] = '.';
		i++;
	}
	line[i] = '\0';
	return (line);
}


char	**make_grid(int size)
{
	char 	**grid;
	int		i;
	char	*line;

	i = 0;
	if (!(grid = (char **)malloc(sizeof(char *) * size + 1)))
		return (NULL);
	while (i < size)
	{
		grid[i] = (char *)malloc(sizeof(char) * size + 1);
		line = gen_line(size);
		ft_strcpy(grid[i], line);
		free(line);
		i++;
	}
	grid[i] = NULL;
	return (grid);
}

void	print_grid(char **grid, int size)
{
	int i;

	i = 0;
	while (grid[i])
	{
		ft_putstr(grid[i]);
		ft_putchar('\n');
		i++;
	}
	free_grid(grid, size);
}

void	free_grid(char **grid, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		ft_strdel(&grid[i]);
		i++;
	}
	free(grid);
}
/*===============================================================*/

int		*x_shift(int *array, int x)
{
	int i;
	int size;

	size = 4;
	i = 0;
	while (size--)
	{
		array[i] = array[i] + x;
		i += 2;
	}
	return (array);
}

int		*y_shift(int *array, int y)
{
	int i;
	int size;

	size = 4;
	i = 1;
	while (size--)
	{
		array[i] = array[i] + y;
		i += 2;
	}
	return (array);
}

int		*shift_tet(int *array, int x, int y)
{
	array = x_shift(array, x);
	array = y_shift(array, y);
	return (array);
}

/*=============================================================*/

int		hi_y(int *tab)
{
	int size;
	int i;
	int y;

	i = 1;
	size = 3;
	y = tab[i];
	while (size--)
	{
		i += 2;
		if (y < tab[i])
			y = tab[i];
	}
	return (y);
}

int		hi_x(int *tab)
{
	int size;
	int i;
	int x;

	i = 0;
	size = 3;
	x = tab[i];
	while (size--)
	{
		i += 2;
		if (x < tab[i])
			x = tab[i];
	}
	return (x);
}

int		piece_collide(char **grid, int *piece)
{
	int size;
	int i;

	i = 0;
	size = 4;
	while (size--)
	{
		if (grid[piece[i + 1]][piece[i]] != '.')
			return (1);
		i += 2;
	}
	return (0);
}

int		box_collide(int *tet, int size)
{
	int x;
	int y;

	x = hi_x(tet);
	y = hi_y(tet);
	if (x >= size || y >= size)
		return (1);
	return (0);
}

int		collision(char **grid, int *tet, int size)
{
	return (!(box_collide(tet, size)) && !(piece_collide(grid, tet)));
}

/*=======================================================================*/

void	insert_piece(char **grid, int *tab, char c)
{
	int i;
	int x;
	int y;

	x = 0;
	y = 0;
	i = 0;
	while (i < 8)
	{
		y = tab[i + 1];
		x = tab[i];
		grid[y][x] = c;
		i += 2;
	}
}

void	clear_piece(char **grid, int *tab)
{
	int i;
	int x;
	int y;

	x = 0;
	y = 0;
	i = 0;
	while (i < 8)
	{
		y = tab[i + 1];
		x = tab[i];
		grid[y][x] = '.';
		i += 2;
	}
}
/*======================================================================*/

void	move(int *array, t_tetris *list)
{
	int i;
	i = 0;

	while (i < 9)
	{
		array[i] = list->array[i];
		i++;
	}
}

void cthing(char *c, int *array)
{
	*c = array[8] + 65;
}


int backtrack(char **grid, t_tetris *list, int size)
{
//	int array[9];
	int *array;
	char c;
	int x;
	int y;
	int i;

	i = 0;

	if (list == NULL)
		return (1);

	array = (int *)malloc(sizeof(int) * 9);	
	c = array[8] + 65;

	y = 0;
	while (y < size)
	{
		x = 0;
		while (x < size)
		{
			move(array, list);
			cthing(&c, array);
			shift_tet(array, x, y);
			if (collision(grid, array, size))
			{
				insert_piece(grid, array, c);
				if (backtrack(grid, list->next, size))
					return (1);
				clear_piece(grid, array);
			}
			x++;
		}
		y++;
	}
	return (0);
}




int		solver(t_tetris *list)
{
	char **grid;
	int size;

	size = min_size(list);
	grid = make_grid(size);

	while(!backtrack(grid, list, size))
	{
		free_grid(grid, size);
		size++;
		grid = make_grid(size);
	}
	print_grid(grid, size); 
	return (0);
}


int		main(int argc, char **argv)
{
	t_tetris *list;
	int i = 0;
	int fd;
	int size;

	if (argc == 2)
	{
		if ((fd = open(argv[1], O_RDONLY)) < 1)
			return (1);

		if ((list = extractpiece(fd, &list)) == NULL)
		{	
			printf("mr stark i'm not feeling so good");
			return (1);
		}
		solver(list);
		}
	else 
		write(1, "usage: enter 1 argument, please", 33);

	
	return (0);
}

/*=================================================================================*/

void	ft_lstappend(t_list **head, t_list *new)
{
	t_list	*temp;

	temp = *head;
	if (!*head)
		*head = new;
	else
	{
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

/*=================================================================================*/

t_tetris	*add_piece(int *ary)
{
	t_tetris *piece;
	int i;

	i = 0;
	piece = (t_tetris *)malloc(sizeof(t_tetris));
	while (i < 9)
	{
		piece->array[i] = ary[i];
		i++;
	}
	piece->next = NULL;
	return (piece);
}

t_tetris	*append(t_tetris *head, int *ary)
{
	t_tetris *headcpy;
	t_tetris *piece;

	headcpy = head;
	while (headcpy->next != NULL)
		headcpy = headcpy->next;
	piece = add_piece(ary); 
	headcpy->next = piece;
	return (head);
}

/*==================================================================================*/

void	reductiony(int **array)
{
	int i;
	int a;
	int b;
	int c;
	int d;

	i = 1;
	while (i <= 7)
	{
		if (i == 1)
			a = (*array)[i];
		if (i == 3)
			b = (*array)[i];
		if (i == 5)
			c = (*array)[i];
		if (i == 7)
			d = (*array)[i];
		i = i + 2;
	}
	while (a > 0 && b > 0 && c > 0 && d > 0)
	{
		a--;
		b--;
		c--;
		d--;
	}
	(*array)[1] = a;
	(*array)[3] = b;
	(*array)[5] = c;
	(*array)[7] = d;
}

void	reductionx(int **array)
{
	int i;
	int a;
	int b;
	int c;
	int d;

	i = 0;

	while (i <= 7)
	{ 
		if (i == 0)
			a = array[0][i];
		if (i == 2)
			b = array[0][i];
		if (i == 4)
			c = array[0][i];
		if (i == 6)
			d = array[0][i];
			i = i + 2;
	}
	while (a > 0 && b > 0 && c > 0 && d > 0)
	{
		a--;
		b--;
		c--;
		d--;
	} 
	array[0][0] = a;
	array[0][2] = b;
	array[0][4] = c;
	array[0][6] = d;
}


t_tetris	*extractpiece(int fd, t_tetris **list)
{
	char buff[22];
	int rd;
	int flag;
	int *array;
	int m;
	int i;
	t_tetris *head;
	t_tetris *helper;
   
	m = 0;
	head = NULL;
	while ((rd = read(fd, buff, 21)) >= 20)
	{	
		buff[21] = '\0';
		if (!isvalidpiece(buff))
			return (head); 
		if (rd == 20)
			flag = 1;
		array = (int *)malloc(sizeof(int) * 9);
		xyator(&array, buff);
		array[8] = m;
		m++;
		reductionx (&array);
		reductiony (&array);
		if (head == NULL)
			head = add_piece(array);
		else
			head = append(head, array); 
		free (array);
	}
	close(fd);
	return (head);
}



int		xyator(int **array, char buff[22])
{
	int x = 0;
	int i = 0;
	int l = 0;

	while (x != 4)
	{
		while (buff[i] != '\n')
		{
			if (buff[x * 5 + 4] != '\n')
				return (1);
			if (buff[i] == '#' && l <= 7)
			{
				(*array)[l++] = i % 5;
				(*array)[l++] =  x;
			}
			i++;
		}
		i++;
		x++;
	}
	return (0);
}

/*==================================================================================*/

/*isvalid functions*/

int touches(char *s)
{
	int i;
	int touchii;

	i= 0;
	touchii = 0;
	while (s[i])
	{
		if (s[i] == '#')
		{
			if (i < 20 && s[i + 1] == '#')
				touchii++;
			if (i > 0 && s[i - 1] == '#')
				touchii++;
			if (i <= 14 && s[i + 5] == '#')
				touchii++;
			if (i >= 5 && s[i - 5] == '#')
				touchii++;
		}
		i++;
	}
	if (touchii == 6 || touchii == 8)
		return (0);
	else
		return (1);
}


int		validsymbol(char *buff)
{
	int i;
	int hashtagnum;
	int dots;

	i = 0;
	hashtagnum = 0;
	dots = 0;

	while (i < ft_strlen(buff))
	{
		if (buff[i] != '.' || buff[i] != '#' || buff[i] != '\n')
			return (0);
		if ((i == 5 || i == 10 || i == 15 || i == 20 || i == 21) && buff[i] != '\n')
			return (0);
		if (buff[i] == '.')
				dots++;
		if (buff[i] == '#')
			hashtagnum++;
		i++;
	}
	if (hashtagnum == 4 && (dots + hashtagnum == 16))
		return (0);
	else
		return (1);
}


int		isvalidpiece(char *buff)
{
	if (validsymbol(buff) && touches(buff))
		return (0);
	else
		return (1);
}
