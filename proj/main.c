#include "color.h"
#include "main.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include <wait.h>

#define command_length 1000

extern char **environ;

struct termios orig_termios;
void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
// Move cursor left
void move_cursor_left()
{
    printf("\033[D");
}
// Move cursor right
void move_cursor_right()
{
    printf("\033[C");
}

int quit = 0;
int main(int argc, char *argv[])
{

    showHello();
    //enableRawMode();
    while (!quit)
    {
        char command[command_length] = {}, *args[command_length] = {};

        showInfo();

        fgets(command, command_length, stdin);

        char *input = strdup(command);
        split(command, args, " \n");
        isExit(args);

        if (cd(args))
            continue;
        if (classify(args, input))
            continue;
        process(args);
    }
}
void output_redirect(char *input)
{
    printf("output:%s\n", input);
    char *argsOut[command_length] = {}, *argsExec[command_length] = {};

    split(input, argsOut, ">");

    split(argsOut[0], argsExec, " \n");
    // To take out the space front of the string.
    for (int i = 0; i < strlen(argsOut[1]); i++)
    {
        if (argsOut[1][i] == 32)
        {
            argsOut[1]++;
        }
        else
        {
            break;
        }
    }
    int originOUT = dup(STDOUT_FILENO);
    close(STDOUT_FILENO);
    int fd = open(argsOut[1], O_CREAT | O_RDWR, S_IRWXU);

    int pid = fork();
    if (pid < 0)
    {
        fprintf(stdout, "Error");
        exit(0);
    }
    if (pid)
    {
        wait(NULL);
        dup2(originOUT, fd);
    }
    else
    {
        execvp(argsExec[0], argsExec);
        fprintf(stderr, "No this command.\n");
        exit(1);
    }
}

void input_redirect(char *input)
{
    char *argsIn[command_length] = {}, *argsExec[command_length] = {};

    split(input, argsIn, "<");

    split(argsIn[0], argsExec, " \n");
    argsIn[1][strlen(argsIn[1]) - 1] = '\0';

    for (int i = 0; i < strlen(argsIn[1]); i++)
    {
        if (argsIn[1][i] == 32)
        {
            argsIn[1]++;
        }
    }
    char *fileName = argsIn[1];

    int originIn = dup(STDIN_FILENO);

    close(STDIN_FILENO);
    int fd = open(fileName, O_RDWR, S_IRWXO);
    if (fd < 0)
    {
        fprintf(stderr, "Can't open this file \"%s\" with error code %d\n", fileName, fd);
        return;
    }

    int pid = fork();
    if (pid < 0)
    {
        fprintf(stdout, "Error");
        exit(0);
    }
    if (pid)
    {
        wait(NULL);
        dup2(originIn, fd);
    }
    else
    {
        execvp(argsExec[0], argsExec);
        fprintf(stderr, "No this command.\n");
        exit(1);
    }
}

void exec_pipe(char *input)
{
    char *argsPipe[command_length] = {}, *argsExec1[command_length] = {}, *argsExec2[command_length] = {};

    split(input, argsPipe, "|");

    split(argsPipe[0], argsExec1, " \n");
    split(argsPipe[1], argsExec2, " \n");
    argsPipe[1][strlen(argsPipe[1]) - 1] = '\0';

    // To take out the space front of the string.
    for (int i = 0; i < strlen(argsPipe[1]); i++)
    {
        if (argsPipe[1][i] == 32)
        {
            argsPipe[1]++;
        }
        else
        {
            break;
        }
    }
    int oringinIn = dup(stdin);
    int oringinOut = dup(stdout);
    int pipeTunnel[2] = {};
    int isSuccess = pipe(pipeTunnel);
    char source = argsPipe[0];
    char *target = argsPipe[1];

    // If pipe fail, print error message and exit.
    if (isSuccess < 0)
    {
        fprintf(stderr, "Pipe Error");
        exit(1);
    }

    // Create a new process to execute the pipe
    int pid, pid2;
    (pid = fork()) && (pid2 = fork());
    if (pid < 0 || pid2 < 0)
    {
        fprintf(stderr, "Fork Error");
        exit(1);
    }
    if (!pid)
    {
        dup2(pipeTunnel[1], 1);
        close(pipeTunnel[0]);
        close(pipeTunnel[1]);
        execvp(argsExec1[0], argsExec1);
    }
    else if (!pid2)
    {
        dup2(pipeTunnel[0], 0);
        close(pipeTunnel[0]);
        close(pipeTunnel[1]);
        execvp(argsExec2[0], argsExec2);
    }
    else
    {
        wait(NULL);
        close(pipeTunnel[1]);
        close(pipeTunnel[0]);
    }
}

int classify(char **args, char *input)
{
    int isIn = 0;
    for (int i = 0; args[i]; i++)
    {

        for (int j = 0; args[i][j]; j++)
        {
            switch (args[i][j])
            {
            case '>':
                isIn++;
                output_redirect(input);
                break;
            case '<':
                isIn++;
                input_redirect(input);
                break;
            case '|':
                isIn++;
                exec_pipe(input);
                break;
            }
        }
    }
    return isIn;
}

void split(char *str, char *target[], char *separator)
{

    int index = 0;
    char *string = strdup(str);
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
    char *home = getData("HOME=");

    getcwd(cwd, 500); // To get the current work direction

    printf(LIGHT_GREEN "%s\n" CYAN "%s> " NONE, replaceFirst(cwd, home, "~"), getData("USER="));
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

void process(char *args[])
{
    int pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "FORK ERROR\n");
        exit(0);
    }
    if (pid)
    {
        wait(NULL);
    }
    else
    {
        execvp(args[0], args);
        fprintf(stderr, "No this command.\n");
        exit(1);
    }
}

void showEnvironment()
{
    for (int i = 0; environ[i]; i++)
    {
        printf("*environ[%d]: %s\n", i, environ[i]);
    }
}

char *getData(char *str)
{
    char pathStr[1000] = "";
    for (int i = 0; environ[i]; i++)
    {
        int length = strlen(str);
        if (!length)
        {
            break;
        }
        char *env = environ[i];
        for (int j = 0; env[j]; j++)
        {
            if (env[j] == str[j] || !length)
            {
                if (length)
                {
                    length--;
                }
                else
                {
                    append(pathStr, env[j]);
                }
            }
            else
            {
                break;
            }
        }
    }
    return strdup(pathStr);
}

void append(char *str, char c)
{
    int i;
    for (i = 0; str[i]; i++)
    {
    }
    str[i] = c;
}

char *replaceFirst(char *source, char *target, char *replacement)
{

    int start = 0, end = 0;
    int contained = isContain(source, target, &start, &end);
    if (!contained)
    {
        return strdup(source);
    }

    int tatal = strlen(source) - strlen(target) + strlen(replacement) + 1;

    char *temp = malloc(tatal);
    // Initial the temp
    memset(temp, NULL, tatal);
    // Copy the content before the target
    strncpy(temp, source, start);
    // concat the the replacement
    strcat(temp, replacement);
    // Concat the the part after the target
    strcat(temp, source + end);

    return strdup(temp);
}

int isContain(char *source, char *target, int *startPosition, int *endPosition)
{
    // if the length of target is large then source
    // that return 0;
    if (strlen(source) < strlen(target))
    {
        return 0;
    }

    int length = strlen(target), match = 0;
    for (int i = 0; source[i]; i++)
    {
        if (source[i] == target[i])
        {
            if (!match)
            {
                *startPosition = i;
            }
            match = 1;
            length--;
            if (!length)
            {
                *endPosition = i + 1;
                return 1;
            }
        }
        else
        {
            if (match)
            {
                return 0;
            }
        }
    }
    return 0;
}

void identify(char *command)
{
    if (!strcmp(command, ESC))
    {
        printf("Special\n");
    }
    if (!strcmp(command, UP))
    {
        printf("Special\n");
    }
    if (!strcmp(command, DOWN))
    {
        printf("Special\n");
    }
    if (!strcmp(command, LEFT))
    {
        printf("Special\n");
    }
    if (!strcmp(command, RIGHT))
    {
        printf("Special\n");
    }
}

int cd(char **arg)
{
    if (!strcmp(arg[0], "cd"))
    {
        for (int i = 0; i < arg[1][i]; i++)
        {
            if (arg[1][i] == '~')
            {
                char *home = getData("HOME=");
                arg[1] = replaceFirst(arg[1], "~", home);
            }
        }
        int iscd = chdir(arg[1]);
        if (iscd)
        {
            printf("No this folder\n");
        }
        return 1;
    }
    return 0;
}

void showHello()
{

    const char *data0 = "   _____          _ _               ______                        _             ";
    const char *data1 = "  / ____|        | (_)             |  ____|                      | |            ";
    const char *data2 = " | |     ___   __| |_ _ __   __ _  | |____   _____ _ __ _   _  __| | __ _ _   _ ";
    const char *data3 = " | |    / _ \\ / _` | | '_ \\ / _` | |  __\\ \\ / / _ \\ '__| | | |/ _` |/ _` | | | |";
    const char *data4 = " | |___| (_) | (_| | | | | | (_| | | |___\\ V /  __/ |  | |_| | (_| | (_| | |_| |";
    const char *data5 = "  \\_____\\___/ \\__,_|_|_| |_|\\__, | |______\\_/ \\___|_|   \\__, |\\__,_|\\__,_|\\__, |";
    const char *data6 = "  _    _                     __/ | ______                __/ |   _         __/ |";
    const char *data7 = " | |  | |                   |___/ |  ____|              |___/   | |       |___/ ";
    const char *data8 = " | |__| | __ _ _ __  _ __  _   _  | |____   _____ _ __ _   _  __| | __ _ _   _  ";
    const char *data9 = " |  __  |/ _` | '_ \\| '_ \\| | | | |  __\\ \\ / / _ \\ '__| | | |/ _` |/ _` | | | | ";
    const char *data10 = " | |  | | (_| | |_) | |_) | |_| | | |___\\ V /  __/ |  | |_| | (_| | (_| | |_| | ";
    const char *data11 = " |_|  |_|\\__,_| .__/| .__/ \\__, | |______\\_/ \\___|_|   \\__, |\\__,_|\\__,_|\\__, | ";
    const char *data12 = "              | |   | |     __/ |                       __/ |             __/ | ";
    const char *data13 = "              |_|   |_|    |___/                       |___/             |___/  ";
    printf("%s\n", data0);
    printf("%s\n", data1);
    printf("%s\n", data2);
    printf("%s\n", data3);
    printf("%s\n", data4);
    printf("%s\n", data5);
    printf("%s\n", data6);
    printf("%s\n", data7);
    printf("%s\n", data8);
    printf("%s\n", data9);
    printf("%s\n", data10);
    printf("%s\n", data11);
    printf("%s\n", data12);
    printf("%s\n", data13);
}