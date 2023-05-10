/*
First, write a simple program called null.c that creates a pointer
to an integer, sets it to NULL, and then tries to dereference it.
Compile this into an executable called null. What happens when you
run this program?
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *p = malloc(sizeof(int));
    printf("Start\n");
    printf("The address of p is %p\n", p);
    printf("The value of p is %d\n", *p);
    printf("End\n");
    return 0;
}