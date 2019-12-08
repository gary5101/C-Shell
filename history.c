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

void history_store(char *command,char *fwd)
{
    FILE *fp;
    char* temp =(char *)malloc(1000);
    strcpy(temp,fwd);
    strcat(temp,"/history.txt");
    fp = fopen(temp, "a");
    if (fp == NULL)
    {
        perror("");
        return;
    }
    fseek(fp, 0, SEEK_END);
    fprintf(fp, "%s", command);
    fprintf(fp, "%s", "\n");
    fclose(fp);
}
void history_print(char *fwd,char **hiscommand, int noofw)
{
    FILE *fp;
    char* temp =(char *)malloc(1000);
    strcpy(temp,fwd);
    strcat(temp,"/history.txt");
    fp = fopen(temp, "r");
    if (fp == NULL)
    {
        perror("");
        return;
    }
    if (noofw == 1)
    {
        int nooflines=10;
        sscanf(hiscommand[1], "%d", &nooflines);
        char ch;
        int num = 0;
        while (!feof(fp))
        {
            ch = fgetc(fp);
            if (ch == '\n')
                num++;
        }
        fseek(fp, 0, SEEK_SET);
        char line[1000];
        char *c = 0;
        int count = 0;
        if (nooflines > num)
        {
            nooflines = num;
        }

        do
        {
            c = fgets(line, 100, fp);
            if ((c != NULL) && (count >= num - nooflines))
                printf("%s", line);
                count++;
        } while (c != NULL);
    }
    else if (noofw == 2)
    {
        int nooflines;
        sscanf(hiscommand[1], "%d", &nooflines);
        char ch;
        int num = 0;
        while (!feof(fp))
        {
            ch = fgetc(fp);
            if (ch == '\n')
                num++;
        }
        fseek(fp, 0, SEEK_SET);
        char line[1000];
        char *c = 0;
        int count = 0;
        if (nooflines > num)
        {
            nooflines = num;
        }

        do
        {
            c = fgets(line, 100, fp);
            if ((c != NULL) && (count >= num - nooflines))
                printf("%s", line);
                count++;
        } while (c != NULL);
    }
    else
    {
        printf("incorrect no of arguments for history");
    }
}
