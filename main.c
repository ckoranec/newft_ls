#include "ft_ls.h"



t_node *lst_swap(t_node *p1, t_node *p2)
{
  p1->next = p2->next;
  p2->next = p1;
  return (p2);
}

t_node *lst_sort_ascii(t_node *head)
{

  if (!head)
    return (NULL);
  if (head->next && ft_strcmp(head->path, head->next->path) > 0)
    head = lst_swap(head, head->next);
  head->next = lst_sort_ascii(head->next);
  if (head->next && ft_strcmp(head->path, head->next->path) > 0)
    {
      head = lst_swap(head, head->next);
      head->next = lst_sort_ascii(head->next);
    }
  return (head);
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

void            addnode(t_node **head, t_stat data, char *path, char *name)
{
  t_node *new;

  if(!(new = ft_memalloc(sizeof(t_node))))
    return;
new->name = ft_strdup(name);
  new->data = data;
  new->path = ft_strdup(path);
new->fullname = makepath(path, name);
new->isdir = S_ISDIR(data.st_mode) ? true : false;
new->next = *head;
*head = new;
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

char parse(int ac,char **av, t_node **dirlist)
{
  t_stat stat;
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
  if (ret != -1 && !(*av))
  {
    lstat(".", &stat);
    addnode(dirlist, stat, 0, ".");
  }
  while(*av)
      {
        lstat(*av, &stat);
        addnode(dirlist, stat, 0, *av);
        av++;
      }
  dirlist = mergesort_list(dirlist, flags);
  return(flags);
}

void  addtodirs(t_node **dirlist, t_node *fil)
{
  t_node *olddir;

  olddir = *dirlist;
  addnode(dirlist, fil->data, fil->path, fil->name);
  olddir->next = *dirlist;
  printf("added: %s\n", (*dirlist)->fullname);
}

void  printit(t_node **dirlist, t_node *fil, char flags)
{
  printf("%s:\n", fil->path);
  while (fil)
   {
     if (!(flags & 0x10) && (ft_strcmp(fil->name, ".") == 0 || ft_strcmp(fil->name, "..") == 0))
     {
       fil = fil->next;
       continue ;
     }
     if (flags & 0x2 && fil->isdir && ft_strcmp(fil->name, ".") != 0 && ft_strcmp(fil->name, "..") != 0)
          addtodirs(dirlist, fil);
     printf("%s\n", fil->name);
     fil = fil->next;
   }
   printf("\n");
}

void  useadir(t_node **dirlist, char flags)
{
    DIR *dir;
  t_dirant *dp;
  t_node *fil;
  t_node *originaldir;
  t_stat tmp;

  fil = 0;
  originaldir = *dirlist;
  printf("opening: %s\n", (*dirlist)->fullname);
  if (!(dir = opendir((*dirlist)->fullname)))
    return ;
while((dp = readdir (dir)) != NULL)
   {
     lstat(dp->d_name, &tmp);
     addnode(&fil,tmp, (*dirlist)->fullname,dp->d_name);
   }
   fil = *(mergesort_list(&fil, flags));
   printit(dirlist, fil, flags);
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

int main (int ac, char **av)
{
  char flags;
  t_node *dirlist;
  t_node *originallist;
  dirlist = 0;
if((flags = parse(ac,av,&dirlist)) == -1)
  return(0);
usedirs(&dirlist, flags);
 //sortlist();
 // print_ls_yo(fil);
 //closedir(dir);
 //makeldirs(somedirs);
 // sortlist(somelist);
/*printls(sorted list);
freelist(sorted list);
*/

return(0);
}
