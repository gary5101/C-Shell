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
#include "inandout.h"
#include <sys/wait.h>
#include <signal.h>
void handle_int();
void sigstoph();
void pipedcommand(char *fullpipecom, char *fwd, char *prompt, int inflag, int outflag, int pipeflag);
void inredirc(char **com, int noofcom, char *fwd, char *prompt);
void outredirc(char **com, int noofcom, char *fwd, char *prompt, int inflag, int pipeflag);
void doubleredirc(char **com, int noofcom, char **filename, int noofw, char *fwd, char *prompt, int inflag, int pipeflag);
void singleredirc(char **com, int noofcom, char **filename, int noofw, char *fwd, char *prompt, int inflag, int pipeflag);
void pinfo(char **process, int noofw);
void restallcommands(char **command, int noofw, char *prompt);
void jobslist(int noofcom);
void kjob(char **com, int noofcom);
void bg(char **com, int noofcom);
void fg(char **com, int noofcom);
void overkill(int noofcom);
void lsl(char *name, char *dirname);
void ls(char **com, int noofcom, char *fwd);
void echo(char **com, int noofcom);
void pwd(int noofw);
void cd(char *path, int noofw, char *fwd);
void history_store(char *command,char *fwd);
void history_print(char *fwd, char **hiscommand, int noofw);
void setenvvar(char **eachc, int noofw);
void unsetenvvar(char **eachc, int noofw);
void sigstophandle(int a)
{
    // printf("hi\n");
    sigstoph();
}
void siginthandle(int a)
{
    handle_int();
}
void execute(char **eachc, int noofw, char *fwd, char *prompt,int *inout)
{
    if (strcmp(eachc[0], "quit") == 0)
    {
        exit(0);
    }
    else if (strcmp(eachc[0], "cd") == 0)
    {
        //printf("cd input %s\n",eachc[1]);
        cd(eachc[1], noofw, fwd);
    }

    else
    {
        int st=0;
        int pid = fork();

        if (pid == 0)
        {
            // fprintf(stderr,"jkfsda=%s\n",eachc[0]);
            // fprintf(stderr,"3\n");
            dup2(inout[1],1);
            if (strcmp(eachc[0], "pwd") == 0)
            {
                pwd(noofw);
            }
            else if (strcmp(eachc[0], "echo") == 0)
            {
                echo(eachc, noofw);
            }
            // else if (strcmp(eachc[0], "") == 0)
            // {
            // }
            else if (strcmp(eachc[0], "ls") == 0)
            {
                // fprintf(stderr,"hi");
                // printf("hi\n");
                ls(eachc, noofw, fwd);
            }
            else if (strcmp(eachc[0], "pinfo") == 0)
            {
                pinfo(eachc, noofw);
            }
            else if (strcmp(eachc[0], "history") == 0)
            {
                history_print(fwd, eachc, noofw);
            }
            else if (strcmp(eachc[0], "setenv") == 0)
            {
                setenvvar(eachc, noofw);
            }
            else if (strcmp(eachc[0], "") == 0)
            {
            }
            else if (strcmp(eachc[0], "unsetenv") == 0)
            {
                unsetenvvar(eachc, noofw);
            }
            else if (strcmp(eachc[0], "jobs") == 0)
            {
                jobslist(noofw);
            }
            else if (strcmp(eachc[0], "overkill") == 0)
            {
                overkill(noofw);
            }
            else if (strcmp(eachc[0], "kjob") == 0)
            {
                kjob(eachc, noofw);
            }
            else if (strcmp(eachc[0], "bg") == 0)
            {
                bg(eachc, noofw);
            }
            else if (strcmp(eachc[0], "fg") == 0)
            {
                fg(eachc, noofw);
            }
            else
            {
                // fprintf(stderr,"4\n");
                if (execvp(eachc[0], eachc) != 0)
                {
                    char tempname[1000];
                    strcpy(tempname, "Command \"");
                    strcat(tempname, eachc[0]);
                    strcat(tempname, "\" not found");
                    fprintf(stderr, "%s\n", tempname);

                    // printf("Command \"%s\" not found\n", temp[0]);
                    //perror("");
                    // return;
                }
            }
            // fprintf(stderr,"5\n");
            exit(0);
        }
        else
        {
            while (wait(&st) != pid)
                ;
        }
        
    }
}
void commandcheck(char **eachc, int noofw, char *fwd, char *prompt)
{
    if (strcmp(eachc[0], "quit") == 0)
    {
        exit(0);
    }
    else if (strcmp(eachc[0], "cd") == 0)
    {
        //printf("cd input %s\n",eachc[1]);
        cd(eachc[1], noofw, fwd);
    }
    else if (strcmp(eachc[0], "pwd") == 0)
    {
        pwd(noofw);
    }
    else if (strcmp(eachc[0], "echo") == 0)
    {
        echo(eachc, noofw);
    }
    // else if (strcmp(eachc[0], "") == 0)
    // {
    // }
    else if (strcmp(eachc[0], "ls") == 0)
    {
        // printf("hi\n");
        ls(eachc, noofw, fwd);
    }
    else if (strcmp(eachc[0], "pinfo") == 0)
    {
        pinfo(eachc, noofw);
    }
    else if (strcmp(eachc[0], "history") == 0)
    {
        history_print(fwd, eachc, noofw);
    }
    else if (strcmp(eachc[0], "setenv") == 0)
    {
        setenvvar(eachc, noofw);
    }
    else if (strcmp(eachc[0], "") == 0)
    {
    }
    else if (strcmp(eachc[0], "unsetenv") == 0)
    {
        unsetenvvar(eachc, noofw);
    }
    else if (strcmp(eachc[0], "jobs") == 0)
    {
        jobslist(noofw);
    }
    else if (strcmp(eachc[0], "overkill") == 0)
    {
        overkill(noofw);
    }
    else if (strcmp(eachc[0], "kjob") == 0)
    {
        kjob(eachc, noofw);
    }
    else if (strcmp(eachc[0], "bg") == 0)
    {
        bg(eachc, noofw);
    }
    else if (strcmp(eachc[0], "fg") == 0)
    {
        fg(eachc, noofw);
    }
    else
    {
        restallcommands(eachc, noofw, prompt);
    }
}
void space(char *temp)
{
    char userinput1[1000];
    for (int i = 0; i < 1000; i++)
    {
        userinput1[i] = 0;
    }
    int c = 0;
    for (int i = 0; i < strlen(temp); i++)
    {
        if (i == 0)
        {
            if ((temp[i] == ' ') || temp[i] == '\t')
                continue;
            else
            {
                userinput1[c++] = temp[i];
            }
        }
        else if (((temp[i] == ' ' || temp[i] == '\t') && (temp[i - 1] == ' ' || temp[i - 1] == '\t')) || temp[i] == '\t')
        {
            continue;
        }
        else
        {
            userinput1[c++] = temp[i];
        }
    }
    if (userinput1[strlen(userinput1) - 1] == ' ')
    {

        userinput1[strlen(userinput1) - 1] = '\0';
    }
    strcpy(temp, userinput1);
}
void sh_loop(char **inpath)
{

    char *prompt = (char *)malloc(1000);
    char *user = (char *)malloc(1000);
    char host[1000];
    user = getenv("USER");
    gethostname(host, 1000);
    // char *fwd = (char *)malloc(PATH_MAX);
    char *tem = (char *)malloc(PATH_MAX);
    char fwd[PATH_MAX];
    char cwd[PATH_MAX];
    strcpy(tem, inpath[0]);
    for (int i = strlen(tem) - 1; i >= 0; i--)
    {
        if (tem[i] == '/')
        {
            tem[i] = '\0';
            break;
        }
    }
    // printf("tem=%s\ninpath=%s\n",tem,inpath[0]);
    if (getcwd(fwd, sizeof(fwd)) != NULL)
    {
        strcat(fwd, "/");
        strcat(fwd, tem);
        if (chdir(fwd) != 0)
        {
            perror("cd failed");
        }
        //printf("%s\n", fwd);
        if (getcwd(fwd, sizeof(fwd)) != NULL)
        {
            strcpy(prompt, "<");
            strcat(prompt, user);
            strcat(prompt, "@");
            strcat(prompt, host);
            strcat(prompt, ":");
            strcat(prompt, "~");
            strcat(prompt, ">");
            printf("<%s@%s:%s>", user, host, "~");
        }
        else
        {
            perror("getcwd() error2");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        perror("getcwd() error1");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        char *userinput = (char *)malloc(1000);
        char *slinecopy = (char *)malloc(1000);
        char ch;
        scanf("%[^\n]s", userinput);
        history_store(userinput,fwd);
        scanf("%c", &ch);
        //space(userinput);
        //printf("%s\n",userinput);
        char **sline = (char **)malloc(1000 * sizeof(char *));
        for (int i = 0; i < 1000; i++)
        {
            sline[i] = (char *)malloc(1000);
        }
        int noofc = 0;
        while (userinput != NULL)
        {
            sline[noofc++] = strsep(&userinput, ";");
            //printf("first=%s\n",sline[noofc-1]);
        }
        for (int i = 0; i < noofc; i++)
        {

            space(sline[i]);
            //printf("i=%d %s\n",i,sline[i]);
            strcpy(slinecopy, sline[i]);
            // printf("org=%s\ncopy=%s\n",sline[i],slinecopy);
            char *eachc[1000];
            for (int i = 0; i < 1000; i++)
            {
                eachc[i] = (char *)malloc(1000);
            }
            int noofw = 0;
            int noofw1 = 0;
            int noofw2 = 0;
            while (sline[i] != NULL)
            {
                eachc[noofw++] = strsep(&sline[i], " ");
                //printf("%s\n",eachc[noofw-1]);
            }
            int inflag = 0;
            int falseinflag = 0;
            int pipeflag = 0;
            int outflag = 0;
            for (int i = 0; i < noofw; i++)
            {
                for (int j = 0; j < strlen(eachc[i]); j++)
                {
                    if (eachc[i][j] == '<')
                        inflag++;
                    if (eachc[i][j] == '<' && pipeflag != 0)
                        falseinflag++;
                    if (eachc[i][j] == '|')
                        pipeflag++;
                    if (eachc[i][j] == '>')
                    {
                        outflag++;
                        if (j != (strlen(eachc[i]) - 1))
                            if (eachc[i][j + 1] == '>')
                                j++;
                    }
                }
            }
            if (inflag > 1 || outflag > 1 || falseinflag > 0)
            {
                fprintf(stderr, "invalid command");
            }
            if (pipeflag > 0)
                pipedcommand(slinecopy, fwd, prompt, inflag, outflag, pipeflag);
            else if (outflag == 1)
            {
                
                outredirc(eachc, noofw, fwd, prompt, inflag, pipeflag);
            }
            else if (inflag == 1)
            {
                inredirc(eachc, noofw, fwd, prompt);
            }
            else
            {
                commandcheck(eachc, noofw, fwd, prompt);
            }
        }
    
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            //printf("hi");
            if (strstr(cwd, fwd) == cwd)
            {
                char *a = cwd + strlen(fwd);
                char pwd[PATH_MAX];
                strcpy(pwd, "~");
                strcat(pwd, a);
                // if(strcmp(cwd,fwd)==0)
                // {
                //     strcpy(pwd,"~");
                // }
                strcpy(prompt, "<");
                strcat(prompt, user);
                strcat(prompt, "@");
                strcat(prompt, host);
                strcat(prompt, ":");
                strcat(prompt, pwd);
                strcat(prompt, ">");
                printf("<%s@%s:%s>", user, host, pwd);
            }
            else
            {
                strcpy(prompt, "<");
                strcat(prompt, user);
                strcat(prompt, "@");
                strcat(prompt, host);
                strcat(prompt, ":");
                strcat(prompt, cwd);
                strcat(prompt, ">");
                printf("<%s@%s:%s>", user, host, cwd);
            }
        }
        else
        {
            perror("getcwd() error");
            exit(EXIT_FAILURE);
        }
        
    }
    return;
}

int main(int argc, char *argv[])
{
    tempin=dup(0);
    tempout=dup(1);
    signal(SIGINT, siginthandle);
    signal(SIGTSTP,sigstophandle);
    sh_loop(argv);
    return EXIT_SUCCESS;
}