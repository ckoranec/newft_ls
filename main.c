#include "ft_ls.h"



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

char parse(int ac,char **av)
{
char flags;
int ret;
ret = 0;
flags = 0;
if(ac < 2)
{
return(0);
}
av++;
    while(*av && **av == '-')
    {
        if((ret = getargs((*av++), &flags)) < 1)
            break;
    }
if (ret == -1)
    return (0);

if(flags & 0x1)
printf("r");
if(flags & 0x2)
printf("R");
if(flags & 0x4)
printf("l");
if(flags & 0x8)
printf("t");
if(flags & 0x10)
printf("a");
return(flags);
}

int main (int ac, char **av)
{
parse(ac,av);
/*makeldirs(somedirs);
sortlist(somelist);
printls(sorted list);
freelist(sorted list);
*/

return(0);
}