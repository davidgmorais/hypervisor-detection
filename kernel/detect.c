#include <linux/module.h>    // Required by all kernel modules
#include <linux/kernel.h>    // Required by printk
#include <linux/init.h>      // Required for __init and __exit macros
#include <linux/proc_fs.h>   // Required for accessing the /proc file system
#include <linux/uaccess.h>   // Required for accessing the copy_to_user function
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/types.h>

#define PROC_FILENAME "detect"
#define C 100
#define N 30000
#define M 10000
#define WARMUPS 10

#define UNROLL_10_TIMES(x) x x x x x x x x x x
#define UNROLL_100_TIMES(x) UNROLL_10_TIMES(UNROLL_10_TIMES(x))
#define UNROLL_10000_TIMES(x) UNROLL_100_TIMES(UNROLL_100_TIMES(x))

#define OUT asm volatile ("out %0, %1" : : "a"(val), "Nd"(port));

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module to detect Hypervisors");

static struct proc_dir_entry * pf = 0;

static ssize_t
procfile_read( struct file * file, char __user * ubuf, size_t count, loff_t * ppos )
{
    struct timespec startTime;
    struct timespec endTime;
    struct timespec dummy;

    uint16_t val = 0x0;
    uint16_t port = 0x50;

 
    long long minTime = __LONG_LONG_MAX__;
    long long overheadPerInstruction;
    long long baselinePerInstruction;
    long long execTimePerInstruction;
    long long realExecPerInstruction;
    long long sampleTime;
    long long ratio;

    char *tmp; //time[10]; // hour(2) : min(2) : sec(2) \n \0 -> 10 bytes
    int i, j;


    if (*ppos != 0) return 0; // There is still text to consume from a previous call
    tmp=kmalloc(2048*sizeof(char),GFP_KERNEL);


    // measure overhead
    for (i=0; i<WARMUPS; i++) {
        getnstimeofday(&startTime);
        for (j=0; j < N/M; j++) {
            UNROLL_10000_TIMES(getnstimeofday(&dummy););
        }
        getnstimeofday(&endTime);
    }
    for (i=0; i<C; i++) {
        getnstimeofday(&startTime);
        for (j=0; j < N/M; j++) {
            UNROLL_10000_TIMES(getnstimeofday(&dummy););
        }
        getnstimeofday(&endTime);

        // time taken by N instuctions, in nanoseconds
        sampleTime = ((endTime.tv_sec * (long long) 1e9 + endTime.tv_nsec) - (startTime.tv_sec * (long long) 1e9 + startTime.tv_nsec)); 
        if (sampleTime < minTime) {
            minTime = sampleTime;
        }

    }

    overheadPerInstruction = 1000 * minTime / N; // time taken per instruction, in picoseconds
   

    // measure instuction
    minTime = __LONG_LONG_MAX__;
    for (i=0; i<WARMUPS; i++) {
        getnstimeofday(&startTime);
        for (j=0; j < N/M; j++) {
            UNROLL_10000_TIMES(OUT);
        }
        getnstimeofday(&endTime);
    }
    for (i=0; i<C; i++) {
        getnstimeofday(&startTime);
        for (j=0; j < N/M; j++) {
            UNROLL_10000_TIMES(OUT);
        }
        getnstimeofday(&endTime);

        // time taken by N instuctions, in nanoseconds
        sampleTime = ((endTime.tv_sec * (long long) 1e9 + endTime.tv_nsec) - (startTime.tv_sec * (long long) 1e9 + startTime.tv_nsec)); 
        if (sampleTime < minTime) {
            minTime = sampleTime;
        }
    }

    execTimePerInstruction = 1000 * minTime / N; // time taken per instruction, in picoseconds

    // measure baseiline
    minTime = __LONG_LONG_MAX__;
    for (i=0; i<WARMUPS; i++) {
        getnstimeofday(&startTime);
        for (j=0; j < N/M; j++) {
            UNROLL_10000_TIMES(asm volatile("nop;"););
        }
        getnstimeofday(&endTime);
    }
    for (i=0; i<C; i++) {
        getnstimeofday(&startTime);
        for (j=0; j < N/M; j++) {
            UNROLL_10000_TIMES(asm volatile("nop;"););
        }
        getnstimeofday(&endTime);

        // time taken by N instuctions, in nanoseconds
        sampleTime = ((endTime.tv_sec * (long long) 1e9 + endTime.tv_nsec) - (startTime.tv_sec * (long long) 1e9 + startTime.tv_nsec)); 
        if (sampleTime < minTime) {
            minTime = sampleTime;
        }
    }

    baselinePerInstruction = 1000 * minTime / N; // time taken per instruction, in picoseconds
   
    realExecPerInstruction = execTimePerInstruction - overheadPerInstruction;
    ratio = realExecPerInstruction / baselinePerInstruction;
    
    if (ratio > 110000) {
        sprintf(tmp, "Virtual host detected.\n");
    } else {
        sprintf(tmp, "Native host detected.\n");
    }

    if (copy_to_user( ubuf, tmp, strlen(tmp) )) // Returns the numbre of bytes that could not be coppyed
        return -EFAULT;

    *ppos = strlen( tmp );

    return *ppos;
}

static struct file_operations options = { // For older kernels
.owner = THIS_MODULE,
.read = procfile_read,
.write = 0
// static struct proc_ops options = { // For more recent kernels
//     .proc_read = procfile_read,
//     .proc_write = 0
};

static int
__init detect_init(void)
{
    pf = proc_create( PROC_FILENAME, 0444, 0, &options );

    if (pf == 0) {
        printk( KERN_ALERT "Detect proc module cannot open /proc/%s!\n", PROC_FILENAME );
    }
    printk( KERN_INFO "Detect proc module installed, check file /proc/%s!\n", PROC_FILENAME );

    return 0;    // 0 - module loaded; otherwise, loading error
}

static void
__exit detect_exit(void)
{
    if (pf != 0) {
        proc_remove( pf );
    }
    printk( KERN_INFO "detect module removed.\n" );
}

module_init(detect_init);
module_exit(detect_exit);
