/*
 * botdata.c
 *
 *  Created on: Dec 3, 2016
 *      Author: borseth
 */
#include "botdata.h"
#include <math.h>
#include "open_interface.h"
#include "uart.h"
#include "movement.h"

///Initializes the values in bdata of type botdata_t
/**
*initializes all of the values in skip to 0
*initializes all of the values in ir_measure to -1
*initializes all of the values in start_degree, end degree, number, and width to -1
*@param bdata holds the data values that are to be initialized
*/
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

///Gets the distance of the IR sensor and returns the distance
/**
*calculates the IR sensor off of the calculation that we made earlier in excel
*/
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

///Returns value from IR sensor
/**
*performs an ADC to get a value from analog to digital of the IR sensor
*This value is returned at the end of the function 
*/
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

///Sends pulse from IR sensor
/**
*Send pulse to port B to the IR sensor and waits 10 microseconds before turning off the sensor. 
*turn on - wait - turn off  port B
*/
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




