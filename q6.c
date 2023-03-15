/*
Write a slight modification of the previous program,
this time using waitpid() instead of wait().
 When would waitpid() be useful?
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

void proc(int);
void err();
void child(int);
void parent(int);

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
        parent(rc);
    }
    else
    {
        child(rc);
    }
}
void err()
{
    printf("Something wrong\n");
    exit(1);
}
void parent(int rc)
{
    int status;
    printf("(Parent) My pid is %d\n", getpid());
    int wait_value = waitpid(rc,&status,0);
    printf("(Parent)The return value of wait is %d\n", wait_value);
}
void child(int rc)
{
    int status;
    printf("(Child) My pid is %d\n", getpid());
    int wait_value = waitpid(rc,&status,WNOHANG);
    printf("(CHild)The return value of wait is %d\n", wait_value);
}