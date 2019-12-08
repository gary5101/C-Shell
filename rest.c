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
#include <signal.h>
#include <time.h>

char bgp[1000][1000];
int allpidscopy[1000];
int allpids[1000];
int bgpids[1000];
int ind = 0;
char *reprompt;
char fgp[1000];
int fpid = -2;
int foreback = 0;
int overkiller = 0;
void handle_int()
{
    

    if(fpid>0)
    {
        kill(fpid,SIGINT);
    }
}
int proc_state(int pid, char *c)
{
    char *temp1 = (char *)malloc(sizeof(char) * 100);
    char *temp = (char *)malloc(sizeof(char) * 100000);
    strcpy(temp, "/proc/");
    sprintf(temp1, "%d", pid);
    strcat(temp1, "/status");
    strcat(temp, temp1);
    int f = open(temp, O_RDONLY);
    // printf("f=%d\n",f);
    if (f == -1)
    {
        free(temp);
        free(temp1);
        return -1;
    }
    strcpy(temp1, "State");
    read(f, temp, 100000);
    // printf("came here1\n");
    char *cp_temp1 = temp1;
    temp1 = strstr(temp, temp1);
    // printf("pid -- %d\n",pid);
    int i = 0;
    while (1)
    {
        if (temp1[i] == '\n')
        {
            temp1[i] = 0;
            break;
        }
        i++;
    }
    // printf("came here2\n");
    // printf("Process Status -- %.12s\n",temp1+7);
    *c = temp1[7];
    if (temp != NULL)
        free(temp);
    if (cp_temp1 != NULL)
        free(cp_temp1);
    close(f);
}
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = number_of_seconds;

    // Stroing start time
    clock_t start_time = clock();

    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds)
        ;
}

void sigstoph()
{
    // printf("ola\n");
    // printf("%d\n",setpgid(fpid,fpid));
    kill(fpid, 19);
    strcpy(bgp[ind], fgp);
    allpids[ind] = fpid;
    allpidscopy[ind++] = fpid;
}
void signalhandle(int a)
{
    sigstoph();
}
void handler(int a)
{
    // if (foreback == 1 &&)
    // {
    int pid;
    pid_t dead = waitpid(-1, &pid, WNOHANG);
    for (int i = 0; i < ind; i++)
    {
        if (allpids[i] == (int)dead)
        {
            // printf("allpids=%d\n",allpids[i]);

            allpids[i] = -2;
            // if (overkiller == 0)
            // {
            fprintf(stderr, "\n%s with pid %d exited\n", bgp[i], dead);
            if (pid == 0)
            {
                fprintf(stderr, "normally\n");
            }
            else
            {
                fprintf(stderr, "abruptly\n");
            }
            printf("%s", reprompt);
            fflush(stdout);
            // }
        }
    }
    //}
}
void restallcommands(char **command, int noofw, char *prompt)
{
    // printf("hi1\nnoofw=%d\n",noofw);
    //     fflush(stdout);
    reprompt = (char *)malloc(1000);
    strcpy(reprompt, prompt);
    int st = 0;
    if (strcmp(command[noofw - 1], "&") != 0)
    {
        // printf("hi");
        // fflush(stdout);
        foreback = 0;
        //printf("hi\n");
        char **temp = (char **)malloc(sizeof(char *) * 1000);
        for (int i = 0; i < noofw + 1; i++)
        {
            temp[i] = (char *)malloc(1000);
        }

        for (int i = 0; i < noofw; i++)
        {
            strcpy(temp[i], command[i]);
        }
        temp[noofw] = NULL;
        pid_t pd;
        // printf("hi3");
        // fflush(stdout);
        pd = fork();

        if (pd == 0)
        {
            //     printf("hi2");
            // fflush(stdout);
            if (execvp(temp[0], temp) != 0)
            {
                char tempname[1000];
                strcpy(tempname, "Command \"");
                strcat(tempname, temp[0]);
                strcat(tempname, "\" not found");
                fprintf(stderr, "%s\n", tempname);

                // printf("Command \"%s\" not found\n", temp[0]);
                //perror("");
                exit(0);
                // return;
            }
        }
        else
        {
            // signal(SIGINT,handle_int);
            // setpgid(pd, pd);
            strcpy(fgp, temp[0]);
            for (int i = 1; i < noofw; i++)
            {
                strcat(fgp, " ");
                strcat(fgp, temp[i]);
            }
            fpid = pd;
            // while (wait(&st) != pd)
            // ;
            int status;
            waitpid(fpid, &status, WUNTRACED);
            //return;
            fpid = -2;
        }
    }
    else
    {
        // printf("hi4");
        // fflush(stdout);
        foreback = 1;
        char **temp = (char **)malloc(sizeof(char *) * 1000);
        for (int i = 0; i < noofw; i++)
        {
            temp[i] = (char *)malloc(1000);
        }
        for (int i = 0; i < noofw - 1; i++)
        {
            strcpy(temp[i], command[i]);
        }
        temp[noofw - 1] = NULL;
        int pid;
        pid = fork();

        if (pid == 0)
        {

            if (execvp(temp[0], temp) != 0)
            {
                fprintf(stderr, "Command \"%s\" not found\n", temp[0]);
                //perror("");
                exit(0);
            }
        }
        else
        {
            setpgid(pid, pid);
            strcpy(bgp[ind], temp[0]);
            for (int i = 1; i < noofw - 1; i++)
            {
                strcat(bgp[ind], " ");
                strcat(bgp[ind], temp[i]);
            }
            allpidscopy[ind] = pid;
            allpids[ind++] = pid;
            signal(SIGCHLD, handler);
            return;
        }
    }
}
void jobslist(int noofcom)
{
    // prinf("jobs\n");
    if (noofcom != 1)
    {
        fprintf(stderr, "jobs commmand does not take any arguments\n");
        return;
    }

    int k = 1;
    for (int i = 0; i < ind; i++)
    {
        if (allpids[i] != -2)
        {
            size_t len = 0;
            char *line = NULL;
            char *path = (char *)malloc(1000);
            char *prid = (char *)malloc(1000);
            strcpy(path, "/proc/");
            sprintf(prid, "%d", allpids[i]);
            strcat(path, prid);
            strcat(path, "/");
            char *temp = (char *)malloc(1000);
            strcpy(temp, path);
            strcat(path, "status");
            char *status = (char *)malloc(20);
            FILE *fp;
            fp = fopen(path, "r");
            if (fp == NULL)
            {
                perror("");
                return;
            }
            while (getline(&line, &len, fp) != -1)
            {
                //printf("2\n");
                if (strstr(line, "State:") == line)
                {
                    strcpy(status, line);
                    status += 7;
                    strcpy(line, status);
                    int i = 0;
                    // printf("line=%s status=%s\n",line,status);
                    while (line[i] != '(')
                    {
                        i++;
                    }
                    i++;
                    int counter = 0;
                    // printf("i=%d\n",i);
                    while (line[i] != ')')
                    {
                        status[counter++] = line[i];
                        i++;
                    }
                    status[counter] = '\0';
                }
            }
            if(strcmp(status,"sleeping")==0)
                printf("[%d] running %s [%d]\n", k++, bgp[i], allpids[i]);
            else if (strcmp(status, "zombie") != 0)
                printf("[%d] %s %s [%d]\n", k++, status, bgp[i], allpids[i]);
            else
            {
                allpids[i] = -2;
            }
        }
    }
}
void overkill(int noofcom)
{
    if (noofcom > 1)
    {
        fprintf(stderr, "overkill commmand does not take any arguments\n");
        return;
    }
    overkiller = 1;
    int k = 0;
    for (int i = 0; i < ind; i++)
    {
        if (allpids[i] > 0)
        {
            kill(allpids[i], SIGKILL);
            delay(1000);
            // fprintf(stderr, "%s with pid %d exited\nabruptly\n", bgp[i], allpids[i]);
            // allpids[i]=-2;
        }
        // allpids[i]=-2;
    }
    overkiller = 0;
}
void kjob(char **com, int noofcom)
{
    if (noofcom != 3)
    {
        fprintf(stderr, "kjob takes two arguments\n");
        return;
    }
    int jobid, signalno;
    if (!sscanf(com[1], "%d", &jobid))
    {
        fprintf(stderr, "enter a number as jobid\n");
        return;
    }

    if (!sscanf(com[2], "%d", &signalno))
    {
        fprintf(stderr, "enter a number as signal number\n");
        return;
    }
    int k = 0, i = 0;
    for (i = 0; i < ind; i++)
    {
        if (allpids[i] > 0)
            k++;
        if (k == jobid)
        {
            kill(allpids[i], signalno);
            break;
        }
    }
    if (i == ind)
    {
        fprintf(stderr, "jobid does not exist\n");
    }
}
void bg(char **com, int noofcom)
{
    if (noofcom != 2)
    {
        fprintf(stderr, "bg takes one arguments\n");
        return;
    }
    int jobid;
    if (!sscanf(com[1], "%d", &jobid))
    {
        fprintf(stderr, "enter a number as jobid\n");
        return;
    }

    int k = 0, i = 0;
    for (i = 0; i < ind; i++)
    {
        if (allpids[i] > 0)
            k++;
        if (k == jobid)
        {
            kill(allpids[i], SIGCONT);
            break;
        }
    }
    if (i == ind)
    {
        fprintf(stderr, "jobid does not exist\n");
    }
}
void fg(char **com, int noofcom)
{
    if (noofcom != 2)
    {
        fprintf(stderr, "fg takes one arguments\n");
        return;
    }
    int jobid;
    if (!sscanf(com[1], "%d", &jobid))
    {
        fprintf(stderr, "enter a number as jobid\n");
        return;
    }

    int k = 0, i = 0;
    for (i = 0; i < ind; i++)
    {
        if (allpids[i] > 0)
            k++;
        if (k == jobid)
        {
            // // signal(SIGTTOU, SIG_IGN);
            // // tcsetpgrp(0, getpgid(allpids[i]));
            // // signal(SIGTTOU, SIG_DFL);
            // // strcpy(fgp,bgp[i]);
            // // fpid=allpids[i];
            // // allpids[i] = -2;
            // // printf("hi\n");
            // // signal(SIGTSTP,signalhandle);
            // // waitpid(getpgid(allpids[i]), NULL, WUNTRACED);
            // // printf("hi1\n");
            // // signal(SIGTTOU, SIG_IGN);
            // // tcsetpgrp(0, getpgid(getpid()));
            // // signal(SIGTTOU, SIG_DFL);
            kill(allpids[i], SIGCONT);

            // int status;
            // fpid=allpids[i];
            // strcpy(fgp,bgp[i]);
            // waitpid((pid_t)allpids[i], &status, WUNTRACED);
            // allpids[i]=-2;
            char stdin_path[100];
            sprintf(stdin_path, "/proc/%d/fd/0", allpids[i]);
            // int new = open(stdin_path, O_APPEND);
            int tmp_in = dup(0);
            int new_stdin = open(stdin_path, O_APPEND);
            strcpy(fgp, bgp[i]);
            fpid = allpids[i];
            allpids[i] = -2;
            dup2(new_stdin, 0);
            while (1)
            {

                char c;
                int yes = proc_state(fpid, &c);
                // printf("proc_state===%d %c\n", yes, c);
                if (yes == -1 || c == 'Z' || c == 'T')
                {

                    // printf("c=%c\n", c);
                    break;
                }
            }
            dup2(tmp_in, 0);
            close(tmp_in);
            close(new_stdin);
            // printf("susflag=%d\n",sus_flag);
            break;
        }
    }
    if (i == ind)
    {
        fprintf(stderr, "jobid does not exist\n");
    }
}
