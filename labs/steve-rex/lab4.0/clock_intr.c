/**
 * clock-intr.c: timer- and interrupt-based clock program
 * This is skeleton code. You need to make it complete.
 *
 * @author Zach Glanz, Zhao Zhang
 * @date 08/07/2016
 */

#include <stdint.h>
#include "timer.h"
#include "lcd.h"
#include "button.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

//#warning "TIMER4A_PRESCALER may not be defined correctly" // delete this line after defining TIMER4A_PRESCALER
//#warning "TIMER4B_PRESCALER may not be defined correctly" // delete this line after defining TIMER4B_PRESCALER
#define TIMER4A_PRESCALER 250 //  - Edit this so a 16-bit timer will not overflow in 1 second
#define TIMER4B_PRESCALER 50 //  - Edit this so a 16-bit timer will not overflow in 200 ms

/**
 * Timer interrupt source 1: the function will be called every one second
 * (you need define TIMER4A_PRESCALER correctly and enable the interrupts for TIMER4A
 */

int hr = 1;
int mn = 0;
int sc = 0;
uint8_t sw;

void TIMER4A_Handler(void)
{
	TIMER4_ICR_R = TIMER_ICR_TATOCINT; //clear the TIMER4A timeout interrupt flag (page 754 of datasheet)\

	sc++;
	if(sc > 59){
		mn++;
		sc = 0;
	}
	if(mn > 59){
		hr++;
		mn = 0;
	}
	if(hr > 11){
		hr = 1;
	}
	//increment clock
}

/**
 * Timer interrupt source 2: the function will be called every 200 milliseconds
 * (you need define TIMER4B_PRESCALER correctly and enable the interrupts for TIMER4B) 
 */
void TIMER4B_Handler(void)
{
	TIMER4_ICR_R = TIMER_ICR_TBTOCINT; //clear flag
	switch(sw){
	case 0:
		break;
	case 1:
		sc--;
		if(sc<0){
			sc = 59;
		}
		break;
	case 2:
		sc++;
		if(sc>59){
			sc = 0;
		}
		break;
	case 3:
		mn--;
		if(mn<0){
			mn = 59;
		}
		break;
	case 4:
		mn++;
		if(mn>59){
			mn = 0;
		}
		break;
	case 5:
		hr--;
		if(hr<0){
			hr = 12;
		}
		break;
	case 6:
		hr++;
		if(hr>12){
			hr = 0;
		}
		break;
	}
}

/// Initialize the timers
void clock_timer_init(void) {
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R4; // Turn on clock to TIMER4

	//Configure the timer for input capture mode
	TIMER4_CTL_R &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN); //disable timerB to allow us to change settings
	TIMER4_CFG_R |= TIMER_CFG_16_BIT; //set to 16 bit timer

	TIMER4_TAMR_R = TIMER_TAMR_TAMR_PERIOD; //set for periodic mode, down count
	TIMER4_TBMR_R = TIMER_TBMR_TBMR_PERIOD; //set for periodic mode, down count

	TIMER4_TAPR_R = TIMER4A_PRESCALER - 1; // 1 sec clock
	TIMER4_TBPR_R = TIMER4B_PRESCALER - 1;  // 200 ms clock

	TIMER4_TAILR_R = (int)(16000000/TIMER4A_PRESCALER); // set the load value for the 1 second clock (with your prescaler, how many clock cycles happen in 1 second?)
	TIMER4_TBILR_R = (int)(16000000/(TIMER4B_PRESCALER * 5)); // set the load value for the 0.2 second clock

	//clear TIMER3B interrupt flags
	TIMER4_ICR_R = (TIMER_ICR_TATOCINT | TIMER_ICR_TBTOCINT); //clears TIMER4 time-out interrupt flags
	TIMER4_IMR_R |= (TIMER_IMR_TATOIM | TIMER_IMR_TBTOIM); //enable TIMER4(A&B) time-out interrupts

	//initialize local interrupts
	NVIC_EN2_R = 0x000000C0; //#warning "enable interrupts for TIMER4A and TIMER4B" n = 0, 1, 2, 3, or 4
	//go to page 105 and find the corresponding interrupt numbers for TIMER4 A&B
	//then set those bits in the correct interrupt set EN register (p. 142)
	
	IntRegister(INT_TIMER4A, TIMER4A_Handler); //register TIMER4A interrupt handler
	IntRegister(INT_TIMER4B, TIMER4B_Handler); //register TIMER4B interrupt handler

	IntMasterEnable(); //intialize global interrupts

	TIMER4_CTL_R |= (TIMER_CTL_TAEN | TIMER_CTL_TBEN); //Enable TIMER4A & TIMER4B
}

/// Displays the time of day on the LCD
/**
 * Displays the time of day on the LCD in the format "HH:MM:SS".  Allows the user to adjust the time using
 * the push buttons.  The push buttons will be checked every 200ms, and will update the screen every time
 * the time is adjusted.
 *
 * SW6 increment hours
 * SW5 decrement hours
 * SW4 increment minutes
 * SW3 decrement minutes
 * SW2 increment seconds
 * SW1 decrement seconds
 */
int main(void){
	lcd_init();
	button_init();
	clock_timer_init();



    while(1) { // loop indefinitely
    	lcd_printf("%02d:%02d:%02d", hr, mn, sc);
        sw = button_getButton();

        //lcd_printf("Button: %d", button);
    }
}
