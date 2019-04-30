#ifndef FT_LS_H
# define FT_LS_H
//# include "libft/libft.h"
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

typedef struct stat t_stat;
typedef struct dirent t_dirant;

typedef struct      s_node
{
            char     *fname;
            char    *path;
            char    *fullname;
            bool    single;
            bool    hidden;
            t_stat  data;
            struct s_node   *next;
}                   t_node;

#endif