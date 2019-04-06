//when the system overflows the LED starts blinking  -- Interrupt Watchdog Timer
//s function sets the watchdog timer as timer interval mode, which will assert an interrupt without causing a PUC.
//clockSelect   is the clock source that the watchdog timer will use. (All clocks)
//clockDivider  is the divider of the clock source, in turn setting the watchdog timer interval

#include "driverlib.h"
uint32_t myACLK=0;
int main(void) {


    //with ACLK as source at an interval of 250 ms.
    WDT_A_initIntervalTimer(WDT_A_BASE,
                            WDT_A_CLOCKSOURCE_ACLK,  //32 khz
                        //    WDT_A_CLOCKDIVIDER_8192);  // 2G, 128M, 8192K , 512K, 32K [Default],8192, 512,64
             WDT_A_CLOCKDIVIDER_512);
             myACLK= CS_getACLK();
    WDT_A_start(WDT_A_BASE);  //This function starts the watchdog timer functionality to start counting again.

    //Enable Watchdog Interupt
    SFR_clearInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
    SFR_enableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);

    //Set P1.0 to output direction
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    PMM_unlockLPM5();

    //Enter LPM3, enable interrupts
    __bis_SR_register(GIE);

    while (1) {
        __no_operation();
    }
    return (0);
}

#pragma vector=WDT_VECTOR
__interrupt void WDT_A_ISR(void) {
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
