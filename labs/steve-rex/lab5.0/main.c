#include "lcd.h"
#include "timer.h"
#include "button.h"
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"

void UART_Transmit_string(char *data,int len);

uint8_t sw = 0;
int temp = 0;
#define TIMER4A_PRESCALER 250 //  - Edit this so a 16-bit timer will not overflow in 1 second
#define TIMER4B_PRESCALER 50 //  - Edit this so a 16-bit timer will not overflow in 200 ms
/*
void TIMER4A_Handler(void){
	return;
}
*/
void TIMER4B_Handler(void)
{
	TIMER4_ICR_R = TIMER_ICR_TBTOCINT; //clear flag


	if(temp != sw ){
		switch(sw){
		case 0:
			break;
		case 1:
			UART_Transmit_string("YAAAAAAS",8);
			UART_Transmit(0x0D);
			UART_Transmit(0x0A);
			break;
		case 2:
			UART_Transmit_string("GAGA",4);
			UART_Transmit(0x0D);
			UART_Transmit(0x0A);
			break;
		case 3:
			UART_Transmit_string("YAAAAAAS!!",10);
			UART_Transmit(0x0D);
			UART_Transmit(0x0A);
			break;
		case 4:
			UART_Transmit_string("Blue, no green, Ahhhhh!!!",25);
			UART_Transmit(0x0D);
			UART_Transmit(0x0A);
			break;
		case 5:
			UART_Transmit_string("No",2);
			UART_Transmit(0x0D);
			UART_Transmit(0x0A);
			break;
		case 6:
			UART_Transmit_string("Yes",3);
			UART_Transmit(0x0D);
			UART_Transmit(0x0A);
			break;
			}
		}
	temp = sw;
	return;
}



void clock_timer_init(void) {
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R4; // Turn on clock to TIMER4

	//Configure the timer for input capture mode
	TIMER4_CTL_R &= ~(TIMER_CTL_TBEN); //disable timerB to allow us to change settings
	TIMER4_CFG_R |= TIMER_CFG_16_BIT; //set to 16 bit timer

	//TIMER4_TAMR_R = TIMER_TAMR_TAMR_PERIOD; //set for periodic mode, down count
	TIMER4_TBMR_R = TIMER_TBMR_TBMR_PERIOD; //set for periodic mode, down count

	//TIMER4_TAPR_R = TIMER4A_PRESCALER - 1; // 1 sec clock
	TIMER4_TBPR_R = TIMER4B_PRESCALER - 1;  // 200 ms clock

	//TIMER4_TAILR_R = (int)(16000000/TIMER4A_PRESCALER); // set the load value for the 1 second clock (with your prescaler, how many clock cycles happen in 1 second?)
	TIMER4_TBILR_R = (int)(16000000/(TIMER4B_PRESCALER * 5)); // set the load value for the 0.2 second clock

	//clear TIMER3B interrupt flags
	TIMER4_ICR_R = ( TIMER_ICR_TBTOCINT); //clears TIMER4 time-out interrupt flags
	TIMER4_IMR_R |= (TIMER_IMR_TBTOIM); //enable TIMER4(A&B) time-out interrupts

	//initialize local interrupts
	NVIC_EN2_R = 0x000000C0; //#warning "enable interrupts for TIMER4A and TIMER4B" n = 0, 1, 2, 3, or 4
	//go to page 105 and find the corresponding interrupt numbers for TIMER4 A&B
	//then set those bits in the correct interrupt set EN register (p. 142)

	//IntRegister(INT_TIMER4A, TIMER4A_Handler); //register TIMER4A interrupt handler
	IntRegister(INT_TIMER4B, TIMER4B_Handler); //register TIMER4B interrupt handler

	IntMasterEnable(); //intialize global interrupts

	TIMER4_CTL_R |= (TIMER_CTL_TBEN); //Enable TIMER4A & TIMER4B
}

void UART_init(void){

	 //enable clock to GPIO, R1 = port B
	 SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	 //enable clock to UART1, R1 = UART1. ***Must be done before setting Rx and Tx
	 SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;
	 //enable alternate functions on port b pins 0 and 1
	 GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1);
	 //enable Rx and Tx on port B on pins 0 and 1
	 GPIO_PORTB_PCTL_R |= 0x00000011;
	 //set pin 0 and 1 to digital
	 GPIO_PORTB_DEN_R |= (BIT0 | BIT1);
	 //set pin 0 to Rx or input
	 GPIO_PORTB_DIR_R &= ~BIT0;
	 //set pin 1 to Tx or output
	 GPIO_PORTB_DIR_R |= BIT1;

	  //turn off uart1 while we set it up
	  UART1_CTL_R &= ~(UART_CTL_UARTEN);
	  //set baud rate
	  UART1_IBRD_R =8;
	  UART1_FBRD_R = 44;
	  //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
	  UART1_LCRH_R = UART_LCRH_WLEN_8 ;
	  //use system clock as source
	  UART1_CC_R = UART_CC_CS_SYSCLK;
	  //re-enable enable RX, TX, and uart1
	  UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);

}

void UART_Transmit(char data){
	while((UART1_FR_R &0x20) != 0);
	UART1_DR_R = data;
}

void UART_Transmit_string(char *data,int len){
	int i;
	for(i = 0; i<len;i++){
		UART_Transmit(data[i]);
	}
}

char UART_Recieve(void){
	char data;

	while(UART1_FR_R & UART_FR_RXFE){
		sw = button_getButton();
	}
	data = (char)(UART1_DR_R & 0xFF);

	return data;
}


int main(void){
	lcd_init();
	UART_init();
	button_init();
	clock_timer_init();
	char signal[21];
	char display[21];
	int i;
	display[20] = 0;
	signal[20] = 0;
	lcd_clear();

	while(1){

		for (i = 0; i<20; i++){

			signal[i] = UART_Recieve();
			if(signal[i] == '\r'){
				//UART_Transmit(0x0D);
				//UART_Transmit(0x0A);
				signal[i] = 0;
				break;
			}
			lcd_clear();
			sprintf(display,"Buff: %d char: %c", i+1, signal[i]);
			lcd_puts(display);
			UART_Transmit(signal[i]);
		}
		UART_Transmit(0x0D);
		UART_Transmit(0x0A);

		lcd_clear();
		lcd_puts(signal);
	}
}
