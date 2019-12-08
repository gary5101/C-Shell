#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/utsname.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <memory.h>
#include <sys/wait.h>
void setenvvar(char **eachc, int noofw)
{
    if (noofw > 3 || noofw < 2)
    {
        printf("incorrect no of arguments for setenv\n");
    }
    if (noofw == 3)
        setenv(eachc[1], eachc[2], 1);
    else if (noofw == 2)
        setenv(eachc[1], "", 1);
}
void unsetenvvar(char **eachc, int noofw)
{
    if (noofw != 2)
    {
        printf("incorrect no of arguments for unsetenv\n");
    }
    else
    {
        if (unsetenv(eachc[1]) != 0)
        {
            perror("variable not set");
        }
    }
}