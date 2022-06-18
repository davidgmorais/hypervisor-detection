#include <sys/time.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define C 100000
#define M 10000

#define UNROLL_10_TIMES(x) x x x x x x x x x x
#define UNROLL_100_TIMES(x) UNROLL_10_TIMES(UNROLL_10_TIMES(x))
#define UNROLL_10000_TIMES(x) UNROLL_100_TIMES(UNROLL_100_TIMES(x))

#define NOP asm volatile ("nop;");

int main(int argc, char const *argv[])
{
    struct timeval startTime;
    struct timeval endTime;

    int starting=10000, range=50000, step=100;
    char filename[256];
    FILE* fd;
    sprintf(filename, "out_%d_%d_%d.csv", starting, range, step);

    if (argc >= 2) starting = atoi(argv[1]);
    if (argc >= 3) range = atoi(argv[2]);
    if (argc >= 4) step = atoi(argv[3]);
    if (argc >= 5) sprintf(filename, "%s", argv[4]);

    int alloc = (range-starting)/step;
    long results[alloc];

    printf("------------------------------------\n");
    printf("Suggesting N with\n    -> starting value: %d\n    -> range: %d\n    -> step: %d\n    -> output: %s\n", starting, range, step, filename);
    printf("------------------------------------\n\n");

    // warmup
    for (int warmup=0; warmup <= range*10; warmup++) {
        UNROLL_10000_TIMES(NOP);
    }

    int n = starting - step;
    for (int i=0; n <= range; i++) {
        long sampleTime;
        long minInstructionTime = LONG_MAX;
        n += step;

        // performance measures for NOP instruction
        for (int it=1; it <= C; it++) {
            gettimeofday(&startTime, NULL);
            for (int i=0; i < n/M; i++) {
                UNROLL_10000_TIMES(NOP);
            }
            gettimeofday(&endTime, NULL);
        
            sampleTime = ((endTime.tv_sec * 1e6 + endTime.tv_usec) - (startTime.tv_sec * 1e6 + startTime.tv_usec));
            if (sampleTime < minInstructionTime) {
                minInstructionTime = sampleTime;
            }
        }

        results[i] = (minInstructionTime * 1e6) / n;
        printf("[N = %d] (%.0f%%) - %ld ps\n", n, (i/((float) alloc))*100, results[i]);
    }

    fd = fopen(filename, "w");
    if (fd == NULL) {
        printf("ERROR opening output file %s\n", filename);
        return 1;
    }

    fprintf(fd, "%s\t%s\n", "N", "Execution time (ps)");
    for (int i =0; i< alloc; i++) {
        fprintf(fd, "%d\t%ld\n", starting + i*step, results[i]);
    }
    fclose(fd);

    return 0;
}
