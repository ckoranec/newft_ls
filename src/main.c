#include "../includes/ft_ls.h"
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

void  printit(t_node **dirlist, t_node *files, char flags)
{
	printf("%s:\n", files->path);
	while (files)
	{
		if (!(flags & 0x10) && (files->name)[0] == '.')
		{
			files = files->next;
			continue ;
		}
		if ((flags & 0x2) && files->isdir == true && ft_strcmp(files->name, ".") != 0 && ft_strcmp(files->name, "..") != 0)
			addtodirs(dirlist, files);
		printf("%s\n", files->name);
		files = files->next;
	}
	printf("\n");
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

void  useadir(t_node **dirlist, char flags)
{
	DIR *dir;
	t_dirant *dp;
	t_node *files;
	t_node *originaldir;
	files = 0;
	originaldir = *dirlist;
	if (!(dir = opendir((*dirlist)->fullname)))
		return ;
	while((dp = readdir (dir)) != NULL)
		addnode(&files, (*dirlist)->fullname, dp->d_name);
	files = *(mergesort_list(&files, flags));
	printit(dirlist, files, flags);
	closedir(dir);
	*dirlist = originaldir;
}

void usedirs(t_node **dirlist, char flags)
{
	t_node *top;
	top = *dirlist;
	while (*dirlist)
	{
		useadir(dirlist, flags);
		*dirlist = (*dirlist)->next;
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
	return(0);
}
