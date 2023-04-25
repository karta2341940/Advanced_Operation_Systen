#include <stdio.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "color.h"
#include "main.h"

#define command_length 1000

int quit = 0;
int main(int argc, char *argv[])
{

    fflush(stdout); // Clear the screen
    while (!quit)
    {
        char command[command_length], *args[command_length];

        showInfo();
        fgets(command, command_length, stdin);

        // printf("%s", command);
        split(command, args);

        isExit(args);

        int rc = fork();
        process(rc, args);
    }
}

void split(char *string, char *target[])
{
    char separator[10] = " \n";
    int index = 0;
    target[index] = strtok(string, separator);
    while (target[index] != NULL)
    {
        index++;
        target[index] = strtok(NULL, separator);
    }
    target[index] = NULL;
}
void showInfo()
{
    char cwd[command_length];
    getcwd(cwd, 500); // To get the current work direction
    printf(LIGHT_GREEN "%s\n" CYAN "%s>" NONE, cwd, getlogin());
}
void isExit(char *args[])
{
    if (strcmp(args[0], "exit") == 0)
    {
        printf("Are you sure you want to leave?(Y/n) ");
        char yn = getchar();
        getchar();
        if ('y' == yn || 'Y' == yn)
        {
            printf("Really?(Y/n) ");
            char yn2 = getchar();
            getchar();
            if ('y' == yn2 || 'Y' == yn2)
            {
                printf("Sorry this shell doesn't support exit.\n");
                return;
            }
        }
        exit(0);
    }
}
void process(int pid, char *args[])
{
    if (pid < 0)
    {
        fprintf(stderr, "FORK ERROR");
        exit(0);
    }
    if (pid)
    {
        wait(NULL);
    }
    else
    {
        char temp[1000] = "";
        char *path = "/bin/";

        strcat(temp, path);
        char *envp[2] = {"PATH=/bin", NULL};
        strcat(temp, args[0]);
        printf("%s\n", args[1]);
        execve(temp, args, envp);
    }
}
