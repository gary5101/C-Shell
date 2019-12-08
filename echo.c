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
void echo(char **com,int noofcom)
{
    for (int i=1;i<noofcom;i++)
    {
        if(i==noofcom-1)
        {
            if (com[i][0]=='$')
            {
                if(strlen(com[i])==1)
                {
                    printf("enter a proper variable name");
                }
                else
                {
                char *value=getenv(com[i]+1);
                if(value!=NULL)
                printf("%s",value);   
                }
            }
            else
                printf("%s",com[i]);
            printf("\n");
        }
        else
        {
            if (com[i][0]=='$')
            {
                if(strlen(com[i])==1)
                {
                    printf("enter a proper variable name");
                }
                else
                {
                char *value=getenv(com[i]+1);
                if(value!=NULL)
                printf("%s ",value);   
                }
            }
            else
                printf("%s ",com[i]);
        }
        
    }
}