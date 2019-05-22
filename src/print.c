/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkertgat <jkertgat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/18 05:44:30 by jkertgat          #+#    #+#             */
/*   Updated: 2019/05/18 06:35:33 by jkertgat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/ft_ls.h"

void printblocksize(t_node *file, char flags)
{
  size_t size;
  int lstsize;

  lstsize = 0;
  size = 0;
  while (file)
    {
      if (!(flags & 0x10) && (file->name)[0] == '.')
	{
	  file = file->next;
	  continue ;
	}
      if (file)
	{
	  lstsize += 1;
	  size += file->data.st_blocks;
	  file = file->next;
	}
    }
  if (lstsize > 1)
    ft_printf("total %zu\n", size);
}

void printit(t_node **dirlist, t_node *files, char flags)
{
  printblocksize(files, flags);
  while (files)
    {
      if (!(flags & 0x10) && (files->name)[0] == '.')
	{
	  files = files->next;
	  continue ;
	}
      if ((flags & 0x2) && files->isdir &&
	  ft_strcmp(files->name, ".") != 0 &&
	  ft_strcmp(files->name, "..") != 0)
	addtodirs(dirlist, files);
      if (flags & 0x4)
	printls(files);
      else
	ft_printf("%s\n", files->name);
      files = files->next;
    }
}

void print_time(t_node *file)
{
  char *modtime;

  modtime = ctime(&(file->data.st_mtime));
  if (((time(NULL) - file->data.st_mtime) >= 15724800) ||
      ((time(NULL) - file->data.st_mtime) <= -15724800))
    {
         write(1, modtime + 4, 7);
  write(1, modtime + 20, 4);
    }
  else
 {
write(1, modtime + 4, 12);
  //  ft_printf("%.6s ", (modtime + 4));
 }
 write(1, " ",1);
}

void print_size(t_node *files)
{
  if (S_ISBLK(files->data.st_mode) || S_ISCHR(files->data.st_mode))
    ft_printf(" %d, %d ", major(files->data.st_rdev),
	      minor(files->data.st_rdev));
  else
    ft_printf(" %7lld ", files->data.st_size);
}

void printls(t_node *files)
{
  struct group *grp;
  struct passwd *pwd;
  char buf[_POSIX_SYMLINK_MAX + 1];

  if (ft_strcmp(files->name, "") == 0)
    return ;
  grp = getgrgid(files->data.st_gid);
  pwd = getpwuid(files->data.st_uid);
  printpermissions(files);
  printxattr(files);
  ft_printf("%3hu ", files->data.st_nlink);
  ft_printf("%-5s %-2s", pwd->pw_name, grp->gr_name);
  print_size(files);
  print_time(files);
  if (S_ISLNK(files->data.st_mode))
    {
      ft_bzero(buf, _POSIX_SYMLINK_MAX + 1);
      if (readlink(files->fullname, buf, _POSIX_SYMLINK_MAX) > 0)
	        ft_printf("%s -> %s\n", files->name, buf);
    }
  else
    ft_printf("%s\n", files->name);
}
