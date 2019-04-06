#include "driverlib.h"
#include "hal_LCD.h"

void displayTemp();
void tempSensor();


// See device datasheet for TLV table memory mapping
#define CALADC_15V_30C  *((unsigned int *)0x1A1A)       // Temperature Sensor Calibration-30 C
#define CALADC_15V_85C  *((unsigned int *)0x1A1C)       // Temperature Sensor Calibration-85 C

volatile unsigned int *temp_deg = &BAKMEM6;             // Celcius measurement

volatile unsigned int t;

int main(void) {

    WDT_A_hold(WDT_A_BASE);
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 ); //Turn LED1 on to indicate new temperature reading
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );

    PMM_unlockLPM5();

    Init_LCD();
    clearLCD();

    tempSensor();

    return (0);
}

void tempSensor(){
	// TimerA UpMode Configuration Parameter
	Timer_A_initUpModeParam initUpParam_A1 =
	{
	    TIMER_A_CLOCKSOURCE_ACLK,               // ACLK Clock Source
	    TIMER_A_CLOCKSOURCE_DIVIDER_1,          // ACLK/1 = 32768Hz
	    0x2000,                                 // Timer period
	    TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
	    TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,   // Disable CCR0 interrupt
	    TIMER_A_DO_CLEAR                        // Clear value
	};

	Timer_A_initCompareModeParam initCompParam =
	{
	    TIMER_A_CAPTURECOMPARE_REGISTER_1,        // Compare register 1
	    TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE, // Disable Compare interrupt
	    TIMER_A_OUTPUTMODE_RESET_SET,             // Timer output mode 7
	    0x1000                                    // Compare value
	};

	/*
	 * Base Address for the ADC Module
	 * Use Timer trigger 1 as sample/hold signal to start conversion
	 * USE MODOSC 5MHZ Digital Oscillator as clock source
	 * Use default clock divider of 1
	 */
	ADC_init(ADC_BASE,
			ADC_SAMPLEHOLDSOURCE_2,
			ADC_CLOCKSOURCE_ADCOSC,
			ADC_CLOCKDIVIDER_1);

	ADC_enable(ADC_BASE);

	//Configure Memory Buffer
	/*
	 * Base Address for the ADC Module
	 * Use input A12 Temp Sensor
	 * Use positive reference of Internally generated Vref
	 * Use negative reference of AVss
	 */
	ADC_configureMemory(ADC_BASE,
			ADC_INPUT_TEMPSENSOR,
			ADC_VREFPOS_INT,
			ADC_VREFNEG_AVSS);

	ADC_clearInterrupt(ADC_BASE,
			ADC_COMPLETED_INTERRUPT);

	// Enable the Memory Buffer Interrupt
	ADC_enableInterrupt(ADC_BASE,
			ADC_COMPLETED_INTERRUPT);

	ADC_startConversion(ADC_BASE,
			ADC_REPEATED_SINGLECHANNEL);

	// Enable internal reference and temperature sensor
	PMM_enableInternalReference();
	PMM_enableTempSensor();

	// TimerA1.1 (125ms ON-period) - ADC conversion trigger signal
	Timer_A_initUpMode(TIMER_A1_BASE, &initUpParam_A1);

	//Initialize compare mode to generate PWM1
	Timer_A_initCompareMode(TIMER_A1_BASE, &initCompParam);

	// Start timer A1 in up mode
	Timer_A_startCounter(TIMER_A1_BASE,
			TIMER_A_UP_MODE
	);

    // Delay for reference settling
	__delay_cycles(300000);

	while (1) {
		__bis_SR_register(LPM3_bits | GIE);                       // LPM3 with interrupts enabled
		__no_operation();                                         // Only for debugger

		// Turn LED1 on when waking up to calculate temperature and update display
		P1OUT |= BIT0;

		// Calculate Temperature in degree C
		signed short temp = (ADCMEM0 - CALADC_15V_30C);
		*temp_deg = ((long)temp * 10 * (85-30) * 10)/((CALADC_15V_85C-CALADC_15V_30C)*10) + 300;

		// Update temperature on LCD
		displayTemp();

		P1OUT &= ~BIT0;
	}
}

/*
 * ADC Interrupt Service Routine
 * Wake up from LPM3 when ADC conversion completes
 */
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:
            break;
        case ADCIV_ADCTOVIFG:
            break;
        case ADCIV_ADCHIIFG:
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:
            // Clear interrupt flag
           ADC_clearInterrupt(ADC_BASE, ADC_COMPLETED_INTERRUPT_FLAG);
           __bic_SR_register_on_exit(LPM3_bits);                // Exit LPM3
            break;
        default:
            break;
    }
}

void displayTemp()
{
	clearLCD();

    int temperature = *temp_deg;

	// Handle negative values
	if (temperature < 0)
	{
		temperature *= -1;
		// Negative sign
		LCDMEM[pos1+1] |= 0x04;
	}

	// Handles displaying up to 999.9 degrees
	if (temperature >= 1000)
		showChar((temperature/1000)%10 + '0', pos2);
	if (temperature >= 100)
		showChar((temperature/100)%10 + '0', pos3);
	if (temperature >= 10)
		showChar((temperature/10)%10 + '0', pos4);
	if (temperature >= 1)
		showChar((temperature/1)%10 + '0', pos5);

	// Decimal point
	LCDMEM[pos4+1] |= 0x01;

	// Degree symbol
	LCDMEM[pos5+1] |= 0x04;

	showChar('C', pos6);
}

