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


int main(int argc, char *argv[])
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
        wait(NULL);
    }
    else
    {
        
    }
}
void err()
{
    printf("Something wrong\n");
    exit(1);
}
void parent()
{
    printf("Parent\n");
}
void child(char *argv[], int argc)
{
   
}




