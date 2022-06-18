#include <stdio.h>
#include <detectHypervisor.h>

#define RUN_TIMES 100
#define WARMUP 10
#define C 1000
#define SYSCALL 1

#ifdef SYSCALL
#define COLLECT detect_hypervisor_syscall(&ratio, C, 0)
#else
#define COLLECT detect_hypervisor_asm(&ratio, C, 0)
#endif



int main(int argc, char const *argv[])
{
    float ratios[RUN_TIMES];
    float ratio;

    printf("-------- Data Collection --------\n");
    printf(" Iteration count: %d\n", C);
    printf(" Times runned: %d\n", RUN_TIMES);
    printf(" Used N: %d\n", N);
    printf("----------------------------------\n\n");

    printf("Warming up..\n");
    for (int i=0; i < WARMUP; i++) {
        COLLECT;
    }

    printf("\rData collection in progress");
    for (int i=0; i < RUN_TIMES; i++) {
        printf("\rData collection in progress - %.0f%%", ((float)i/(float)RUN_TIMES)*100);
        COLLECT;
        ratios[i] = ratio;
        fflush(stdout);
    }
    printf("\rData collection completed successfully\n");

    FILE *fd;
    char filename[] = "collected.csv";
    fd = fopen(filename, "w");

    if (fd != NULL) {

        fprintf(fd, "Iteration count: %d\n", C);
        fprintf(fd, "Times runned: %d\n", RUN_TIMES);
        fprintf(fd, "Used N: %d\n\n", N);

        for (int i=0; i < RUN_TIMES; i++) {
            fprintf(fd, "%f\n", ratios[i]);
        }
        printf("Output stored in %s\n", filename);
    }

    return 0;
}
