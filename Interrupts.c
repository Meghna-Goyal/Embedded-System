/*green and red LED are off. Press button, both LED are ON using interrupt*/
#include "driverlib.h"

int main(void) {
    WDT_A_hold(WDT_A_BASE);
    FRCTL0 = FRCTLPW | NWAITS_0;
    PMM_unlockLPM5();

    // ports configuration
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN0);

    // interrupt configuration
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN2);
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN2);

    __bis_SR_register(GIE);  // enable global interrupt
    while(1)
    {
__no_operation();
    }

    return (0);
}
    /*
     * PORT1 Interrupt Service Routine
     * Handles S1 button press interrupt
     */
    #pragma vector = PORT1_VECTOR  // assigns correct location in the vector table
    __interrupt void PORT1_ISR(void)  // interrupt keyword tells the compiler the save/ restore context  func does accepts or return any argument
    {
    //    P1OUT |= BIT0;    // Turn LED1 On
        switch(__even_in_range(P1IV, P1IV_P1IFG7))  // Iv register gives the value for highest priority INT for the port1 max is 0x10
        {
            case P1IV_NONE : break;  // 0x00  - none
            case P1IV_P1IFG0 : break; //0x02  - pin0
            case P1IV_P1IFG1 : break;  // 0x04 - pin1
            case P1IV_P1IFG2 : GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
                                            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN0);
            break;
            case P1IV_P1IFG3 : break;
            case P1IV_P1IFG4 : break;
            case P1IV_P1IFG5 : break;
            case P1IV_P1IFG6 : break;
            case P1IV_P1IFG7 : break;
        }
    }
