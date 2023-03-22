/*
Write a program that creates a child process,
and then in the child closes standard output
(STDOUT FILENO). What happens if the child
calls printf() to print some output after
closing the descriptor?
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

void proc(int);
void err();
void parent();
void child();

int main()
{
    int pid = fork();
    proc(pid);
    if (pid == getpid())
    {
        printf("\n---------------Program END---------------\n");
    }
}
void proc(int pid)
{
    switch (pid)
    {
    case 0:
        child();
        break;
    case -1:
        err();
        break;

    default:
        parent();
        break;
    }
}
void err()
{
    printf("Error");
    exit(-1);
}

void parent()
{
    printf("(Parent)I am parent\n");
    wait(NULL);
    printf("(Parent)Parent END\n");
}
void child()
{
    printf("(Child)Before CLOSE\n");
    close(STDOUT_FILENO);
    open("q7.txt", O_RDWR | O_TRUNC | O_CREAT, S_IRWXU);
    printf("(Child)After CLOSED\n");
    printf("(Child)Child END\n");
}
