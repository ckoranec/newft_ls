/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseandstuff.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkertgat <jkertgat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/18 05:40:15 by jkertgat          #+#    #+#             */
/*   Updated: 2019/05/18 05:40:15 by jkertgat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/ft_ls.h"

int getargs(char *av, char *flags)
{
  if (*(av + 1) == '-')
    return (0);
  while (*(++av))
    {
      if (*av == 'r')
	*flags |= 0x1;
      else if (*av == 'R')
	*flags |= 0x2;
      else if (*av == 'l')
	*flags |= 0x4;
      else if (*av == 't')
	*flags |= 0x8;
      else if (*av == 'a')
	*flags |= 0x10;
      else
	return (-1);
    }
  return (1);
}

char ls_parse(char **av, t_node **dirlist)
{
  char flags;
  int ret;

  ret = 0;
  flags = 0;
  av++;
  while (*av && **av == '-')
    {
      if ((ret = getargs((*av), &flags)) < 1)
	break ;
      av++;
    }
  if (ret == -1)
    return (0);
  if (!(*av))
    addnode(dirlist, 0, ".");
  while (*av)
    {
      addnode(dirlist, 0, *av);
      av++;
    }
  dirlist = mergesort_list(dirlist, flags);
  return (flags);
}

bool checkthatshitnadd(t_node **dirlist, t_node **files)
{
  t_stat temp;

  if ((lstat((*dirlist)->fullname,
	     &temp) == 0) &&
      !(S_ISDIR(temp.st_mode)))
    addnode(files, (*dirlist)->fullname,
	    (*dirlist)->name);
  else
    {
      ft_printf("ft_ls: %s: %s\n", (*dirlist)->fullname, strerror(errno));
      if (ft_strcmp(strerror(errno), "Permission denied") == 0)
	return (false);
    }
  return (true);
}

void freeit(t_node **files)
{
  t_node *temp;

  while (*files)
    {
      temp = *files;
      *files = (*files)->next;
      temp->next = NULL;
      ft_strdel(&(temp->name));
      ft_strdel(&(temp->path));
      ft_strdel(&(temp->fullname));
      free(temp);
    }
}

void useadir(t_node **dirlist, char flags)
{
  DIR *dir;
  t_dirant *dp;
  t_node *files;
  t_node *copyofdirlist;

  copyofdirlist = *dirlist;
  files = 0;
  if (!(dir = opendir((*dirlist)->fullname)))
    {
      if (!checkthatshitnadd(dirlist, &files))
	return ;
    }
  else
    while ((dp = readdir(dir)) != NULL)
      addnode(&files, (*dirlist)->fullname, dp->d_name);
  mergesort_list(&files, flags);
  printit(&copyofdirlist, files, flags);
  freeit(&files);
  if (dir)
    closedir(dir);
}   
