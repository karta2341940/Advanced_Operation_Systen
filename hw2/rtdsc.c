#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

uint64_t rdtsc()
{
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc\n\t"
                         : "=a"(lo), "=d"(hi)); // low32-bit store in EAX register，high32-bit store in EDX register
    return ((uint64_t)hi << 32) | lo;
}
double

int main()
{
    uint64_t start, end, cycles;
    double seconds, nano;
    start = rdtsc(); // get start cpu cycles
    /* call the function */
    int n = 1, fd;
    char c[10];
    fd = open("file", O_RDONLY);
    while (n--)
    {
        read(fd, c, 0);
    }
    /* end */
    end = rdtsc();                                // get end cpu cycles
    cycles = end - start;                         // calcuate CPU cycle
    seconds = (double)cycles / (double)210000000; // calcuate real time -> formula :cycle / 2.1GHz # 2.1GHz because ws's CPU us 2.1GHz
    nano = (double)cycles / (double)2.1;
    printf("Cycles: %lu \n", cycles);
    printf("Seconds: %lfs\n", seconds);
    printf("Nano Seconds: %lfns\n", nano);
}