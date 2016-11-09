/*
 * main.c
 */
#include <stdbool.h>
#include "open_interface.h"
#include "timer.h"
#include "lcd.h"
#include "button.h"
#include "uart.h"
#include "WiFi.h"
#include <math.h>
#include "driverlib/interrupt.h"

void timer1_init();
void gpiopb_init();
void move_servo(double  degree);
void interpret_buttons(int button);

void clock_timer_init(void);
void send_pulse();
unsigned time2distance();
//void gpio_init();


void ir_sensor_init();
int getADC();
double get_distance();

int pulse_period = 320000;	//clock cycles ~20ms pulse width
int decrementing = 0;	//decrementing = 1, incrementing = 0;
double count = 90;			//location in degrees of the sonar
double mid_width = 24000;	//up time of clock cycle
double max_servo = 36977;
double min_servo = 8696;
volatile enum {LOW, HIGH, DONE} state, state_ir;
volatile unsigned int initial_value = 0;
volatile unsigned int second_value = 0;
volatile unsigned int final_value = 0;
volatile float distance = 0;
volatile int ir_adc = 0, initial = 1;



int main(void){

	clock_timer_init();

	lcd_init();
	button_init();
	gpiopb_init();
	timer1_init();
	uart_init();
//	GPIO_init();
	int sonar_dist = 0;
	double ir_dist = 0;
	ir_sensor_init();

//	WiFi_start("PplmcWtft!");
	interpret_buttons(7);	//so we can move the servo to 90 degrees
	move_servo(0);
	state_ir = state = LOW;
	uart_flush();
	char send_string[100] = "Degrees\t\tIR Distance (cm)\tSonar Distance (cm)\r\n";
	uart_sendStr(send_string);
	uart_flush();
	initial = 0;
	while(1)
	{
		if(state_ir == LOW && state == LOW)
		{
			send_pulse();
			ir_adc = getADC();
			state_ir = HIGH;

		} else if(state_ir == HIGH && state == HIGH) {
			sonar_dist = time2distance();
			ir_dist = get_distance();
			sprintf(send_string, "%d\t\t%d\t\t%d\r\n\0", (int)count, (int)ir_dist, sonar_dist);
			uart_sendStr(send_string);
			interpret_buttons(2);
			state_ir = state = LOW;
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

	GPIO_PORTB_DEN_R |= 0x08;
	GPIO_PORTB_AFSEL_R &= 0x00;

	GPIO_PORTB_AFSEL_R |= 0x20;
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB5_T1CCP1;
	GPIO_PORTB_DIR_R |= 0x20; // set PB5 as output 0b0010 0000
	GPIO_PORTB_DEN_R |= 0x20;

}

void move_servo(double degree)
{

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
			degree = 2;
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
//	lcd_printf("count: %f\n decrementing: %d\nwidth: %f", count, decrementing, mid_width);
	move_servo(count);
}



void TIMER3B_Handler(void){
	TIMER3_ICR_R |= TIMER_IMR_CBEIM;

	if(state == LOW && initial != 1){
		initial_value = TIMER3_TBV_R;
		state = HIGH;
	} else if(initial != 1){
		second_value = TIMER3_TBV_R;

	}





}

/// Initialize the timers
void clock_timer_init(void) {

	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3; // Turn on clock to TIMER3

		//Configure the timer for input capture mode
		TIMER3_CTL_R &= ~(TIMER_CTL_TBEN); //disable timerB to allow us to change settings
		TIMER3_CFG_R |= TIMER_CFG_16_BIT; //set to 16 bit timer

		TIMER3_TBMR_R = (TIMER_TBMR_TBCMR | TIMER_TBMR_TBCDIR | TIMER_TBMR_TBMR_CAP); //set for capture mode, up count TIMER_TBMR_TBAMS?

		TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;

	//	TIMER3_TBILR_R = (int)(16000000/(50 * 5));
	//	TIMER3_TBILR_R = 0xFFFF;


		TIMER3_IMR_R |= (TIMER_IMR_CBEIM);	//sets the interrupt mask

		TIMER3_ICR_R |= (TIMER_ICR_CBECINT); //clears TIMER3 time-out interrupt flags

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

}


unsigned time2distance(){
	int overflow = 0, value=0, final_value=0, distance=0;

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
//	final_value = 0;
//	distance = 0;
	state = DONE;
	return distance;






//	return 0;
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
	state_ir = DONE;
	return dist;
}












