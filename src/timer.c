#include "timer.h"

//volatile int* TIMER_PTR = (int *)0xFF202000;
struct timer_t * const TIMER = (struct timer_t *) 0xFF202000;

void waitASec(){
    waitHalfASec();
    waitHalfASec();

}

void waitHalfASec(){
        int status;
        //*(TIMER_PTR +8) = 100000000; //half a sec times 2 for 1 sec
        //*(TIMER_PTR +12) = 1000000000 >> 16;

        TIMER->control = 0x8;
        TIMER->status = 0;
        TIMER->periodLo = (100000000 & 0x0000FFFF);
        TIMER->periodHi = (100000000 & 0xFFFF0000) >> 16;
        TIMER->control = 0x4;
        while((TIMER->status & 0x1) == 0);

        TIMER->status = 0;
}
