/*
 * main.c
 *
 *  Created on: Oct 11, 2016
 *      Author: borseth & STEVE :^)
 */

#include "Timer.h"
#include "lcd.h"
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"

volatile int current_state = 0;
volatile int rising_edge_time = 0;
volatile int falling_edge_time = 0;
volatile int overflow = 0;


void TIMER3B_Handler(void);

/*
int leventtime;
int pulsewidth;
float distance;
*/
//int overflow;

//#define T3_CTL = (*((volatile unsigned long *)0x4003300C))

//enum {IN,OUT};

void regestersinit(){


	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	//enable clock to GPIO, R1 = port B
	GPIO_PORTB_DEN_R |= 0x08;
	// set pin 3
	GPIO_PORTB_AFSEL_R &= 0x00;
	//set all functions to be controlled by GPIO registers
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;
	//turn on T3CCP1 for control

	GPIO_PORTB_PCTL_R &= ~GPIO_PCTL_PB3_T3CCP1;
	//turn off T3CCP1 for control

}

void timerinit(){

	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3; // Turn on clock to TIMER3

	TIMER3_CTL_R &= ~TIMER_CTL_TBEN;
	// set timer3 disabled to port B
	TIMER3_CFG_R |= TIMER_CFG_16_BIT;
	// splits clock in two
	TIMER3_TBMR_R |= (TIMER_TBMR_TBCMR | TIMER_TBMR_TBCDIR | TIMER_TBMR_TBMR_CAP);
	// set to count up
	TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;
	// set timer3 event mode both rising and falling
	TIMER3_ICR_R |= TIMER_ICR_CBECINT;
	// clear status flags
	TIMER3_IMR_R |= TIMER_IMR_CBEIM;
	// set interrupt mask


	NVIC_EN1_R |= 0x10;
	//initilize local intrupts

	IntRegister(INT_TIMER3B, TIMER3B_Handler);
	IntMasterEnable();

	TIMER3_CTL_R |= TIMER_CTL_TBEN;
	// set timer3 enabled to port B

}


void TIMER3B_Handler(void)
{
 // int ctime = TIMER3_TBR_R;

	TIMER3_ICR_R |= TIMER_IMR_CBEIM;
	// clear status flags



	if (current_state == 0)
	{
		// save current count to rising edge
		current_state = 1;
		rising_edge_time = TIMER3_TBV_R;
	}
	else if (current_state == 1)
	{
		// save current count to falling edge
		current_state = 2;
		falling_edge_time = TIMER3_TBV_R;
	}

//
//
//	if(!leventtime){
//		leventtime = TIMER3_TBR_R;
//	}
//	else if(leventtime == -1);
//	else{

//
//		else{
//			pulsewidth = TIMER3_TBR_R - leventtime;
//			distance = ((0.002125 * pulsewidth) / 2) - 13;
//		}
//
//	}



}


void send_pulse()
{
	//leventtime = -1;// sending pulse
	GPIO_PORTB_PCTL_R &= ~GPIO_PCTL_PB3_T3CCP1;


	GPIO_PORTB_AFSEL_R &= 0xF7; //switch off

	GPIO_PORTB_DIR_R |= 0x08; // set PB3 as output
	GPIO_PORTB_DATA_R |= 0x08; // set PB3 to high
	timer_waitMicros(20);// wait at least 5 microseconds based on data sheet
	GPIO_PORTB_DATA_R &= 0xF7; // set PB3 to low
	GPIO_PORTB_DIR_R &= 0xF7; // set PB3 as input

	GPIO_PORTB_AFSEL_R |= 0x08; //switch on
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;

	current_state = 0;

}

int sonar_getDistance()
{
	send_pulse();
	while(current_state != 2);

	if (falling_edge_time < rising_edge_time) // overflow occured
	{
		falling_edge_time += 16777216-1;
		overflow++;
	}


	return ((17000*(falling_edge_time - rising_edge_time)) / 16000000);

	// ping
	// while 2
	// do math
}


int main(){
	char *s;
	s = "hello";
	lcd_init();
	regestersinit();
	timerinit();
	lcd_printf(s);
	//overflow = 0;
	while(1){
		//while (current_state != 2);
		timer_waitMillis(200);
		//if(distance > 0){
		lcd_printf("Distance:%d \nDelta: %d\nOverflow:%d", sonar_getDistance() , falling_edge_time - rising_edge_time, overflow);
		//}

	}
}


