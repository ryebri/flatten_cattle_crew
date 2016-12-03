/*
 * main.c
 *
 *  Created on: Oct 4, 2016
 *      Author: borseth
 */

#include "lcd.h"
#include "timer.h"
#include "button.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"


void regestersinit(){

	//enable ADC 0 module on port B
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	//enable clock for ADC
	SYSCTL_RCGCADC_R |= 0x1;
	//enable port B pin 0 to work as alternate functions
	GPIO_PORTB_AFSEL_R |= 0x08;
	//set pin to input - 0
	GPIO_PORTB_DEN_R &= 0x08;
	//disable analog isolation for the pin
	GPIO_PORTB_AMSEL_R |= 0x08;
	//initialize the port trigger source as processor (default)
	GPIO_PORTB_ADCCTL_R = 0x00;

	//disable SS0 sample sequencer to configure it
	ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;
	//initialize the ADC trigger source as processor (default)
	ADC0_EMUX_R = ADC_EMUX_EM0_PROCESSOR;
	//set 1st sample to use the AIN10 ADC pin
	ADC0_SSMUX0_R |= 0x000A;
	//enable raw interrupt
	ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);
	//enable oversampling to average
	ADC0_SAC_R |= ADC_SAC_AVG_64X;
	//re-enable ADC0 SS0
	ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;
/*
	//clear interrupt flags
	ADC0_ISC_R |= ADC_ISC_IN0;
	//enable ADC0SS0 interrupt
	ADC0_IM_R |= ADC_IM_MASK0;
	//enable interrupt for IRQ 14 set bit 14
	NVIC_EN0_R |= 0x00004000;
	//tell cpu to use ISR handler for ADC0SS0
	IntRegister(INT_ADC0SS0, ADC0SS0_Handler);
	//enable global interrupts
	IntMasterEnable();
*/
}

int irdistance(){
	int value;
	ADC0_PSSI_R=ADC_PSSI_SS0;
	//wait for ADC conversion to be complete
	while((ADC0_RIS_R & ADC_RIS_INR0) == 0){}
	ADC0_ISC_R=ADC_ISC_IN0;
	value = ADC0_SSFIFO0_R;
	return 291724* pow(value,-1.33);//y = 291724x-1.33

}



int main(){
	regestersinit();
	lcd_init();
	int distance;

	while(1){
/*
		ADC0_PSSI_R=ADC_PSSI_SS0;
		//wait for ADC conversion to be complete
		while((ADC0_RIS_R & ADC_RIS_INR0) == 0){}
		ADC0_ISC_R=ADC_ISC_IN0;
		value = ADC0_SSFIFO0_R;
*/
		distance = irdistance();//291724* pow(value,-1.33);//y = 291724x-1.33
		timer_waitMillis(200);
		lcd_printf("distance: %d",distance);
		timer_waitMillis(50);

	}
}



