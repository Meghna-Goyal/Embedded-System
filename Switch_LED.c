//Write a program to implement the following: Both LEDs are off.
//When you press SW1, the red LED blinks.
//When you press SW1 again it stops.
//When you press SW1 again, it starts blinking again.

//Demonstrating the issues with polling and switch debouncing


#include "driverlib.h"
#include <msp430.h>

volatile unsigned short Button1 = 0;
volatile unsigned short count = 0;

int main(void) {

    WDT_A_hold(WDT_A_BASE);
    PM5CTL0 &= ~LOCKLPM5;

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);

    for(;;) {

        Button1 = GPIO_getInputPinValue( GPIO_PORT_P1, GPIO_PIN2 );

              if ( Button1 == GPIO_INPUT_PIN_LOW ) {
                  if (count) {
                      count = 0;
                  }
                  else {
                      count = 1;
                  }
              }
            if( count)
            {
                GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
            }

            else
           {
              GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
              _delay_cycles(200);
            }

   }

return (0);
}



