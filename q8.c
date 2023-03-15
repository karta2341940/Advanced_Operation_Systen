/*
Write a program that creates two children,
and connects the standard output of one to
the standard input of the other,using the
pipe() system call.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

void err()
{
    fprintf(stderr, "Fork err");
    exit(1);
}

int main()
{
    char content[50]="";
    int p[2];
    if (pipe(p) < 0)
    {
        perror("Pipe err");
        exit(1);
    }
    int pid = fork();
    if (pid < 0)
        err();
    if (!pid)
    {
        int pid2 = fork();
        if (pid2 < 0)
            err();
        if (!pid2)
        {
            strcpy(content,"Hello from sub 2");
            write(p[1],content,50);
        }
        wait(NULL);
        char ct[50];
        read(p[0],ct,50);
    }
    else
    {
        printf("This is parent\n");
        wait(NULL);
    }
    return 1;
}
