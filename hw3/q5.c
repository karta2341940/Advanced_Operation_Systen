/*
Write a program that creates an array of integers called data
of size 100 using malloc; then, set data[100] to zero. What happens
when you run this program? What happens when you run this
program using valgrind? Is the program correct?
*/


#include <stdio.h>
#include <stdlib.h>
int main()
{
    int *data = (int *)malloc(100 * sizeof(int));
    data[100] = 0;
    printf("data[100] = %d\n", data[100]);
    return 0;
}