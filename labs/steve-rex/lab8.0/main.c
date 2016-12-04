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

int DEGREE0 = 6950;
int DEGREE180 = 34200;
int DEGREE_90 = ((34200 - 6950)/2) + 6950;
int DEGREE_MULTIPLIER =  (int)(34200 - 6950) / 180;

void TIMER3B_Handler(void);

double degrees = 90;


//#define T3_CTL = (*((volatile unsigned long *)0x4003300C))



void regestersinit(){

	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	//enable clock to GPIO, R1 = port B
		//GPIO_PORTB_DEN_R &= 0xDF;
		// set pin 5
	GPIO_PORTB_AFSEL_R |= 0x20;
	//set alternative function T1CCP1
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB5_T1CCP1;//GPIO_PCTL_PB3_T3CCP1;
	//set it to alternative function T1CCP1

	GPIO_PORTB_DIR_R |= 0x20;
	//sets direction to output for pin 5
	GPIO_PORTB_DEN_R |= 0x20;
	//re-enable digital functionality for pin 5

}

void timerinit(){

	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1; // Turn on clock to TIMER1

	TIMER1_CTL_R &= ~TIMER_CTL_TBEN;
	// set timer1 disabled to port B
	TIMER1_CFG_R |= TIMER_CFG_16_BIT;
	// splits clock in two
//	TIMER1_TBMR_R &= (~TIMER_TBMR_TBCMR);
	TIMER1_TBMR_R |= (TIMER_TBMR_TBAMS | TIMER_TBMR_TBMR_PERIOD);//(TIMER_TBMR_TBCMR | TIMER_TBMR_TBCDIR | TIMER_TBMR_TBMR_CAP); //TnAMS bit to 0x1, the TnCMR bit to 0x0, and the TnMR field to 0x2.

	TIMER1_CTL_R |= TIMER_CTL_TBPWML;
	// set timer1 to pwm mode to normal (not inverted)
	TIMER1_TBILR_R = 0x4E200 & 0xFFFF; //320000
	TIMER1_TBPR_R = 0x4E200 >> 16;
	//set timer top
	TIMER1_TBMATCHR_R = DEGREE_90;//22000 - 90 degrees
	//set match value to 1ms or 0 degrees
	TIMER1_CTL_R |= TIMER_CTL_TBEN;
	// set timer1 enabled to port B

}

void degree_servo(double degree){
	int value = (TIMER1_TBMATCHR_R - DEGREE0) / DEGREE_MULTIPLIER;
	TIMER1_TBMATCHR_R = (degree * DEGREE_MULTIPLIER) + DEGREE0;
	value -= degree;
	if(value < 0 ){
		value = value *-1;
	}
	timer_waitMillis(value * 5);
}

void move_servo(double degree, int dir){
	if(degrees + (degree * dir) < 0 || degrees + (degree * dir) > 180){
		return;
	}
	TIMER1_TBMATCHR_R += (degree * DEGREE_MULTIPLIER )* dir;
}
/* degrees	  value
 * cybot 3
 * 0 		: 8000
 * 180 		: 36000
 * cybot 2
 * 0		: 6950
 *180		: 34200
 */
void degreesweep(void){
	lcd_printf("DEMO");
	degree_servo(0);
	//timer_waitMillis(1000);
	degree_servo(45);
	//timer_waitMillis(1000);
	degree_servo(90);
	//timer_waitMillis(1000);
	degree_servo(135);
	//timer_waitMillis(1000);
	degree_servo(180);
	//timer_waitMillis(1000);
	degree_servo(90);

}
int main(){
	regestersinit();
	timerinit();
	button_init();
	lcd_init();

	char *direction;
	int btn = 0;
	int dir = 1;
	direction = "up";
	while(1){
		if(btn == 0){
			btn = button_getButton();
			switch(btn){
			case 1:// 1 degree
				move_servo(1,dir);
				break;
			case 2:// 2.5 degrees
				move_servo(2.5,dir);
				break;
			case 3:// 5 degrees
				move_servo(5,dir);
				break;
			case 4:// switch between incriment or decriment
				if(dir == 1){
					dir = -1;
					direction = "down";
				}
				else{
					dir = 1;
					direction = "up";
				}
				break;
			case 5:
				degreesweep();
				break;
			}
		}
		else{
			btn = button_getButton();
		}
		degrees = (TIMER1_TBMATCHR_R - DEGREE0) / DEGREE_MULTIPLIER;
		lcd_printf("cur value: %d\ndir: %s\ndegrees: %f",TIMER1_TBMATCHR_R, direction, degrees );

	}//indicate increase or decrease degrees

}


