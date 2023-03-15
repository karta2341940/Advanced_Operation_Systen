/*
Write a program that calls fork(). 
Before calling fork(), have the main 
process access a variable (e.g., x) 
and set its value to something (e.g., 100).
 What value is the variable in the child process? 
 What happens to the variable when both the child 
 and parent change the value of x?
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int x = 100;
    printf("X = %d \n", x);
    printf("pid = %d \n", getpid());
    int pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Oh my goodman, fork is fail\n");
        exit(1);
    }
    else if (pid == 0)
    {
        x = 1;
        printf("Oh my friend, I am child process.\t");
        printf("My PID is %d and Now x = %d\n", getpid(), x);
    }
    else
    {
        x = 2;
        printf("Oh my child, I am parent process.\t");
        printf("My PID is %d and Now x = %d\n", getpid(), x);
        wait(NULL);
        printf("\n--------------\n");
    }
}