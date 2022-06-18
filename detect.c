#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "detectHypervisor.h"

#define C 100000
#define USAGE "help\n"

int main(int argc, char **argv)
{
    // -v   verbose
    // -i:  iteration count
    // -t:  threshold
    // -m:  mode (asm vs. syscall)
    // -h   help
    
    int option;
    int verbose = 0;
    int iterations = C;
    char *mode = "ASM";
    int customThreshold = -1;    

    while ((option = getopt(argc, argv, ":vi:t:m:h")) != -1) {
        switch (option) {
            case 'v':
                verbose = 1;
                break;

            case 'i':
                iterations = atoi(optarg);
                break;

            case 't':
                customThreshold = atoi(optarg);
                break;

            case 'm':
                if (strcasecmp(optarg, "ASM") == 0 || strcasecmp(optarg, "SYSCALL") == 0) {
                    mode = optarg;
                } else {
                    mode = NULL;
                    printf("Mode %s unknown.\n", optarg);
                    return 1;
                }
                break;
            case 'h':
                printf(USAGE);
                return 0;
                break;

            case '?':
            default:
                if (optopt == 'i' || optopt == 't' || optopt == 'm') {
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                } else {
                    printf("Unknown option, place check the avaailable options with -h");
                }
                return 1;
        }
    }
    
    float ratio;
    int isHypervisor;

    if (strcasecmp(mode, "ASM") == 0) {
        if (customThreshold > 0) {
            isHypervisor = detect_hypervisor_asm_customThreshold(&ratio, iterations, verbose, customThreshold);
        } else {
            isHypervisor = detect_hypervisor_asm(&ratio, iterations, verbose);
        }

    } else {
        if (customThreshold > 0) {
            isHypervisor = detect_hypervisor_syscall_customThreshold(&ratio, iterations, verbose, customThreshold);
        } else {
            isHypervisor = detect_hypervisor_syscall(&ratio, iterations, verbose);
        }
    }

    printf("%d - %s\n", isHypervisor, isHypervisor ? "Hypervisor detected" : "Native host detected");

    // fingerprint strats
}
