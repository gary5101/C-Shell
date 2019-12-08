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
void lsl(char *name,char *dirname)
{
    struct stat st;
    char *modeval = malloc(sizeof(char) * 9 + 1);
    char temp[1000];
    strcpy(temp,dirname);
    strcat(temp,"/");
    strcat(temp,name);
    if(stat(temp, &st) == 0)
    {
        char *type=(char*) malloc(5);
        mode_t perm = st.st_mode;
        if(S_ISDIR(perm))
        {
            strcpy(type,"d");
        }
        else if(S_ISREG(perm))
        {
            strcpy(type,"-");
        }
        else if(S_ISLNK(perm))
        {
            strcpy(type,"l");
        }
        modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
        modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
        modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
        modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
        modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
        modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
        modeval[6] = (perm & S_IROTH) ? 'r' : '-';
        modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
        modeval[8] = (perm & S_IXOTH) ? 'x' : '-';    
        modeval[9]='\0';
        int nlink =st.st_nlink;
        char *pname=(char*)malloc(100);
        char *gname=(char*)malloc(100);
        pname=getpwuid(st.st_uid)->pw_name;
        gname=getgrgid(st.st_gid)->gr_name;
        long long int size=st.st_size;
        char *buffer=(char*)malloc(80);
        strcpy(buffer,ctime(&st.st_mtime));
        buffer+=4;
        buffer[12]='\0';
        printf("%s%s\t%d\t%s\t%s\t%lld\t%s\t%s\n",type,modeval,nlink,pname,gname,size,buffer,name);                        
    }
    else
    {
        perror("Sorry cannot open file status!\n");
        return;
    }   
}
void ls(char **com, int noofcom,char *fwd)
{
    int lflag=0,aflag=0;
    int noofdir=0;
    char dirn[1000][1000];
    for (int i=1;i<noofcom;i++)
    {
        if(com[i][0]=='-')
        {
            for (int j = 1; j < strlen(com[i]); j++)
            {
                if(com[i][j]=='l')
                {
                    lflag=1;
                }
                else if (com[i][j]=='a')
                {
                    aflag=1;
                }
                else
                {
                    printf("ls: invaild option --\'%c\'\n",com[i][j]);
                    return;
                }
                
            }
        }
        else
        {
            strcpy(dirn[noofdir++],com[i]);   
        }
        
    }
    if(noofdir==0)
    {
        char dirname[10];

        DIR*p;
        struct dirent *d;
        strcpy(dirname,".");
        p=opendir(dirname);
        if(p==NULL)
        {
            char tempname[1000];
            strcpy(tempname,"ls: cannot access \'");
            strcat(tempname,dirname);
            strcat(tempname,"\'");
            perror(tempname);
            return;
        }
        while(d=readdir(p))
        {
            if(aflag==1)
            {
                if(lflag==0)
                    printf("%s\n",d->d_name);
                else
                {
                    lsl(d->d_name,dirname);                    
                }
                
            }
            else 
            {
                if(lflag==0)
                {
                    if(d->d_name[0]!='.')
                        printf("%s\n",d->d_name);
                }
                else
                {
                    if(d->d_name[0]!='.')
                    {
                        lsl(d->d_name,dirname);
                    }
                }
                
            }
    
                   
        }
    }
    for(int i=0;i< noofdir;i++)
    {
        char *dirname=(char*)malloc(1000);

        DIR*p;
        struct dirent *d;
        strcpy(dirname,dirn[i]);
        if(dirname[0]=='~')
        {
            dirname+=1;
            char *temp=(char *)malloc(1000);
            strcpy(temp,fwd);
            strcat(temp,dirname);
            strcpy(dirname,temp);
        }
        if((dirname[strlen(dirname)-1]=='/')&&(strlen(dirname)!=1))
        {
            dirname[strlen(dirname)-1]='\0';
        }
        p=opendir(dirname);
        if(p==NULL)
        {
            char tempname[1000];
            strcpy(tempname,"ls: cannot access \'");
            strcat(tempname,dirname);
            strcat(tempname,"\'");
            perror(tempname);
            continue;
        }
        //printf("%s:\n",dirname);
        if(noofdir>1)
            printf("%s:\n",dirname);
        while(d=readdir(p))
        {
            if(aflag==1)
            {
                if(lflag==0)
                    printf("%s\n",d->d_name);
                else
                {
                    
                    lsl(d->d_name,dirname);                    
                }
                
            }
            else 
            {
                if(lflag==0)
                {
                    if(d->d_name[0]!='.')
                        printf("%s\n",d->d_name);
                }
                else
                {
                    if(d->d_name[0]!='.')
                    {
                        
                        lsl(d->d_name,dirname);
                    }
                }
                
            }  
        }
        printf("\n");
    }

}