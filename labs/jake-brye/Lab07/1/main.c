/*
 * main.c
 */
#include <stdbool.h>
#include "open_interface.h"
#include "timer.h"
#include "lcd.h"
#include "button.h"
#include <math.h>
#include "driverlib/interrupt.h"

volatile enum {LOW, HIGH, DONE} state;
volatile unsigned int initial_value = 0;
volatile unsigned int second_value = 0;
volatile unsigned int final_value = 0;
volatile float distance = 0;

void uart_init(void) {     //enable clock to GPIO, R1 = port B
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

		//enable clock to UART1, R1 = UART1. ***Must be done before setting Rx and Tx
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;
		//enable alternate functions on port b pins 0 and 1
	GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1); // check this!!
		//enable Rx and Tx on port B on pins 0 and 1
	GPIO_PORTB_PCTL_R |= 0x00000011;
		//set pin 0 and 1 to digital
	GPIO_PORTB_DEN_R |= (BIT0 | BIT1);
		//set pin 0 to Rx or input
	GPIO_PORTB_DIR_R &= ~BIT0;
		//set pin 1 to Tx or output
	GPIO_PORTB_DIR_R |= BIT1;
	//calculate baudrate
	uint16_t iBRD = 8;//use equations
	uint16_t fBRD =  44;//use equations
	//turn off uart1 while we set it up
	UART1_CTL_R &= ~(UART_CTL_UARTEN);
	//set baud rate
	UART1_IBRD_R = iBRD;
	UART1_FBRD_R = fBRD;
	//set frame, 8 data bits, 1 stop bit, no parity, no FIFO
	UART1_LCRH_R = UART_LCRH_WLEN_8 ;
	//use system clock as source
	UART1_CC_R = UART_CC_CS_SYSCLK;
	//re-enable enable RX, TX, and uart1
	UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);

}

void TIMER3B_Handler(void){
int overflow = 0, value=0;
//	if(initial_value == 0){
//		initial_value = TIMER3_TBR_R;
//		state = HIGH;
//	} else {
//		final_value = initial_value - TIMER3_TBR_R;
//		initial_value = 0;
////		lcd_clear();
//		lcd_printf("final_value: %hu\n", final_value);
//		state = DONE;
//		//disable interrupts
//		//enable port 3 for normal gpio functions?
//	}
//	TIMER3_ICR_R |= 0x400;
	TIMER3_ICR_R |= TIMER_IMR_CBEIM;

	if(state == LOW){
		initial_value = TIMER3_TBR_R;
		state = HIGH;
	} else {
		second_value = TIMER3_TBR_R;
		if(second_value < initial_value)
		{
			overflow = 1;
		}
		value = overflow*16000000;
		final_value = second_value + value - initial_value;
		initial_value = 0;
//		lcd_clear();
//		lcd_printf("final_value: %hu\n", final_value);

		distance = ((0.002125 * final_value)/2);
//		final_value = final_value * 0.0000625;
		lcd_printf("final_value: %.2f\ndistance: %.2f\noverflow: %d", (final_value * 0.0000625), distance, overflow);
		final_value = 0;
		distance = 0;
		state = DONE;
		//disable interrupts
		//enable port 3 for normal gpio functions?
	}



//	TIMER3_ICR_R |= 0x00000404;
//	TIMER3_ICR_R |= TIMER_ICR_CBECINT;

}

/// Initialize the timers
void clock_timer_init(void) {
//	IntMasterDisable();
//	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3; // Turn on clock to TIMER3
//
//	//Configure the timer for input capture mode
//	TIMER3_CTL_R &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN); //disable timerB to allow us to change settings
//
//	TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;
//	TIMER3_CFG_R |= TIMER_CFG_16_BIT; //set to 16 bit timer
//
//	TIMER3_TBMR_R = TIMER_TBMR_TBMR_CAP; //set for capture mode, down count
//
//
////	TIMER3_TBILR_R = (int)(16000000/(50 * 5));
//	TIMER3_TBILR_R = 0xFFFF;
//
//	TIMER3_IMR_R |= (TIMER_IMR_CBEIM);
////	TIMER3_MIS_R
//
//	//clear TIMER3B interrupt flags
//	TIMER3_ICR_R |= (TIMER_ICR_TBTOCINT); //clears TIMER3 time-out interrupt flags
//
//
//	//initialize local interrupts
//	//NVIC_EN2_R = 0x000000C0; //#warning "enable interrupts for TIMER4A and TIMER4B" n = 0, 1, 2, 3, or 4
//	//go to page 105 and find the corresponding interrupt numbers for TIMER4 A&B
//	//then set those bits in the correct interrupt set EN register (p. 142)
//	NVIC_EN1_R = 0x00000010;		//36-32=4
//	TIMER3_CTL_R |= (TIMER_CTL_TAEN | TIMER_CTL_TBEN); //Enable TIMER3B
//
//
//
//	IntMasterEnable(); //intialize global interrupts
//
////	TIMER3_CTL_R |=   (TIMER_CTL_TBEN); //Enable TIMER3B

	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3; // Turn on clock to TIMER3

		//Configure the timer for input capture mode
		TIMER3_CTL_R &= ~(TIMER_CTL_TBEN); //disable timerB to allow us to change settings
		TIMER3_CFG_R |= TIMER_CFG_16_BIT; //set to 16 bit timer

		TIMER3_TBMR_R |= (TIMER_TBMR_TBCMR | TIMER_TBMR_TBCDIR | TIMER_TBMR_TBMR_CAP); //set for capture mode, up count

		TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;

	//	TIMER3_TBILR_R = (int)(16000000/(50 * 5));
	//	TIMER3_TBILR_R = 0xFFFF;

		TIMER3_ICR_R |= (TIMER_ICR_CBECINT); //clears TIMER3 time-out interrupt flags
		TIMER3_IMR_R |= (TIMER_IMR_CBEIM);	//sets the interrupt mask



		//initialize local interrupts
		//NVIC_EN2_R = 0x000000C0; //#warning "enable interrupts for TIMER4A and TIMER4B" n = 0, 1, 2, 3, or 4
		//go to page 105 and find the corresponding interrupt numbers for TIMER4 A&B
		//then set those bits in the correct interrupt set EN register (p. 142)
		NVIC_EN1_R |= 0x00000010;		//36-32=4
//		TIMER3_CTL_R |= (TIMER_CTL_TBEN); //Enable TIMER3B


		IntRegister(INT_TIMER3B, TIMER3B_Handler); //register TIMER4B interrupt handler
		IntMasterEnable(); //intialize global interrupts

		TIMER3_CTL_R |=   (TIMER_CTL_TBEN); //Enable TIMER3B
}



// Send pulse to port B
void send_pulse()
{

	GPIO_PORTB_PCTL_R &= ~GPIO_PCTL_PB3_T3CCP1;

	GPIO_PORTB_AFSEL_R &= 0xF7;


	//GPIO_PORTB_PCTL_R &= 0xF7;
	GPIO_PORTB_DIR_R |= 0x08; // set PB3 as output
	GPIO_PORTB_DATA_R |= 0x08; // set PB3 to high
	timer_waitMicros(10);// wait at least 5 microseconds based on data sheet
//	GPIO_PORTB_DEN_R &= 0xF7;
	GPIO_PORTB_DATA_R &= 0xF7; // set PB3 to low
	GPIO_PORTB_DIR_R &= 0xF7; // set PB3 as input
	GPIO_PORTB_AFSEL_R |= 0x08;

	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;


//	GPIO_PORTB_DEN_R |= 0x08;
//	GPIO_PORTB_PCTL_R |= 0x00007000;  //activiating Port mux control 3
//	GPIO_PORTB_DEN_R |= 0x08;
//	NVIC_EN1_R = 0x0001000;
}


unsigned time2distance(){
	TIMER3_CTL_R |= 0x0000D000;







	return 0;
}


void GPIO_init()
{
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	GPIO_PORTB_DEN_R |= 0x08;

	GPIO_PORTB_AFSEL_R &= 0x00;
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;
	GPIO_PORTB_PCTL_R &= ~GPIO_PCTL_PB3_T3CCP1;
}

int main(void){


lcd_init();
GPIO_init();
//SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
//IntRegister(INT_TIMER3B, TIMER3B_Handler); //register TIMER4B interrupt handler
// clock_timer_init();
state = DONE;
clock_timer_init();
while(1)
{

// if(state == DONE)
// {
	state = LOW;
	send_pulse();

	 timer_waitMillis(500);
// }

}
	
	return 0;
}





/*
 * init(){
 * GPIO's
 * TIMER3
 * ISR
 * }
 *
 * Ping_pulse(){
 *
 *
 *
 *
 *
 * }
 */



















