#define N 30000
#define M 10000
#define WARMUPS 20
#define THRESHOLD_ASM 900
#define THRESHOLD_SYSCALL 10000

#define UNROLL_10_TIMES(x) x x x x x x x x x x
#define UNROLL_100_TIMES(x) UNROLL_10_TIMES(UNROLL_10_TIMES(x))
#define UNROLL_10000_TIMES(x) UNROLL_100_TIMES(UNROLL_100_TIMES(x))
#define VERBOSE(x) if (verbose) x;

#define GETPID getpid();
#define CPUID asm volatile("cpuid": "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3]) : "a" (0), "c" (0));
#define BASELINE asm volatile ("nop;");

float measure_baseline(int iterations, int verbose);

int detect_hypervisor_asm(float *ratio, int iterations, int verbose);
int detect_hypervisor_asm_customThreshold(float *ratio, int iterations, int verbose, int threshold);
int detect_hypervisor_syscall(float *ratio, int iterations, int verbose);
int detect_hypervisor_syscall_customThreshold(float *ratio, int iterations, int verbose, int threshold);