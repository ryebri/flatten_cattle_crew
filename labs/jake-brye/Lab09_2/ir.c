#include "ir.h"

void ir_sensor_init(){
	//initialize gpio
	//enable ADC 0 module on port B
	SYSCTL_RCGCGPIO_R |= 0x2;

	//enable clock for ADC0
	SYSCTL_RCGCADC_R |= 0x1;

	//enable port D pin 0 to work as alternate functions
	GPIO_PORTB_AFSEL_R |= 0x40;

	//set pin to input - 0
	GPIO_PORTB_DEN_R &= 0b11111111;

	//disable analog isolation for the pin
	GPIO_PORTB_AMSEL_R |= 0x40;

	//initialize the port trigger source as processor (default)
	GPIO_PORTB_ADCCTL_R = 0x00;

	//initialize the adc
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

double get_distance()
{
	double dist = pow(getADC(), -1.186);
	dist = 120008*dist;
	state_ir = DONE;
	return dist;
}