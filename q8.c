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

int main()
{
    int pid1 = fork();
    int pid2 = fork();
}
