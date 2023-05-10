/*
Now pass a funny value to free (e.g., a pointer in the middle of the
array you allocated above). What happens? Do you need tools to
find this type of problem?
*/

#include <stdio.h>
#include <stdlib.h>
int main()
{
    int *data = (int *)malloc(3 * sizeof(int));
    data[0] = 1;

    free(data + 1);
    printf("data[0] = %d\n", data[0]);
    return 0;
}