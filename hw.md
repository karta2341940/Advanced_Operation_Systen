# HW1
# Advanced Operation System, Spring 2023
## Department of Computer Science and Information Engineering National Pingtung University

## Meng Xian, Du(CBB108047)

Write a program that calls fork(). 
Before calling fork(), have the main process access a variable (e.g., x)and set its value to something (e.g., 100).

What value is the variable in the child process?

What happens to the variable when both the child and parent change the value of x?

## Solution: Please refer to q1.c
### Q1.c :
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int x = 100;
    printf("X = %d \n", x);
    printf("pid = %d \n", getpid());
    int pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Oh my goodman, fork is fail\n");
        exit(1);
    }
    else if (pid == 0)
    {
        x = 1;
        printf("Oh my friend, I am child process.\t");
        printf("My PID is %d and Now x = %d\n", getpid(), x);
    }
    else
    {
        x = 2;
        printf("Oh my child, I am parent process.\t");
        printf("My PID is %d and Now x = %d\n", getpid(), x);
        wait(NULL);
        printf("\n--------------\n");
    }
}
```

## Q1 Result
```bash
X = 100 
pid = 24854 
Oh my child, I am parent process.       My PID is 24854 and Now x = 2
Oh my friend, I am child process.       My PID is 24855 and Now x = 1

--------------
```
We can see at the beginning of main function I declare a variable x.
and the 













