/*----------------------------------------------------------------------------
 *      R T L  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    RTX_CONFIG.C
 *      Purpose: Configuration of RTX Kernel for Cortex-M
 *      Rev.:    V3.20
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2008 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include "LVConfig.h"

/*----------------------------------------------------------------------------
 *      RTX User configuration part BEGIN
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//
// <h>Task Definitions
// ===================
//
//   <o>Number of concurrent running tasks <0-250>
//   <i> Define max. number of tasks that will run at the same time.
//   <i> Default: 6
#ifndef OS_TASKCNT
 #define OS_TASKCNT     16
#endif

//   <o>Number of tasks with user-provided stack <0-250>
//   <i> Define the number of tasks that will use a bigger stack.
//   <i> The memory space for the stack is provided by the user.
//   <i> Default: 0
#ifndef OS_PRIVCNT
 #define OS_PRIVCNT     16
#endif

//   <o>Task stack size [bytes] <20-4096:8><#/4>
//   <i> Set the stack size for tasks which is assigned by the system.
//   <i> Default: 200
#ifndef OS_STKSIZE
 #define OS_STKSIZE     64
#endif

// <q>Check for the stack overflow
// ===============================
// <i> Include the stack checking code for a stack overflow.
// <i> Note that additional code reduces the Kernel performance.
#ifndef OS_STKCHECK
 #define OS_STKCHECK    1
#endif

// <q>Run in privileged mode
// =========================
// <i> Run all Tasks in privileged mode.
// <i> Default: Unprivileged
#ifndef OS_RUNPRIV
 #define OS_RUNPRIV     1
#endif

//   <o>Number of user timers <0-250>
//   <i> Define max. number of user timers that will run at the same time.
//   <i> Default: 0  (User timers disabled)
#ifndef OS_TIMERCNT
 #define OS_TIMERCNT    0
#endif

// </h>
// <h>SysTick Timer Configuration
// =============================
//   <o>Timer clock value [Hz] <1-1000000000>
//   <i> Set the timer clock value for selected timer.
//   <i> Default: 50000000  (50MHz)
#ifndef OS_CLOCK
 #define OS_CLOCK       50000000
#endif

//   <o>Timer tick value [us] <1-1000000>
//   <i> Set the timer tick value for selected timer.
//   <i> Default: 10000  (10ms)
#ifndef OS_TICK
 #define OS_TICK        1000
#endif

// </h>
// <e>Round-Robin Task switching
// =============================
// <i> Enable Round-Robin Task switching.
#ifndef OS_ROBIN
 #define OS_ROBIN       1
#endif

//   <o>Round-Robin Timeout [ticks] <1-1000>
//   <i> Define how long a task will execute before a task switch.
//   <i> Default: 5
#ifndef OS_ROBINTOUT
 #define OS_ROBINTOUT   1
#endif


/* RTA is configured in file RTX_Config.c */
#define __RTA_RTX_CONFIG

// Standard library system mutexes
// ===============================
//  Define max. number system mutexes that are used to protect 
//  the arm standard runtime library. For microlib they are not used.
#ifndef OS_MUTEXCNT
 #define OS_MUTEXCNT    50
#endif
// </e>

//------------- <<< end of configuration section >>> -----------------------

/*----------------------------------------------------------------------------
 *      RTX User configuration part END
 *---------------------------------------------------------------------------*/

#define MAGIC_WORD      0xE25A2EA5
#define OS_TRV          ((U32)(((double)OS_CLOCK*(double)OS_TICK)/1E6)-1)


/*----------------------------------------------------------------------------
 *      Global Functions
 *---------------------------------------------------------------------------*/

/*--------------------------- os_idle_demon ---------------------------------*/

__task void os_idle_demon (void) {
   /* The idle demon is a system task, running when no other task is ready */
   /* to run. The 'os_xxx' function calls are not allowed from this task.  */

   for (;;) {
   /* HERE: include optional user code to be executed when no task runs.*/
   }
}


/*--------------------------- os_tmr_call -----------------------------------*/

void os_tmr_call (U16 info) {
   /* This function is called when the user timer has expired.             */
   /* Parameter "info" is the parameter defined when the timer was created.*/
   /* HERE: include here optional user code to be executed on timeout.     */

} /* end of os_tmr_call */

/*--------------------------- os_stk_overflow -------------------------------*/

extern void RTX_OSStackOverflow (void);

void os_stk_overflow (OS_TID task_id_in) {
   /* This function is called when a stack overflow is detected.           */
   /*  'task_id'    holds a task id for this task                          */
   static volatile OS_TID task_id;
   task_id = task_id_in;

   /* Use a uVision 'RTX Kernel' debug dialog page 'Active Tasks' to      */
   /* check which task has got a stack overflow and needs a bigger stack. */

   /* HERE: define user macros, or include here optional code to be          */
   /* executed on stack overflow.                                            */
   RTX_OSStackOverflow ();
   for (;;);
}


/*----------------------------------------------------------------------------
 *      RTX Configuration Functions
 *---------------------------------------------------------------------------*/


#include <RTX_lib.c>

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

