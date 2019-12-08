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
#include "inandout.h"
void space(char *temp);
void execute(char **eachc, int noofw, char *fwd, char *prompt, int *inout);
void commandcheck(char **eachc, int noofw, char *fwd, char *prompt);
void inredirc(char **com, int noofcom, char *fwd, char *prompt)
{
    int noofw1 = 0, noofw2 = 0;
    char *eachc1[1000];
    for (int i = 0; i < 1000; i++)
    {
        eachc1[i] = (char *)malloc(1000);
    }
    char *eachc2[1000];
    for (int i = 0; i < 1000; i++)
    {
        eachc2[i] = (char *)malloc(1000);
    }
    int inflag = 0;
    // printf("inflag1=%d\n", inflag);
    for (int j = 0; j < noofcom; j++)
    {
        if (inflag == 0)
        {
            if ((strcmp(com[j], "<") != 0) && (com[j][0] != '<'))
                noofw1++;
        }
        else
        {
            noofw2++;
        }
        int firstinpos = 0;
        for (int i = 0; i < strlen(com[j]); i++)
        {
            if (com[j][i] == '<')
            {
                firstinpos = i + 1;
                inflag++;
                if ((strcmp(com[j], "<") != 0) && (com[j][strlen(com[j]) - 1] != '<'))
                {
                    noofw2++;
                }
                continue;
            }

            if (inflag == 0)
            {
                eachc1[j][i] = com[j][i];
            }
            if (inflag > 0)
            {
                if (firstinpos != 0)
                {
                    eachc2[noofw2 - 1][i - firstinpos] = com[j][i];
                }
                else
                {
                    eachc2[noofw2 - 1][i - firstinpos] = com[j][i];
                }
            }
        }
    }
    if (noofw2 > 1)
    {
        fprintf(stderr, "enter only one file name");
    }
    else
    {
        eachc1[noofw1] = '\0';
        int st = 0;
        int pid = fork();
        if (pid == 0)
        {
            int fw = open(eachc2[0], O_RDONLY);
            int fd = dup(fileno(stdin));
            dup2(fw, 0);
            // execvp(eachc1[0],eachc1);
            commandcheck(eachc1, noofw1, fwd, prompt);
            exit(0);
        }
        else
        {
            while (wait(&st) != pid)
                ;
        }
    }
}
void pipedcommand(char *fullpipecom, char *fwd, char *prompt, int inflag, int outflag, int pipeflag)
{
    char *fullpipecomcopy = (char *)malloc(1000);
    strcpy(fullpipecomcopy, fullpipecom);
    // printf("full  =  %s\n", fullpipecom);
    // fflush(stdout);
    char **pipcom = (char **)malloc(1000 * sizeof(char *));
    for (int i = 0; i < 1000; i++)
    {
        pipcom[i] = (char *)malloc(1000);
    }
    // printf("full  =  %s\n", fullpipecom);

    int noofc = 0;
    while (fullpipecom != NULL)
    {
        pipcom[noofc++] = strsep(&fullpipecom, "|");
        space(pipcom[noofc - 1]);
        //printf("first=%s\n",sline[noofc-1]);
    }
    // printf("full1  =  %s\n", fullpipecom);

    int fout, fin;
    char *temp1 = (char *)malloc(1000);
    char *temp2 = (char *)malloc(1000);
    char **tempr = (char **)malloc(1000 * sizeof(char *));
    for (int i = 0; i < 1000; i++)
    {
        tempr[i] = (char *)malloc(1000);
    }
    // printf("pc  =  %s\n", pipcom[0]);
    strcpy(temp2, pipcom[0]);
    int count = 0;
    for (int i = 0; i < strlen(temp2); i++)
    {
        if (temp2[i] == '<')
        {
            // printf("i=%d\n", i);
            while (temp2 != NULL)
            {
                tempr[count++] = strsep(&temp2, "<");
                space(tempr[count - 1]);
                //printf("first=%s\n",sline[noofc-1]);
            }
            // printf("fdsa=%s!\n", tempr[1]);

            // printf("i=%d\n", i);

            break;
        }
    }

    // printf("fdsa=%s!\n", tempr[0]);
    if (count != 0)
    {
        strcpy(pipcom[0], tempr[0]);
        // printf("fdsa=%s!\n", pipcom[0]);
        // fflush(stdout);
        fin = open(tempr[1], O_RDONLY);
        dup2(fin, 0);
    }
    for (int i = 0; i < noofc - 1; i++)
    {

        char *temp = (char *)malloc(1000);
        count = 0;
        for (int j = 0; j < strlen(pipcom[i]); j++)
        {
            if (pipcom[i][j] == '>')
            {
                while ((temp = strtok_r(pipcom[i], ">", tempr)))
                    count++;
                if (strstr(pipcom[i], ">>") != NULL)
                    fout = open(temp, O_WRONLY | O_CREAT | O_APPEND, 0644);
                else
                {
                    fout = open(temp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                }

                continue;
            }
        }
        char *eachc[1000];
        for (int j = 0; j < 1000; j++)
        {
            eachc[j] = (char *)malloc(1000);
        }
        int noofw = 0;
        while (pipcom[i] != NULL)
        {
            eachc[noofw++] = strsep(&pipcom[i], " ");

            //printf("%s\n",eachc[noofw-1]);
        }
        int inout[2];
        if (i != noofc - 1)
            pipe(inout);
        else
        {
            if (count == 0)
            {
                fout = -1;
            }
            inout[1] = fout;
        }
        eachc[noofw] = 0;

        execute(eachc, noofw, fwd, prompt, inout);

        close(inout[1]);
        dup2(inout[0], 0);
        // for(int j=0;j<noofw;j++)
        // {
        //     printf("%s\n",eachc[j]);
        // }
    }
    noofc--;
    char *temp = (char *)malloc(1000);
    char *temp3 = (char *)malloc(1000);

    count = 0;
    strcpy(temp3, pipcom[noofc]);
    for (int j = 0; j < strlen(temp3); j++)
    {
        // printf("j=%d\n", j);
        if (temp3[j] == '>')
        {
            // printf("j1=%d\n", j);
            while (temp3 != NULL)
            {
                tempr[count++] = strsep(&temp3, ">");
                space(tempr[count - 1]);
                //printf("first=%s\n",sline[noofc-1]);
            }
            if (strstr(pipcom[noofc], ">>") != NULL)
                fout = open(tempr[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            else
            {
                fout = open(tempr[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }

            break;
        }
    }
    // printf("count=%d\n",count);
    // printf("tempr=%s\n",tempr[0]);

    if (count == 0)
    {
        char *eachc[1000];
        for (int j = 0; j < 1000; j++)
        {
            eachc[j] = (char *)malloc(1000);
        }
        int noofw = 0;
        while (pipcom[noofc] != NULL)
        {
            eachc[noofw++] = strsep(&pipcom[noofc], " ");
        }
        int inout[2];
        inout[0] = 0;
        inout[1] = 1;
        eachc[noofw] = 0;
        // fprintf(stderr, "1\n");
        execute(eachc, noofw, fwd, prompt, inout);
        // fprintf(stderr, "2\n");
        dup2(tempin, 0);
        dup2(tempout, 1);
    }
    else
    {
        strcpy(pipcom[noofc], tempr[0]);
        dup2(fout, 1);
        char *eachc[1000];
        for (int j = 0; j < 1000; j++)
        {
            eachc[j] = (char *)malloc(1000);
        }
        int noofw = 0;
        while (pipcom[noofc] != NULL)
        {
            eachc[noofw++] = strsep(&pipcom[noofc], " ");
        }
        int inout[2];
        inout[0] = 0;
        inout[1] = 1;
        eachc[noofw] = 0;
        // fprintf(stderr, "1\n");
        execute(eachc, noofw, fwd, prompt, inout);
        // fprintf(stderr, "2\n");
        dup2(tempin, 0);
        dup2(tempout, 1);
    }

    //printf("%s\n",eachc[noofw-1]);
}

void doubleredirc(char **com, int noofcom, char **filename, int noofw, char *fwd, char *prompt, int inflag, int pipeflag)
{
    if (noofw > 1)
    {
        printf("enter only one file name");
    }
    else if (noofw == 0)
    {
        fprintf(stderr, "bash: syntax error near unexpected token `newline\'\n");
    }
    else
    {
        int fw = open(filename[0], O_WRONLY | O_CREAT, 0644);
        lseek(fw, 0, SEEK_END);
        int fd = dup(fileno(stdout));
        dup2(fw, 1);
        if (inflag == 1 && pipeflag == 0)
            inredirc(com, noofcom, fwd, prompt);
        else
            commandcheck(com, noofcom, fwd, prompt);
        fflush(stdout);
        dup2(fd, fileno(stdout));
        close(fd);
        clearerr(stdout);
    }
}
void singleredirc(char **com, int noofcom, char **filename, int noofw, char *fwd, char *prompt, int inflag, int pipeflag)
{
    if (noofw > 1)
    {
        printf("enter only one file name");
    }
    else if (noofw == 0)
    {
        fprintf(stderr, "bash: syntax error near unexpected token `newline\'\n");
    }
    else
    {
        int fw = open(filename[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        int fd = dup(fileno(stdout));
        dup2(fw, 1);
        if (inflag == 1 && pipeflag == 0)
            inredirc(com, noofcom, fwd, prompt);
        else
            commandcheck(com, noofcom, fwd, prompt);
        fflush(stdout);
        dup2(fd, fileno(stdout));
        close(fd);
        clearerr(stdout);
    }
}
void outredirc(char **eachc, int noofw, char *fwd, char *prompt, int inflag, int pipeflag)
{
    int outflag = 0;
    int doubleoutflag = 0;
    int noofw1 = 0;
    int noofw2 = 0;
    char *eachc1[1000];
    for (int i = 0; i < 1000; i++)
    {
        eachc1[i] = (char *)malloc(1000);
    }
    char *eachc2[1000];
    for (int i = 0; i < 1000; i++)
    {
        eachc2[i] = (char *)malloc(1000);
    }
    for (int j = 0; j < noofw; j++)
    {
        if (outflag == 0)
        {
            if ((strcmp(eachc[j], ">") != 0) && (strcmp(eachc[j], ">>") != 0) && (eachc[j][0] != '>'))
                noofw1++;
        }
        else
        {
            noofw2++;
        }
        int firstoutpos = 0;
        for (int i = 0; i < strlen(eachc[j]); i++)
        {
            if (i != strlen(eachc[j]) - 1)
            {
                if (eachc[j][i] == '>' && eachc[j][i + 1] == '>')
                {
                    firstoutpos = i + 2;
                    doubleoutflag++;
                    i++;
                }
            }
            if (eachc[j][i] == '>')
            {
                firstoutpos = i + 1;
                outflag++;
                if (((strcmp(eachc[j], ">") != 0) && (strcmp(eachc[j], ">>") != 0)) && (eachc[j][strlen(eachc[j]) - 1] != '>'))
                {
                    noofw2++;
                }
                continue;
            }

            if (outflag == 0)
            {
                eachc1[j][i] = eachc[j][i];
            }
            if (outflag > 0)
            {
                // printf("1");
                if (firstoutpos != 0)
                {
                    // printf("j=%d    noofw2=%d   i=%d    firs=%d\n",j,noofw2,i,firstoutpos);
                    eachc2[noofw2 - 1][i - firstoutpos] = eachc[j][i];
                }
                else
                {
                    // printf("3");
                    // printf("j=%d    noofw2=%d   i=%d    firs=%d\n",j,noofw2,i,firstoutpos);

                    eachc2[noofw2 - 1][i - firstoutpos] = eachc[j][i];
                }
            }
        }
    }
    if (doubleoutflag == 1)
        doubleredirc(eachc1, noofw1, eachc2, noofw2, fwd, prompt, inflag, pipeflag);
    else
    {
        singleredirc(eachc1, noofw1, eachc2, noofw2, fwd, prompt, inflag, pipeflag);
    }
}