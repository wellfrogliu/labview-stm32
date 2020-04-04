/*** These settings can be modified ***/

/* This define sets the maximum number of RTX tasks. Increase this number to
   allow for more timed loops in the system. */
#define OS_TASKCNT              16
#define OS_PRIVCNT              OS_TASKCNT

/* TCP/IP stttings */
#define RLARM_UDP_CHECKSUMS     0           /* Use UDP checksums 1: Yes, 0: No */
#define TCP_TASK_STACK_BYTES    400         /* RTX TCP Main Task stack size */
#define TCP_TIMER_STACK_BYTES   80          /* RTX TCP Timer Task stack size */


/*** DO NOT EDIT BELOW HERE ***/

// #include "LVAutoConfig.h"
