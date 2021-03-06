/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkertgat <jkertgat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/18 05:48:00 by jkertgat          #+#    #+#             */
/*   Updated: 2019/05/18 05:48:00 by jkertgat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

void	printftype(t_node *files)
{
	char	t;

	t = '-';
	t = S_ISDIR(files->data.st_mode) ? 'd' : t;
	t = S_ISSOCK(files->data.st_mode) ? 's' : t;
	t = S_ISBLK(files->data.st_mode) ? 'b' : t;
	t = S_ISFIFO(files->data.st_mode) ? 'p' : t;
	t = S_ISLNK(files->data.st_mode) ? 'l' : t;
	t = S_ISCHR(files->data.st_mode) ? 'c' : t;
	ft_printf("%c", t);
}

void	printpermissions(t_node *files)
{
	printftype(files);
	ft_printf((files->data.st_mode & S_IRUSR) ? "r" : "-");
	ft_printf((files->data.st_mode & S_IWUSR) ? "w" : "-");
	if (files->data.st_mode & S_ISUID)
		ft_printf((files->data.st_mode & S_IXUSR) ? "S" : "s");
	else
		ft_printf((files->data.st_mode & S_IXUSR) ? "x" : "-");
	ft_printf((files->data.st_mode & S_IRGRP) ? "r" : "-");
	ft_printf((files->data.st_mode & S_IWGRP) ? "w" : "-");
	if (files->data.st_mode & S_ISGID)
		ft_printf((files->data.st_mode & S_IXGRP) ? "S" : "s");
	else
		ft_printf((files->data.st_mode & S_IXGRP) ? "x" : "-");
	ft_printf((files->data.st_mode & S_IROTH) ? "r" : "-");
	ft_printf((files->data.st_mode & S_IWOTH) ? "w" : "-");
	if (files->data.st_mode & S_ISVTX)
		ft_printf((files->data.st_mode & S_IXOTH) ? "t" : "T");
	else
		ft_printf((files->data.st_mode & S_IXOTH) ? "x" : "-");
}

void	printxattr(t_node *files)
{
	acl_t		acl;
	acl_entry_t	entry;

	acl = acl_get_link_np(files->fullname, ACL_TYPE_EXTENDED);
	if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &entry) == -1)
	{
		acl_free(acl);
		acl = 0;
	}
	if (listxattr(files->fullname, 0, 0, XATTR_NOFOLLOW) > 0)
		ft_printf("@ ");
	else if (acl != 0)
		ft_printf("+ ");
	else
		ft_printf("  ");
	if (acl)
		acl_free(acl);
}
