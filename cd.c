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
void cd(char *path,int noofw,char *fwd)
{
    if(noofw>2)
    {
        printf("incorrect no of arguments for cd\n");
    }
    else if(noofw==1)
    {
        if(chdir(fwd)!=0)
        {
            perror("cd failed");
        }
    }
    else
    {
        if(path[0]=='~')
        {
            path+=1;
            char *temp=(char *)malloc(1000);
            strcpy(temp,fwd);
            strcat(temp,path);
            if(chdir(temp)!=0)
            {
                perror("cd failed");
            }   
        }
        else
        {
            if(chdir(path)!=0)
            {
                perror("cd failed");
            }
        }
        
    }
    

}
