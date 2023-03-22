#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIZE 1024

void errorCheck(int fd)
{
    if (fd < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int)getpid());
    int pfd[2];
    int nread;
    char buf[SIZE];
    char value[SIZE];
    if (pipe(pfd) == -1)
    {
        perror("pipe failed");
        exit(1);
    }
    printf("pipe ready. Start fork\n");


    int rc = fork();
    int rc_2 = 0;
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        // child process
        printf("child1 execution (write) \n");
        strcpy(buf, "Hello This is write form child1");
        if (write(pfd[1], buf, strlen(buf) + 1) != strlen(buf) + 1)
        {
            printf("Error writing to pipe.\n");
            exit(1);
        }

        if (close(pfd[1]) == -1)
        {
            printf("Error closing reading end of the pipe.\n");
            exit(1);
        }
        // child2 start fork
        rc_2 = fork();
        if (rc_2 < 0)
        {
            fprintf(stderr, "fork2 failed\n");
            exit(1);
        }
        if (rc_2 == 0)
        {
            // child2 process
            printf("child2 execution (read)\n");
            nread = read(pfd[0], value, SIZE);
            if (nread == -1)
            {
                printf("Error reading from pipe.\n");
                exit(1);
            }
            // while((nread = read(pfd[0],buf,SIZE))!=0){
            printf("child2 read %s\n", value);
            //}
            if (close(pfd[0]) == -1)
            {
                printf("Error closing writing end of the pipe.\n");
                exit(1);
            }
            exit(1);
        }
        else
        {
            printf("child1 wait (read)\n");
            wait(NULL);
            exit(1);
        }
    }
    else
    {
        // parent process
        wait(NULL);
        printf("parent ending.\n");
    }
}