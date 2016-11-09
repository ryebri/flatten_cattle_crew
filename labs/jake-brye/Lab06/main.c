/*
 * main.c
 */
#include "open_interface.h"
#include "timer.h"
#include "lcd.h"
#include "button.h"
#include <math.h>

void uart_init(void) {     //enable clock to GPIO, R1 = port B
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
void ir_sensor_init(){
	//enable ADC 0 module on port B
	SYSCTL_RCGCGPIO_R |= 0x2;

	//enable clock for ADC0
	SYSCTL_RCGCADC_R |= 0x1;

	//enable port D pin 0 to work as alternate functions
	GPIO_PORTB_AFSEL_R |= 0x08;

	//set pin to input - 0
	GPIO_PORTB_DEN_R &= 0b11111111;

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
}

int getADC() {
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
	double dist = pow(getADC(), -1.081);
	dist = 22407*dist;
	return dist;
}




int main(void) {
	ir_sensor_init();
	lcd_init();
	double dist;
	//initiate SS1 conversion

	while(1)
	{

		dist = get_distance();
		lcd_printf("Distance: %.2f cm", dist);
		timer_waitMillis(200);

	}

	
	return 0;
}
