#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <alloca.h>

const char filename[] = "q2.txt";
void err();
int parent();
int child();
int openFile();
int writeFile();
char *getTypeStr(char *type);
char *readfile(char *type);
int main()
{
    int file = open(filename, O_CREAT|O_EXCL, S_IRWXU);

    printf("pid = %d,file = %d \n", getpid(), file);
    close(file);
    int pid = fork();

    if (pid < 0)
    {
        err();
    }
    else if (pid == 0)
    {
        child();
    }
    else
    {
        parent();
    }
    wait(NULL);
}

int parent()
{

    if (!writeFile("Parent"))
    {
        return 0;
    }
    if (!strcmp(readfile("Parent"), ""))
    {
        return 0;
    }
}

int child()
{

    if (!writeFile("Child"))
    {
        return 0;
    }
    if (!strcmp(readfile("Child"), ""))
    {
        return 0;
    }
}

void err()
{
    fprintf(stderr, "Oh my goodman, fork is fail\n");
    exit(1);
}

int openFile(char *type)
{
    printf("Opening the file in %s process. My PID = %d \n", type, (int)getpid());
    int file_des = open(filename, O_RDWR|O_CREAT, S_IRWXU);
    if (file_des < 0)
    {
        printf("(%s)File open error with open return %d\n", type, file_des);
        return -1;
    }
    printf("(%s)The file has been opened.(file_des = %d)\n", type, file_des);
    return file_des;
}

int writeFile(char *type)
{
    int file_des = openFile(type);
    if (file_des == -1)
    {
        return 0;
    }
    printf("(%s)The file is writing.file_des is %d\n", type, file_des);
    char *str = getTypeStr(type);
    size_t strlength = strlen(str);
    ssize_t writeStatus = write(file_des, str, strlength);

    if (writeStatus < 0)
    {
        printf("(%s)Writing fail.\n", type);
        return 0;
    }
    printf("(%s)Writing finish. And close the file\n", type);
    close(file_des);
    return 1;
}

char *getTypeStr(char *type)
{
    if (!strcmp("Child", type))
    {
        return "I am Child.\n";
    }
    else
    {
        return "I am Parent.\n";
    }
}

char *readfile(char *type)
{
    int file_des = openFile(type);
    if (file_des < 0)
        return "";
    printf("(%s)The file is on read. file_des = %d.\n", type, file_des);
    char buf[2048];
    ssize_t readf = read(file_des, buf, 20);
    if (readf < 0)
    {
        printf("(%s)Read file fail", type);
        return "";
    }
    printf("(%s)Reading finish.the content of q2.txt is \n-------\n%s\n-------\n", type,buf);
    close(file_des);
    printf("(%s)The file is closing.",type);
    return "";
}
