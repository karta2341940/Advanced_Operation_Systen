/*
Write a program that opens a file (with the open() system call)
and then calls fork() to create a new process. Can both the child
and parent access the file descriptor returned by open()? What
happens when they are writing to the file concurrently, i.e., at the
same time?
*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <alloca.h>
#define bool int
#define true 1
#define false 0


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
    remove(filename);
    printf("Program Start. pid = %d\n",getpid());
    
    int pid = fork();

    if (pid < 0)
    {
        err();
    }
    else if (pid == 0)
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
    else
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
}

void err()
{
    fprintf(stderr, "Oh my goodman, fork is fail\n");
    exit(1);
}
/// @brief To open the file.
/// @param type
/// @param readable if true => read,false =>write
/// @return
int openFile(char *type, bool readable)
{
    printf("(%s)Opening the file. The process. My PID = %d \n", type, (int)getpid());
    int file_des = -1;
    if (readable)
    {
        file_des = open(filename, O_RDONLY | O_CREAT, S_IRWXU);
    }
    else
    {
        file_des = open(filename, O_WRONLY | O_CREAT|O_APPEND , S_IRWXU);
    }
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
    int file_des = openFile(type, false);
    if (file_des == -1)
    {
        return 0;
    }
    char *str = getTypeStr(type);
    size_t strlength = strlen(str);
    ssize_t writeStatus = write(file_des, str, strlength);

    printf("(%s)The file is writing. file_des is %d. And the writeStatus is %ld\n", type, file_des, writeStatus);
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
        return "I am Child.\n\0";
    }
    else
    {
        return "I am Parent.\n\0";
    }
}

char *readfile(char *type)
{
    
    int file_des = openFile(type, true);
    if (file_des < 0)
        return "";
    printf("(%s)The file is on read. file_des = %d.\n", type, file_des);
    char buf[2048];
    ssize_t readf = read(file_des, buf, 23);
    if (readf < 0)
    {
        printf("(%s)Read file fail\n", type);
        return "";
    }
    printf("(%s)Reading finish.the content of q2.txt is \n-------\n%s\n-------\n", type, buf);
    close(file_des);
    printf("(%s)The file is closing.\n", type);
    
    return "";
}
