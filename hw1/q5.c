/*
Now write a program that uses wait() to wait for the child process
to finish in the parent. What does wait() return? What happens if
you use wait() in the child?
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void proc(int);
void err();
void child();
void parent();

int main()
{
    int rc = fork();
    proc(rc);
}
void proc(int rc)
{
    if (rc < 0)
        err();
    if (rc)
    {
        parent();
    }
    else
    {
        child();
    }
}
void err()
{
    printf("Something wrong\n");
    exit(1);
}
void parent()
{
    printf("(Parent) My pid is %d\n", getpid());
    int wait_value = wait(NULL);
    printf("(Parent)The return value of wait is %d\n", wait_value);
    printf("(Parent)So we can know wait function return the pid of child process.\n");
}
void child()
{
    printf("(Child) My pid is %d\n", getpid());
    int wait_value = wait(NULL);
    printf("(CHild)The return value of wait is %d\n", wait_value);
    printf("\nAccording to the document of wait() said \n\"For errors, return (pid_t) -1\"\n so we can know use wait() in child process will occurs error \n if the child process doesn't have another child process.\n");
}
