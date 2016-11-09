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

void timer1_init();
void gpiopb_init();
void move_servo(double  degree);
void interpret_buttons(int button);

int pulse_period = 320000;	//clock cycles ~20ms pulse width
int decrementing = 0;	//decrementing = 1, incrementing = 0;
double count = 90;			//location in degrees of the sonar
double mid_width = 24000;	//up time of clock cycle
double max_servo = 36977;
double min_servo = 8696;

int main(void){

	lcd_init();
	button_init();
	gpiopb_init();
	timer1_init();

	interpret_buttons(7);	//so we can move the servo to 90 degrees

	int button = 0;
	while(1)
	{
		button = button_getButton();
		if(button != 0)
		{
			interpret_buttons(button);
			button = 0;
		}
	}


	return 0;
}



void timer1_init()
{
//***set GPIO PB5, turn on clk, alt. function, output, enable***

	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1; //turn on clk for timer1
	TIMER1_CTL_R &= ~(TIMER_CTL_TBEN);//disable timer to config

	TIMER1_TBMR_R |= (TIMER_TBMR_TBAMS | TIMER_TBMR_TBMR_PERIOD ); //periodic and PWM enable
	TIMER1_CFG_R = TIMER_CFG_16_BIT; //set size of timer to 16

	TIMER1_CTL_R |= TIMER_CTL_TBEN; //enable timer
}


void gpiopb_init()
{
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	GPIO_PORTB_DEN_R &= 0xDF; //0b1101 1111
	GPIO_PORTB_AFSEL_R |= 0x20;
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB5_T1CCP1;
	GPIO_PORTB_DIR_R |= 0x20; // set PB5 as output 0b0010 0000
	GPIO_PORTB_DEN_R |= 0x20;

}

void move_servo(double degree)
{
//	unsigned pulse_width; // pulse width in cycles
	// calculate pulse width in cycles
	//TIMER0_TAMATCHR_R = period_width - pulse_width; // set pulse width
	// you need to call timer_waitMillis( ) here to enforce a delay for the servo to
	// move to the position
//	 if(degree > 180)
//	 {
//		 degree = 180;
//	 } else if(degree < 0)
//	 {
//		 degree = 0;
//	 }
 	 mid_width = 16000 + (4000/45)*degree;
	 mid_width = (((mid_width - 16000)*(max_servo - min_servo))/(32000-16000)) + min_servo;

	TIMER1_TBILR_R = pulse_period & 0xFFFF; //lower 16 bits of the interval
	TIMER1_TBPR_R = (pulse_period >> 16) & 0xFF; //set the upper 8 bits of the interval

	TIMER1_TBMATCHR_R = ((int)(pulse_period - mid_width)) & 0xFFFF;
	TIMER1_TBPMR_R = (((int)(pulse_period - mid_width)) >> 16) & 0xFF;

	timer_waitMillis(100);

}

void interpret_buttons(int button)
{
	double degree = 0;

	switch(button)
		{
		case 1:
			degree = 1;
			break;

		case 2:
			degree = 2.5;
			break;

		case 3:
			degree = 5;
			break;

		case 4:
			if(decrementing == 0)
			{
				decrementing = 1;

			} else {
				decrementing = 0;
			}
			break;

		default:
			break;

		}

	if(decrementing == 0)
	{
		count = count + degree;
	} else {
		count = count - degree;
	}

	if(count > 180)
	{
		count = 180;
	} else if(count < 0){
		count = 0;
	}
	lcd_printf("count: %f\n decrementing: %d\nwidth: %f", count, decrementing, mid_width);
	move_servo(count);
}













