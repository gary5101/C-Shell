#include<stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/utsname.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include<ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include<fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <memory.h>
#include<sys/wait.h>
void pwd(int noofw)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        
        printf("%s\n",cwd);
    }
    else
    {
        perror("getcwd() error");
      exit(EXIT_FAILURE);

    }
    
}