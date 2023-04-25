#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LENGTH 1024

int main(int argc, char *argv[]) {
    char input[MAX_LENGTH];
    char *args[MAX_LENGTH/2 + 1];
    int should_run = 1;

    while (should_run) {
        printf("shell> ");
        fflush(stdout);

        // read input
        fgets(input, MAX_LENGTH, stdin);

        // tokenize input into arguments
        int i = 0;
        args[i] = strtok(input, " \n");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " \n");
        }
        args[i] = NULL;

        // execute command
        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
        } else {
            pid_t pid = fork();
            if (pid < 0) {
                fprintf(stderr, "fork failed\n");
            } else if (pid == 0) {
                execvp(args[0], args);
                fprintf(stderr, "exec failed\n");
                exit(1);
            } else {
                wait(NULL);
            }
        }
    }

    return 0;
}
