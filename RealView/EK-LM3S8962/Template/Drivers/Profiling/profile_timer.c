typedef unsigned char tBoolean;
#include "timer.h"
#include "hw_memmap.h"
#include "sysctl.h"

void microsecond_init() {
	// clock the peripheral and set to stall on debug stop
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerControlStall(TIMER0_BASE, TIMER_A, 1);

	// configure as periodic 32-bit timer
	TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);

	// enable timer 0
	TimerEnable(TIMER0_BASE, TIMER_A);
} 

void microsecond_fini() {
	TimerDisable(TIMER0_BASE, TIMER_A); 	
}
