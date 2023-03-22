/*
Write another program using fork(). The child process should print “hello”;
the parent process should print “goodbye”.
You should try to ensure that the child process always prints first;
can you do this without calling wait() in the parent?
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
    /*
        When child proc status has change. 
        Signal sent to parent process, 
        and call signalHandler to deal with the event.
    */
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
