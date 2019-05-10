#include "../includes/ft_ls.h"

void	printftype(t_node *files)
{
	char			t;

	t = '-';
	t = S_ISDIR(files->data.st_mode) ? 'd' : t;
	t = S_ISSOCK(files->data.st_mode) ? 's' : t;
	t = S_ISBLK(files->data.st_mode) ? 'b' : t;
	t = S_ISFIFO(files->data.st_mode) ? 'p' : t;
	t = S_ISLNK(files->data.st_mode) ? 'l' : t;
	t = S_ISCHR(files->data.st_mode) ? 'c' : t;
	printf("%c", t);
}

void	printpermissions(t_node *files)
{
	printftype(files);
	printf((files->data.st_mode & S_IRUSR) ? "r" : "-");
	printf((files->data.st_mode & S_IWUSR) ? "w" : "-");
	if (files->data.st_mode & S_ISUID)
		printf((files->data.st_mode & S_IXUSR) ? "S" : "s");
	else
		printf((files->data.st_mode & S_IXUSR) ? "x" : "-");
	printf((files->data.st_mode & S_IRGRP) ? "r" : "-");
	printf((files->data.st_mode & S_IWGRP) ? "w" : "-");
	if (files->data.st_mode & S_ISGID)
		printf((files->data.st_mode & S_IXGRP) ? "S" : "s");
	else
		printf((files->data.st_mode & S_IXGRP) ? "x" : "-");
	printf((files->data.st_mode & S_IROTH) ? "r" : "-");
	printf((files->data.st_mode & S_IWOTH) ? "w" : "-");
	if (files->data.st_mode & S_ISVTX)
		printf((files->data.st_mode & S_IXOTH) ? "t" : "T");
	else
		printf((files->data.st_mode & S_IXOTH) ? "x" : "-");
}

void	printxattr(t_node *files)
{
	acl_t				acl;
	acl_entry_t			entry;

	acl = acl_get_link_np(files->fullname, ACL_TYPE_EXTENDED);
	if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &entry) == -1)
	{
		acl_free(acl);
		acl = 0;
	}
	if (listxattr(files->fullname, 0, 0, XATTR_NOFOLLOW) > 0)
		printf("@ ");
	else if (acl != 0)
		printf("+ ");
	else
		printf("  ");
	if (acl)
		acl_free(acl);
}

void			print_time(t_node *file)
{
	char		*modtime;

	modtime = ctime(&(file->data.st_mtime));
	if (((time(NULL) - file->data.st_mtime) >= 15724800) ||
		((time(NULL) - file->data.st_mtime) <= -15724800))
		printf("%.6s  %.4s ", (modtime + 4), (modtime + 20));
	else
		printf("%.6s %.5s ", (modtime + 4), (modtime + 11));
}

void	print_size(t_node *files)
{
	if (S_ISBLK(files->data.st_mode) || S_ISCHR(files->data.st_mode))
		printf(" %d, %d ", major(files->data.st_rdev), minor(files->data.st_rdev));
	else
		printf(" %7lld ", files->data.st_size);
}

void	printl(t_node *files)
{
	struct group	*grp;
	struct passwd	*pwd;
	char			buf[_POSIX_SYMLINK_MAX + 1];

	if (ft_strcmp(files->name, "") == 0)
		return ;
	grp = getgrgid(files->data.st_gid);
	pwd = getpwuid(files->data.st_uid);
	printpermissions(files);
	printxattr(files);
	printf("%3hu ", files->data.st_nlink);
	printf("%-10s %-7s", pwd->pw_name, grp->gr_name);
	print_size(files);
	print_time(files);
	if (S_ISLNK(files->data.st_mode))
	{
		ft_bzero(buf, _POSIX_SYMLINK_MAX + 1);
		if (readlink(files->fullname, buf, _POSIX_SYMLINK_MAX) > 0)
			printf("%s -> %s\n", files->name, buf);
	}
	else
		printf("%s\n", files->name);
}

void  addtodirs(t_node **dirlist, t_node *files)
{
	t_node *new;
	if(!(new = ft_memalloc(sizeof(t_node))))
		return;
	new->name = ft_strdup(files->name);
	new->path = ft_strdup(files->path);
	new->fullname = ft_strdup(files->fullname);
	lstat(new->fullname, &(new->data));
	new->isdir = (S_ISDIR(new->data.st_mode)) ? true : false;
	new->next = (*dirlist)->next;
	(*dirlist)->next = new;
	*dirlist = new;
}

void				printblocksize(t_node *file, char flags)
{
	size_t			size;
	int				lstsize;

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
		printf("total %zu\n", size);
}

void  printit(t_node **dirlist, t_node *files, char flags)
{
	printblocksize(files, flags);
	while (files)
	{
		if (!(flags & 0x10) && (files->name)[0] == '.')
		{
			files = files->next;
			continue ;
		}
		if ((flags & 0x2) && files->isdir && ft_strcmp(files->name, ".") != 0 && ft_strcmp(files->name, "..") != 0)
			addtodirs(dirlist, files);
		if (flags & 0x4)
			printl(files);
		else
			printf("%s\n", files->name);
		files = files->next;
	}
}

char          *makepath(char *s1, char *s2)
{
	char *ret;
	char  *str;

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

void            addnode(t_node **head, char *path, char *name)
{
	t_node *new;

	if(!(new = ft_memalloc(sizeof(t_node))))
		return;
	new->name = ft_strdup(name);
	new->path = ft_strdup(path);
	new->fullname = makepath(path, name);
	lstat(new->fullname, &(new->data));
	new->isdir = (S_ISDIR(new->data.st_mode)) ? true : false;
	new->next = *head;
	*head = new;
}

void	freeit(t_node **files)
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

void  useadir(t_node **dirlist, char flags)
{
	DIR *dir;
	t_stat temp;
	t_dirant *dp;
	t_node *files;
	t_node *copyofdirlist;
	files = 0;
	copyofdirlist = *dirlist;
	if (!(dir = opendir((*dirlist)->fullname)))
	{
		if ((lstat((*dirlist)->fullname, &temp) == 0) && !(S_ISDIR(temp.st_mode)))
			addnode(&files, (*dirlist)->fullname, (*dirlist)->name);
		else
		{
			printf("ft_ls: %s: %s\n", (*dirlist)->fullname, strerror(errno));
			if (ft_strcmp(strerror(errno), "Permission denied") == 0)
				return ;
		}
	}
	else
		while((dp = readdir(dir)) != NULL)
			addnode(&files, (*dirlist)->fullname, dp->d_name);
	mergesort_list(&files, flags);
	printit(&copyofdirlist, files, flags);
	freeit(&files);
	if (dir)
		closedir(dir);
}

void usedirs(t_node **dirlist, char flags)
{
	//char	single;
	t_node *cpy;
	cpy = *dirlist;
	//single = (*dirlist)->next ? 0 : 1;
	while (cpy)
	{
		if (cpy->next)
			printf("%s:\n", cpy->fullname);
		useadir(&cpy, flags);
		cpy = cpy->next;
		if (cpy)
			printf("\n");
	}
}

int getargs(char *av,char *flags)
{
	if(*(av + 1) == '-')
		return(0);
	while(*(++av))
	{
		if(*av == 'r')
			*flags |= 0x1;
		else if(*av == 'R')
			*flags |= 0x2;
		else if(*av == 'l')
			*flags |= 0x4;
		else if(*av == 't')
			*flags |= 0x8;
		else if(*av == 'a')
			*flags |= 0x10;
		else
			return(-1);
	}
	return(1);
}

char parse(char **av, t_node **dirlist)
{
	char flags;
	int ret;
	ret = 0;
	flags = 0;
	av++;
	while(*av && **av == '-')
	{
		if((ret = getargs((*av), &flags)) < 1)
				break;
		av++;
	}
	if (ret == -1)
		return (0);
	if (!(*av))
		addnode(dirlist, 0, ".");
	while(*av)
	{
		addnode(dirlist, 0, *av);
		av++;
	}
	dirlist = mergesort_list(dirlist, flags);
	return(flags);
}

int main (int ac, char **av)
{
	char flags;
	t_node *dirlist;
	dirlist = 0;
	if((flags = parse(av,&dirlist)) == -1 && ac)
		return(0);
	usedirs(&dirlist, flags);
	freeit(&dirlist);
	return(0);
}
