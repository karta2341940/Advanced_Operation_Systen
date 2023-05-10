/*
Create a program that allocates an array of integers (as above), frees
them, and then tries to print the value of one of the elements of
the array. Does the program run? What happens when you use
valgrind on it?
*/


#include <stdio.h>
#include <stdlib.h>
int main()
{
    int *data = (int *)malloc(100 * sizeof(int));
    free(data);
    printf("data[0] = %d\n", data[0]);
    return 0;
}