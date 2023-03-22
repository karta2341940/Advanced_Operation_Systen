#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

uint64_t rdtsc()
{
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc\n\t"
                         : "=a"(lo), "=d"(hi));
    // low32-bit store in EAX register，high32-bit store in EDX register
    /* This code in asm like following
    rdtsc ; read time-stamp counter into edx:eax
    mov lo, eax ; store low 32 bits into lo
    mov hi, edx ; store high 32 bits into hi
    */
    return ((uint64_t)hi << 32) | lo;
}
uint64_t m_start(int s)
{
    printf("----------Measure Start%d------------\n", s);
    uint64_t start = rdtsc();
    return start;
}
double m_end(uint64_t start)
{
    uint64_t end, cycle;
    double sec, nanoSec;
    end = rdtsc();
    cycle = end - start; // calcuate CPU cycle
    sec = (double)cycle / (double)210000000;
    nanoSec = (double)cycle / (double)2.1;
    printf("Cycle : %lu \n", cycle);
    printf("Second: %lfs\n", sec);
    printf("Nano Second: %lfns\n", nanoSec);
}
void load()
{
    int n = 1, fd;
    char c[10];
    fd = open("file", O_RDONLY);
    while (n--)
    {
        read(fd, c, 0);
    }
    printf("%s", c);
}

void s_call2()
{
    printf("My pid is %d.\n", (int)getpid());
}

void s_call3()
{
    int rc = fork();
    if (rc < 0)
    {
        exit(-1);
    }
    if (rc)
    {
        printf("\t--ls result--\n\n\n");
        wait(NULL);
        printf("\n\n\n\t--ls result--\n");
    }
    else
    {
        execl("/bin/ls", "ls", NULL);
    }

    return 0;
}

int main()
{
    uint64_t start;
    // Measure 1
    start = m_start(1);
    load();
    m_end(start);
    // Measure 2
    start = m_start(2);
    s_call2();
    m_end(start);
    // Measure 3
    start = m_start(3);
    s_call3();
    m_end(start);
}
