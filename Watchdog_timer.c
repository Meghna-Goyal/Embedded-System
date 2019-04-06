// demonstration of watchdog timer using the counter variable incremented values
//we dont need to ask microcontroller to wait for the operations if its frequency is less than 8MHz

#include "driverlib.h"
#include <stdio.h>
int main(void) {
    uint32_t myACLK=0;
    int count = 0 ;

    FRCTL0 = FRCTLPW | NWAITS_0; //disabling the waits for the speed less than 8 MHz as the processor is very slow to wait for the opertions to take place
    //WDT_A_hold(WDT_A_BASE); //disabling watch dog timer. so it will not reset.
    myACLK= CS_getACLK();

    WDT_A_initWatchdogTimer(WDT_A_BASE, WDT_A_CLOCKSOURCE_ACLK, WDT_A_CLOCKDIVIDER_8192); //change the watch dog timer clock to ACLK-- making it more slow
    WDT_A_start (WDT_A_BASE);

    while(1)
    {
        printf ("count %d \n", count); //speed depends on the master clock by default 1mhz
        count ++;
      //WDT_A_resetTimer(WDT_A_BASE); //reseting the clock of watchdog timer to zero --similar to disabling the interrupt
    }



    return (0);
}
