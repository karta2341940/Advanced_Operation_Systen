/*
Write a program that calls fork() and then calls some form of
exec() to run the program /bin/ls. See if you can try all of the
variants of exec(), including (on Linux) execl(), execle(),
execlp(), execv(), execvp(), and execvpe().
Why do you think there are so many variants of the same basic call?
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void proc(int, char *[], int);
void err();
void child(char *[], int);
void parent();

const char *path = "/bin/ls";
int main(int argc, char *argv[])
{
    printf("The argc is %d\n", argc);
    int rc = fork();
    proc(rc, argv, argc);
}
void proc(int rc, char *argv[], int argc)
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
        child(argv, argc);
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
    printf("Child\n");
    int i = 0;
    for (i = 0; i < argc; i++)
    {
        
        printf("%d - ARG : %s\n", i,argv[i]);
        if (strcmp(argv[i], "execl")==0)
        {
            printf("execl\n");
            execl(path, "ls", "-al", NULL);
            return 0;
        }
        if (!strcmp(argv[i], "execlp"))
        {
            printf("execlp\n");
            execlp("ls", "ls", "-al", NULL);
            return 0;
        }
        if (!strcmp(argv[i], "execle"))
        {
            printf("execle\n");
            execle(path, "ls", "-al", NULL,NULL);
            return 0;
        }
        if (!strcmp(argv[i], "execv"))
        {
            printf("execv\n");
            const char *av[] = {"ls", "-l", NULL};
            execv(path, av);
            return 0;
        }
        if (!strcmp(argv[i], "execvp"))
        {
            printf("execvp\n");
            const char *av[] = {"ls", "-l", NULL};
            execvp("ls", av);
            return 0;
        }
        if (!strcmp(argv[i], "execvpe"))
        {
            printf("execvp\n");
            const char *av[] = {"ls", "-l", NULL};
            execvpe("ls", av,NULL);
            return 0;
        }
    }
}
