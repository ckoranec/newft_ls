/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkertgat <jkertgat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/18 05:14:35 by jkertgat          #+#    #+#             */
/*   Updated: 2019/05/18 06:03:20 by jkertgat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/ft_ls.h"

void addtodirs(t_node **dirlist, t_node *files)
{
	t_node *new;

	if (!(new = ft_memalloc(sizeof(t_node))))
		return ;
	new->name = ft_strdup(files->name);
	new->path = ft_strdup(files->path);
	new->fullname = ft_strdup(files->fullname);
	lstat(new->fullname, &(new->data));
	new->isdir = (S_ISDIR(new->data.st_mode)) ? true : false;
	new->next = (*dirlist)->next;
	(*dirlist)->next = new;
	*dirlist = new;
}

char *makepath(char *s1, char *s2)
{
	char *ret;
	char *str;

	if (!s2)
		return (0);
	if (!(str = ft_memalloc(sizeof(char) * ft_strlen(s1) + ft_strlen(s2) + 2)))
		return (0);
	ret = str;
	while (s1 && *s1)
		*(str++) = *(s1++);
	if (s1 && *(s1 - 1) != '/')
		*(str++) = '/';
	while (s2 && *s2)
		*(str++) = *(s2++);
	return (ret);
}

void addnode(t_node **head, char *path, char *name)
{
	t_node *new;

	if (!(new = ft_memalloc(sizeof(t_node))))
		return ;
	new->name = ft_strdup(name);
	new->path = ft_strdup(path);
	new->fullname = makepath(path, name);
	lstat(new->fullname, &(new->data));
	new->isdir = (S_ISDIR(new->data.st_mode)) ? true : false;
	new->next = *head;
	*head = new;
}

void usedirs(t_node **dirlist, char flags)
{
	t_node *cpy;

	cpy = *dirlist;
	while (cpy)
		{
	ft_printf("%s:\n", cpy->fullname);
			useadir(&cpy, flags);
			cpy = cpy->next;
			if (cpy)
	ft_printf("\n");
		}
}

int main(int ac, char **av)
{
	char flags;
	t_node *dirlist;

	dirlist = 0;
	if ((flags = ls_parse(av, &dirlist)) == -1 && ac)
		return (0);
	usedirs(&dirlist, flags);
	freeit(&dirlist);
	while(1)
		;
	return (0);
}
