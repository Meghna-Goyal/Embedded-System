// using compare mode to implement PWM.
// CCR2 register of TImerA and TimerB is used to toggle LED
#include "driverlib.h"

void initTimers(void);

int main(void) {

    WDT_A_hold(WDT_A_BASE);

    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );  // Red LED (LED2)
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN0 );  // Green LED (LED2)
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN0 );

    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P8,
                    GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1,
                        GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    PMM_unlockLPM5();

    initTimers();

    //__bis_SR_register( GIE );         // Enable interrupts globally

   while(1) {
       __no_operation();             // Placeholder for while loop (not required)
    }

    return (0);
}

void initTimers(void)
{
    // Set up the interrupt using CCROIFG to toggle red LED1
    // Set up the interrupt using TA1IFG toggle green LED2
    Timer_A_initUpModeParam initUpParam = { 0 };
    initUpParam.clockSource =                 TIMER_A_CLOCKSOURCE_ACLK;       // Use ACLK (slower clock)
    initUpParam.clockSourceDivider =          TIMER_A_CLOCKSOURCE_DIVIDER_1;  // Input clock = ACLK / 1 = 32KHz
    initUpParam.timerPeriod =                 0xFFFE;                       // Half the time
    initUpParam.timerInterruptEnable_TAIE =   TIMER_A_TAIE_INTERRUPT_DISABLE;  // Enable TAR -> 0 interrupt
    initUpParam.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE; //Enable compare interrupt
    initUpParam.timerClear =                  TIMER_A_DO_CLEAR;               // Clear TAR & clock divider
    initUpParam.startTimer =                  false;                          // Don't start the timer, yet

    Timer_A_initUpMode( TIMER_A1_BASE, &initUpParam );

    Timer_A_initCompareModeParam initCcr2Param = { 0 };
    initCcr2Param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;               // Use CCR2 for compare
    initCcr2Param.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE; // Since directly driving LED, interrup not req'd
    initCcr2Param.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_RESET;        // Toggle provides a 1 sec period based on CCR0 and CCR2 values
    initCcr2Param.compareValue = 0x0400;                                      // Compare value: 4000 = 1/2 second

    Timer_A_initCompareMode( TIMER_A1_BASE, &initCcr2Param );

    Timer_A_clearTimerInterrupt( TIMER_A1_BASE );   // Clear/enable flags and start timer
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_0 + TIMER_A_CAPTURECOMPARE_REGISTER_2); //Clear CCROIFG, CCR1IFG

    Timer_A_startCounter(
            TIMER_A1_BASE,
            TIMER_A_UP_MODE
    );

    Timer_A_initUpModeParam initUpParam1 = { 0 };
    initUpParam1.clockSource =                 TIMER_A_CLOCKSOURCE_ACLK;      // Use ACLK (slower clock)
    initUpParam1.clockSourceDivider =          TIMER_A_CLOCKSOURCE_DIVIDER_1;  // Input clock = ACLK / 1 = 32KHz
    initUpParam1.timerPeriod =                 0xFFFE;                        // Half the time
    initUpParam1.timerInterruptEnable_TAIE =   TIMER_A_TAIE_INTERRUPT_ENABLE;  // Enable TAR -> 0 interrupt
    initUpParam1.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE; //Enable compare interrupt
    initUpParam1.timerClear =                  TIMER_A_DO_CLEAR;               // Clear TAR & clock divider
    initUpParam1.startTimer =                  false;                          // Don't start the timer, yet

    Timer_A_initUpMode( TIMER_A0_BASE, &initUpParam1 );

    Timer_A_initCompareModeParam initCcr1Param = { 0 };
    initCcr1Param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;               // Use CCR2 for compare
    initCcr1Param.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE; // Since directly driving LED, interrupt not req'd
    initCcr1Param.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_RESET;        // Toggle provides a 1 sec period based on CCR0 and CCR2 values
    initCcr1Param.compareValue = 0x6000;                                      // Compare value: 4000 = 1/2 second

    Timer_A_initCompareMode( TIMER_A0_BASE, &initCcr1Param );

    Timer_A_clearTimerInterrupt( TIMER_A0_BASE );   // Clear/enable flags and start timer
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_0 + TIMER_A_CAPTURECOMPARE_REGISTER_1); //Clear CCROIFG, CCR1IFG

    Timer_A_startCounter(
            TIMER_A0_BASE,
            TIMER_A_UP_MODE
    );
}
