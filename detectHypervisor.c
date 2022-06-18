#include <sys/time.h>
#include <time.h>
#include <stdio.h> 
#include <unistd.h>
#include <limits.h>
#include "detectHypervisor.h"


float measure_baseline(int iterations, int verbose) {
    struct timeval startTime;
    struct timeval endTime;

    long sampleTime;
    long minBaselineTime = LONG_MAX;

    // warmups
    if (verbose) printf("\rWarming up ");
    for (int warmup=0; warmup < WARMUPS; warmup++) {
        if (verbose) printf("\rWarming up - %.0f%%", ((float)warmup/(float)WARMUPS)*100);

        gettimeofday(&startTime, NULL);
        for (int i=0; i < N/M; i++) {
            UNROLL_10000_TIMES(BASELINE);
        }
        gettimeofday(&endTime, NULL);
        
        sampleTime = ((endTime.tv_sec * 1e6 + endTime.tv_usec) - (startTime.tv_sec * 1e6 + startTime.tv_usec));
        if (verbose) fflush(stdout);
    }
    if (verbose) printf("\rWarming up - Done. \n");


    // performance measures for NOP instruction
    if (verbose) printf("\rPerforming time measurements for baseline instruction NOP ");
    for (int it=1; it <= iterations; it++) {
        if (verbose) printf("\rPerforming time measurements for baseline instruction NOP - %.0f%%", ((float)it/(float)iterations)*100);

        gettimeofday(&startTime, NULL);
        for (int i=0; i < N/M; i++) {
            UNROLL_10000_TIMES(BASELINE);
        }
        gettimeofday(&endTime, NULL);
    
        sampleTime = ((endTime.tv_sec * 1e6 + endTime.tv_usec) - (startTime.tv_sec * 1e6 + startTime.tv_usec));
        if (sampleTime < minBaselineTime) {
            minBaselineTime = sampleTime;
        }
        if (verbose) fflush(stdout);   
    }
    if (verbose) printf("\rPerforming time measurements for baseline instruction NOP - Done. \n");


    return ((float) minBaselineTime * 1e3) / N;           // nanoseconds
}


static float measeure_overhead(int iterations, int verbose) {
    struct timeval startTime;
    struct timeval endTime;
    struct timeval dummy;

    long sampleTime;
    long minSampleTime = LONG_MAX;

    // warmups
    if (verbose) printf("\rWarming up ");
    for (int warmup=0; warmup < WARMUPS; warmup++) {
        if (verbose) printf("\rWarming up - %.0f%%", ((float)warmup/(float)WARMUPS)*100);

        gettimeofday(&startTime, NULL);
        for (int i=0; i < N/M; i++) {
            UNROLL_10000_TIMES(gettimeofday(&dummy, NULL););
        }
        gettimeofday(&endTime, NULL);
        
        sampleTime = ((endTime.tv_sec * 1e6 + endTime.tv_usec) - (startTime.tv_sec * 1e6 + startTime.tv_usec));
        if (verbose) fflush(stdout);
    }
    if (verbose) printf("\rWarming up - Done. \n");


    // performance measures for gettimeofday() instruction
    if (verbose) printf("\rPerforming time measurements for sampleTime of gettimeofday() ");
    for (int it=1; it <= iterations; it++) {
        if (verbose) printf("\rPerforming time measurements for sampleTime of gettimeofday() - %.0f%%", ((float)it/(float)iterations)*100);

        gettimeofday(&startTime, NULL);
        for (int i=0; i < N/M; i++) {
            UNROLL_10000_TIMES(gettimeofday(&dummy, NULL););
        }
        gettimeofday(&endTime, NULL);
    
        sampleTime = ((endTime.tv_sec * 1e6 + endTime.tv_usec) - (startTime.tv_sec * 1e6 + startTime.tv_usec));
         if (sampleTime < minSampleTime) {
            minSampleTime = sampleTime;
        }
        if (verbose) fflush(stdout);   
    }
    if (verbose) printf("\rPerforming time measurements for sampleTime of gettimeofday() - Done. \n");


    return ((float) minSampleTime * 1e3) / N;           // nanoseconds
}


static float measure_asm_intruction(int iterations, int verbose) {
    struct timeval startTime;
    struct timeval endTime;
    struct timeval dummy;

    long sampleTime;
    long minInstructionTime = LONG_MAX;
    int regs[4];

    // warmups
    if (verbose) printf("\rWarming up ");
    for (int warmup=0; warmup < WARMUPS; warmup++) {
        if (verbose) printf("\rWarming up - %.0f%%", ((float)warmup/(float)WARMUPS)*100);

        gettimeofday(&startTime, NULL);
        for (int i=0; i < N/M; i++) {
            UNROLL_10000_TIMES(CPUID);
        }
        gettimeofday(&endTime, NULL);
        
        sampleTime = ((endTime.tv_sec * 1e6 + endTime.tv_usec) - (startTime.tv_sec * 1e6 + startTime.tv_usec));
        if (verbose) fflush(stdout);
    }
    if (verbose) printf("\rWarming up - Done. \n");


    // performance measures for CPUID instruction
    if (verbose) printf("\rPerforming time measurements for asm intruction CPUID ");
    for (int it=1; it <= iterations; it++) {
        if (verbose) printf("\rPerforming time measurements for asm intruction CPUID - %.0f%%", ((float)it/(float)iterations)*100);

        gettimeofday(&startTime, NULL);
        for (int i=0; i < N/M; i++) {
            UNROLL_10000_TIMES(CPUID);
        }
        gettimeofday(&endTime, NULL);
    
        sampleTime = ((endTime.tv_sec * 1e6 + endTime.tv_usec) - (startTime.tv_sec * 1e6 + startTime.tv_usec));
         if (sampleTime < minInstructionTime) {
            minInstructionTime = sampleTime;
        }
        if (verbose) fflush(stdout);   
    }
    if (verbose) printf("\rPerforming time measurements for asm intruction CPUID - Done. \n");


    return ((float) minInstructionTime * 1e3) / N;           // nanoseconds
}



static float measure_syscall(int iterations, int verbose) {
    struct timeval startTime;
    struct timeval endTime;
    struct timeval dummy;

    long sampleTime;
    long minInstructionTime = LONG_MAX;
    int regs[4];

    // warmups
    if (verbose) printf("\rWarming up ");
    for (int warmup=0; warmup < WARMUPS; warmup++) {
        if (verbose) printf("\rWarming up - %.0f%%", ((float)warmup/(float)WARMUPS)*100);

        gettimeofday(&startTime, NULL);
        for (int i=0; i < N/M; i++) {
            UNROLL_10000_TIMES(GETPID);
        }
        gettimeofday(&endTime, NULL);
        
        sampleTime = ((endTime.tv_sec * 1e6 + endTime.tv_usec) - (startTime.tv_sec * 1e6 + startTime.tv_usec));
        if (verbose) fflush(stdout);
    }
    if (verbose) printf("\rWarming up - Done. \n");


    // performance measures for GETPID instruction
    if (verbose) printf("\rPerforming time measurements for syscall GETPID ");
    for (int it=1; it <= iterations; it++) {
        if (verbose) printf("\rPerforming time measurements for syscall GETPID - %.0f%%", ((float)it/(float)iterations)*100);

        gettimeofday(&startTime, NULL);
        for (int i=0; i < N/M; i++) {
            UNROLL_10000_TIMES(GETPID);
        }
        gettimeofday(&endTime, NULL);
    
        sampleTime = ((endTime.tv_sec * 1e6 + endTime.tv_usec) - (startTime.tv_sec * 1e6 + startTime.tv_usec));
         if (sampleTime < minInstructionTime) {
            minInstructionTime = sampleTime;
        }
        if (verbose) fflush(stdout);   
    }
    if (verbose) printf("\rPerforming time measurements for syscall GETPID - Done. \n");


    return ((float) minInstructionTime * 1e3) / N;           // nanoseconds}
}



int detect_hypervisor_asm(float *ratio, int iterations, int verbose) {

    float minSamplePerInstruction = measeure_overhead(iterations, verbose);
    float minInstructionPerInstruction = measure_asm_intruction(iterations, verbose);
    float minBaseLinePerInstruction = measure_baseline(iterations, verbose);

    float realMinInstructionPerIntruction = minInstructionPerInstruction - minSamplePerInstruction;
    
    VERBOSE(printf("\n\n------ Baseline time ---------\n"));
    VERBOSE(printf("Time taken by N instuctions: %.0f microseconds (%f seconds)\n", minBaseLinePerInstruction*N*1e-3, minBaseLinePerInstruction*N*1e-9));
    VERBOSE(printf("Time taken per intruction: %.2f nanoseconds\n ", minBaseLinePerInstruction));

    VERBOSE(printf("\n\n------ Instruction time -----------\n"));
    VERBOSE(printf("Time taken by N instuctions: %.0f microseconds (%f seconds)\n", minInstructionPerInstruction*N*1e-3, minInstructionPerInstruction*N*1e-9));
    VERBOSE(printf("Time taken per intruction: %.2f nanoseconds\n ", minInstructionPerInstruction));

    *ratio = (float) realMinInstructionPerIntruction / minBaseLinePerInstruction;
    
    VERBOSE(printf("\n\n------ Relative time ---------\n"));
    VERBOSE(printf("Instruction time taken with sample time (%.2f nanoseconds) deducted: %.2f nanosecs\n", minSamplePerInstruction, realMinInstructionPerIntruction));
    VERBOSE(printf("Baseline time taken with sample time (0 nanoseconds) deducted: %.2f nanosecs\n", minBaseLinePerInstruction));
    VERBOSE(printf("Ratio r (cpuid:nop): %f\n\n", *ratio));

    return *ratio > THRESHOLD_ASM;
}



int detect_hypervisor_asm_customThreshold(float *ratio, int iterations, int verbose, int threshold) {
    int retVal = detect_hypervisor_asm(ratio, iterations, verbose);
    return *ratio > threshold;
}




int detect_hypervisor_syscall(float *ratio, int iterations, int verbose) {

    float minSamplePerInstruction = measeure_overhead(iterations, verbose);
    float minInstructionPerInstruction = measure_syscall(iterations, verbose);
    float minBaseLinePerInstruction = measure_baseline(iterations, verbose);

    float realMinInstructionPerIntruction = minInstructionPerInstruction - minSamplePerInstruction;
    
    VERBOSE(printf("\n\n------ Baseline time ---------\n"));
    VERBOSE(printf("Time taken by N instuctions: %.0f microseconds (%f seconds)\n", minBaseLinePerInstruction*N*1e-3, minBaseLinePerInstruction*N*1e-9));
    VERBOSE(printf("Time taken per intruction: %.2f nanoseconds\n ", minBaseLinePerInstruction));

    VERBOSE(printf("\n\n------ Instruction time -----------\n"));
    VERBOSE(printf("Time taken by N instuctions: %.0f microseconds (%f seconds)\n", minInstructionPerInstruction*N*1e-3, minInstructionPerInstruction*N*1e-9));
    VERBOSE(printf("Time taken per intruction: %.2f nanoseconds\n ", minInstructionPerInstruction));

    *ratio = (float) realMinInstructionPerIntruction / minBaseLinePerInstruction;
    
    VERBOSE(printf("\n\n------ Relative time ---------\n"));
    VERBOSE(printf("Instruction time taken with sample time (%.2f nanoseconds) deducted: %.2f nanosecs\n", minSamplePerInstruction, realMinInstructionPerIntruction));
    VERBOSE(printf("Baseline time taken with sample time (0 nanoseconds) deducted: %.2f nanosecs\n", minBaseLinePerInstruction));
    VERBOSE(printf("Ratio r (cpuid:nop): %f\n\n", *ratio));

    return *ratio > THRESHOLD_SYSCALL;
}



int detect_hypervisor_syscall_customThreshold(float *ratio, int iterations, int verbose, int threshold) {
    int retVal = detect_hypervisor_syscall(ratio, iterations, verbose);
    return *ratio > threshold;
}

