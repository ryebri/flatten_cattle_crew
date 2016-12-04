/*
 * botdata.c
 *
 *  Created on: Dec 3, 2016
 *      Author: borseth
 */
#include "botdata.h"
#include <math.h>
#include "open_interface.h"


void botdata_init(botdata_t *bdata){
	int i;
	for(i = 0; i < 10; i++)
	{
		bdata->skip[i] = 0;
		//rec_string[i] = '\0';
	}
	for(i = 0; i<180; i++)
	{
		bdata->ir_measure[i] = -1;
		bdata->ir_measure_raw[i] = -1;

	}

	for(i = 0; i < 10; i ++)
	{
		bdata->obj[i].start_degree = -1;
		bdata->obj[i].end_degree = -1;
		bdata->obj[i].number = -1;
		bdata->obj[i].width = -1;
	}
}


double get_distance()
{
//	double dist = pow(getADC(), -1.186);	//cybot 7
//	dist = 120008*dist;
//	double dist = pow(getADC(), -0.965);	//cybot 2?
//	dist = 11953*dist;
	double dist = pow(getADC(),-1.147);		//cybot 4
	dist= 74644*dist;
	return dist;
}

int getADC(){
	ADC0_PSSI_R=ADC_PSSI_SS0;
	//wait for ADC conversion to be complete
	while((ADC0_RIS_R & ADC_RIS_INR0) == 0){
	//wait
	}
	//grab result
	ADC0_ISC_R=ADC_ISC_IN0;
	int value = ADC0_SSFIFO0_R;
	return value;
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
