/*
Write a program that calls fork() and then calls some form of
exec() to run the program /bin/ls. See if you can try all of the
variants of exec(), including (on Linux) execl(), execle(),
execlp(), execv(), execvp(), and execvpe(). Why do
you think there are so many variants of the same basic call?
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void proc(int);
void err();
void child();
void parent();
/// @brief To handle the signal event
/// @param sig 
void signalHandler(int sig)
{
    sig=sig;
    return ;
}

int main()
{

    signal(SIGCHLD, signalHandler);
    int rc = fork();
    proc(rc);
}
void proc(int rc)
{
    if (rc < 0)
        err();
    if (rc)
    {
        pause();
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
    return;
}

void parent()
{
    printf("goodbye\n");
}
void child()
{
    printf("hello\n");
}
