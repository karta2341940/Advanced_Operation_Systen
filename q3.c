/*
Write another program using fork(). The child process should print “hello”;
the parent process should print “goodbye”.
You should try to ensure that the child process always prints first;
can you do this without calling wait() in the parent?
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int proc(int);
void err();
void child();
void parent();

int main(int argc, char *argv[])
{
    int rc = fork();
    proc(rc);
}
int proc(int rc)
{
    if (rc < 0)
        err();
    if (rc)
        parent();
    else
        child();
}
void err()
{
    printf("Something wrong\n");
    return;
}

void parent(){

}
void child(){
    
}













