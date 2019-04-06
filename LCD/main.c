//It displays a scrolling message on to start with.
//When you press SW1, it displays a certain number. Everytime you press SW1, the number is incremented by 1.
//If the displayed number is even, the heart symbol is displayed.
//On pressing SW2, the number is reset to 0.

#include "LCD.h"
#include "driverlib.h"
 unsigned  int counter = 0;

int main(void) {

    WDT_A_hold(WDT_A_BASE);
    // Stop Watchdog timer
    //WDT_A_hold(__MSP430_BASEADDRESS_WDT_A__);

    // Initialize LCD
    LCD_init();

    // configure Interrupt
     GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN2);
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN2);

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2,GPIO_PIN6);
   GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
   GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN6);
   GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN6);

   PMM_unlockLPM5();

   __bis_SR_register(GIE);

   displayScrollText("THIS IS MY FIRST LED PROGRAM");

    while(1) {
        __no_operation();     // Placeholder for while loop (not required)
    }

    return 0;
}


#pragma vector = PORT1_VECTOR  // assigns correct location in the vector table
  __interrupt void PORT1_ISR(void)  // interrupt keyword tells the compiler the save/ restore context  func does accepts or return any argument
  {
      switch(__even_in_range(P1IV, P1IV_P1IFG7))  // Iv register gives the value for highest priority INT for the port1 max is 0x10
      {
          case P1IV_NONE : break;  // 0x00  - none
          case P1IV_P1IFG0 : break; //0x02  - pin0
          case P1IV_P1IFG1 : break;  // 0x04 - pin1
          case P1IV_P1IFG2 :

                      counter  =counter +1;
                      if (counter%2 == 0)
                      {
                   LCD_displayNumber (counter);
                   // Set the LCD's "Heart" symbol
                   // LCDMx location
                   LCD_E_setMemory( LCD_E_BASE,
                                                    LCD_E_MEMORY_BLINKINGMEMORY_12,
                                                        LCD_HEART  );
                      }
                      else
                      {
                          // Erases just the heart symbol
                          LCD_E_clearMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_HEART);

                          LCD_displayNumber (counter);
                      }

          break;
          case P1IV_P1IFG3 : break;
          case P1IV_P1IFG4 : break;
          case P1IV_P1IFG5 : break;
          case P1IV_P1IFG6 : break;
          case P1IV_P1IFG7 : break;
      }
  }


#pragma vector = PORT2_VECTOR  // assigns correct location in the vector table
  __interrupt void PORT2_ISR(void)  // interrupt keyword tells the compiler the save/ restore context  func does accepts or return any argument
  {
      switch(__even_in_range(P2IV, P2IV_P2IFG7))  // Iv register gives the value for highest priority INT for the port1 max is 0x10
      {
          case P1IV_NONE : break;  // 0x00  - none
          case P1IV_P1IFG0 : break; //0x02  - pin0
          case P1IV_P1IFG1 : break;  // 0x04 - pin1
          case P1IV_P1IFG2 : break;
          case P1IV_P1IFG3 : break;
          case P1IV_P1IFG4 : break;
          case P1IV_P1IFG5 : break;
          case P1IV_P1IFG6 :

              counter= 0;
              // Erases just the heart symbol
              LCD_E_clearMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_HEART);
              LCD_showChar('R', 1);
                  LCD_showChar('E', 2);
                  LCD_showChar('S', 3);
                  LCD_showChar('E', 4);
                  LCD_showChar('T', 5);
              break;
          case P1IV_P1IFG7 : break;
      }
  }





