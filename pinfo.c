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
void pinfo(char **process,int noofw)
{
    int procid=0;
    char *prid=(char *)malloc(1000);
    int c=0;
    if(noofw==1)
    {
        procid=getpid();
        while(procid!=0)
        {
            prid[c++]=48+(procid%10);
            procid/=10;
        }
        for(int i=0;i<c/2;i++)
        {
            char temp;
            temp=prid[i];
            prid[i]=prid[c-i-1];
            prid[c-i-1]=temp;
        }
    }
    else
        strcpy(prid,process[1]);
    //printf("prid %s %d\n",prid,procid);
    FILE *fname;
    ssize_t read;
    size_t len=0,len1=0;    
    char * line =NULL;
    char *line1 =NULL;
    char *path=(char*)malloc(1000);
    strcpy(path,"/proc/");
    strcat(path,prid);
    fname=fopen(path,"r");
    if(fname==NULL)
    {
        printf("Process not found\n");
        return;
    }
    fclose(fname);
    strcat(path,"/");
    char *temp=(char*)malloc(1000);
    strcpy(temp,path);
    strcat(path,"status");
    strcat(temp,"exe");
    FILE *fp;
    // FILE *f;
    fp=fopen(path,"r");
    if(fp==NULL)
    {
        perror("");
        return;
    }
    // f=fopen(temp,"r");
    // if(f==NULL)
    // {
    //     perror("");
    //     return;
    // }
    char *status=(char*)malloc(5);
    char *vmsize=(char*)malloc(20);
    int k=0;
    //printf("1\n");
    while(getline(&line,&len,fp)!=-1)
    {
        //printf("2\n");
        if(strstr(line,"State:")==line)
        {
            strcpy(status,line);
            status+=7;
            strcpy(line,status);
            int i=0;
            while(line[i]!=' ')
            {
                status[i++]=line[i];
            }
            status[i]='\0';
            
        }
        if(strstr(line,"VmSize:")==line)
        {
            for(int i=strlen(line)-5;;i--)
            {
                if(line[i]==' ')
                    break;
                vmsize[k++]=line[i];
            }
            for(int i=0;i<k/2;i++)
            {
                char temp;
                temp=vmsize[i];
                vmsize[i]=vmsize[k-i-1];
                vmsize[k-i-1]=temp;
            }
        }
    }
    ssize_t length;
    char *execpath=(char *)malloc(1000);
    readlink(temp,execpath,1000);
    // while (getline(&line1,&len1,f)!=-1)
    // {
    //     strcpy(execpath,line1);
    // }
    printf("pid -- %s\n",prid);
    printf("Process Status -- %s\n",status);
    printf("memory -- %s\n",vmsize);
    printf("Exectuable Path --%s\n",execpath);



    return;
}