#include "profile_timer.h"

void microsecond_init() {
	T1PR = 11;
	T1TCR = 1;
} 

void microsecond_fini() {
	T1TCR = 0;  	
}

