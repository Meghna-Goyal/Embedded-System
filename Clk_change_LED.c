/*The MCLK rate can be changed as shown in the attached program.
 By using a blinking LED demonstrate that the MCLK has changed after a button press.
 You should check that the MCLK value has really been changed in the Expressions window.*/

#include <driverlib.h>
#include <msp430.h>
#define MCLK_DESIRED_FREQUENCY_IN_KHZ 10000
#define REFOCLK_FREQUENCY 32768
#define MCLK_FLLREF_RATIO MCLK_DESIRED_FREQUENCY_IN_KHZ /(REFOCLK_FREQUENCY/1024 )

volatile unsigned short Button = 0;
uint32_t myACLK  = 0;
uint32_t myMCLK = 0 ;
void toggle_clock();

void main (void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer
    FRCTL0 = FRCTLPW | NWAITS_1;
    PMM_unlockLPM5();

    // Configuring the ports
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );  // LED_ PIN
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );
    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN2 ); // Switch

    while(1){
        while (GPIO_getInputPinValue( GPIO_PORT_P1, GPIO_PIN2 )) // for blinking LED
        {
            GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
            _delay_cycles (600000);
        }

        toggle_clock();
        while (GPIO_getInputPinValue( GPIO_PORT_P1, GPIO_PIN2 ))
         {
        GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
         _delay_cycles (600000);
         }
    }
}

//function for changing clock cycle rate
void toggle_clock(){

    myMCLK  = CS_getMCLK();
    myACLK  = CS_getACLK();

    // Set ACLK to use REFO as its oscillator source (32KHz)
    CS_initClockSignal(
            CS_ACLK,            // Clock you're configuring
            CS_REFOCLK_SELECT,  // Clock source
            CS_CLOCK_DIVIDER_1  // Divide down clock source by this much
    );
    // Set the FLL's clock reference clock source
    CS_initClockSignal(
            CS_FLLREF,         // Clock you're configuring
            CS_REFOCLK_SELECT, // Clock source
            CS_CLOCK_DIVIDER_1 // Divide down clock source by this much
    );
    // Configure the FLL's frequency and set MCLK & SMCLK to use the FLL as their source (8MHz)
    CS_initFLLSettle(
            MCLK_DESIRED_FREQUENCY_IN_KHZ,  // MCLK frequency
            MCLK_FLLREF_RATIO               // Ratio between MCLK and FLL's reference clock source
    );

    myMCLK  = CS_getMCLK();
    myACLK  = CS_getACLK();
}





