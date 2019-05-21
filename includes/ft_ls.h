
#ifndef FT_LS_H
# define FT_LS_H

# include "../libft/libft.h"
# include "../ft_printf/ft_printf.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <pwd.h>
# include <string.h>
# include <grp.h>
# include <sys/xattr.h>
# include <time.h>
# include <errno.h>
# include <dirent.h>
# include <sys/acl.h>
# include <limits.h>

typedef struct stat t_stat;
typedef struct dirent t_dirant;

typedef struct      s_node
{
  char     *name;
  char    *path;
  char    *fullname;
  bool    single;
  bool    isdir;
  bool    hidden;
  t_stat  data;
  struct s_node   *next;
}                   t_node;

int     sortbyflag(char flags, t_node *a, t_node *b);
t_node**mergesort_list(t_node **list, char flags);
void printftype(t_node *files);
void printpermissions(t_node *files);
void printxattr(t_node *files);
void print_time(t_node *file);
void print_size(t_node *files);
void printls(t_node *files);
void addtodirs(t_node **dirlist, t_node *files);
void printblocksize(t_node *file, char flags);
void printit(t_node **dirlist, t_node *files, char flags);
char*makepath(char *s1, char *s2);
void addnode(t_node **head, char *path, char *name);
void freeit(t_node **files);
bool checkthatshitnadd(t_node **dirlist, t_node **files);
void useadir(t_node **dirlist, char flags);
void usedirs(t_node **dirlist, char flags);
int getargs(char *av, char *flags);
char ls_parse(char **av, t_node **dirlist);
int sorttime(t_node *a, t_node *b);
int sortbyflag(char flags, t_node *a, t_node *b);
t_node*sortedmerge_list(t_node *a, t_node *b, char flags);
void fbsplit_list(t_node *source, t_node **frontref, t_node **backref);
t_node**mergesort_list(t_node **list, char flags);


#endif


