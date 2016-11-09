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
#include <stdlib.h>
#define PI 3.14159265

void part1();
void part2();
void analyze();
void find_width();
void sonar_init();
void ir_sensor_init();
void servo_init();
void TIMER3B_Handler(void);
int getADC();
void send_pulse();
void move_servo(double degree);
unsigned time2distance();
void interpret_buttons(int button);



typedef struct{
	int start_degree;
	int end_degree;
	int width;
	int distance;
	int number;
}Objects;


//void ir_sensor_init();
//int getADC();
double get_distance();

int pulse_period = 320000;	//clock cycles ~20ms pulse width
int decrementing = 0;	//decrementing = 1, incrementing = 0;
double count = 0;			//location in degrees of the sonar
double mid_width = 24000;	//up time of clock cycle
double max_servo = 34205;	//cybot 7 36977		cybot 2 34205
double min_servo = 6829;	//cybot 7 8696		cybot 2 6829
volatile enum {LOW, HIGH, DONE, READY} state, state_ir;
volatile unsigned int initial_value = 0;
volatile unsigned int second_value = 0;
volatile unsigned int final_value = 0;
volatile float distance = 0;
volatile int ir_adc = 0, initial = 1;

int ir_measure[180];
int obj_count = 0;
int sonar_dist = 0;
double ir_dist = 0;
Objects obj[10];
char send_string[100] = "Degrees\t\tIR Distance (cm)\tSonar Distance (cm)\r\n";
int skip[10];

int main(void){



	int i = 0;


	for(i = 0; i < 10; i++)
	{
		skip[i] = 0;
	}
	for(i = 0; i<180; i++)
	{
		ir_measure[i] = -1;

	}

	for(i = 0; i < 10; i ++)
	{
		obj[i].start_degree = -1;
		obj[i].end_degree = -1;
		obj[i].number = -1;
		obj[i].width = -1;
	}

	lcd_init();
	button_init();
	sonar_init();
	ir_sensor_init();
	servo_init();
	uart_init();
//	GPIO_init();

//	ir_sensor_init();
//	WiFi_stop();
//	WiFi_start("PplmcWtft!");
//	interpret_buttons(7);	//so we can move the servo to 90 degrees
	uart_receive();
	move_servo(0);
	timer_waitMillis(1000);
	state_ir = state = LOW;
	uart_flush();

	uart_sendStr(send_string);
	uart_flush();
	initial = 0;
	part2();

	return 0;
}

void part1()
{
	while(1)
	{
		if(state_ir == LOW && state == LOW)
		{
			send_pulse();
			ir_adc = getADC();
			state_ir = HIGH;
			timer_waitMillis(500);

		} else if(state_ir == HIGH && state == HIGH) {
			sonar_dist = time2distance();
			ir_dist = get_distance();
//			sonar_dist = time2distance();
			sprintf(send_string, "%d\t\t%d\t\t%d\r\n\0", (int)count, (int)ir_dist, sonar_dist);
			uart_sendStr(send_string);
			interpret_buttons(2);
			state_ir = LOW;
			state = LOW;
//			timer_waitMillis(500);
		}
	}
}

void part2(){

	int ir_dist2 = 0, ir_dist3 = 0;
	int i = 0;
	for(i = 0; i < 180; i++)
	{
	//part 2 ir stuff
		ir_adc = getADC();
		ir_dist = get_distance();
		timer_waitMillis(10);

		ir_adc = getADC();
		ir_dist2 = get_distance();
		timer_waitMillis(10);
		ir_adc = getADC();
		ir_dist3 = get_distance();

		ir_dist = (ir_dist + ir_dist2 + ir_dist3)/3;


//		ir_dist = get_distance();
		if(ir_dist > 34 && ir_dist < 80)
		{
			ir_measure[i] = ir_dist;

//				obj_count++;
		}
		sprintf(send_string, "%d\t\t%d\r\n\0", (int)i, (int)ir_dist);
		uart_sendStr(send_string);
		interpret_buttons(1);
		timer_waitMillis(50);

	}

	analyze();
	find_width();

	initial = 0;
	for(i = 0; i < obj_count; i ++)
	{
		if(obj[i].width < obj[initial].width && skip[i] != 1)
		{
			initial = i;
		}
	}

	lcd_printf("Smallest object, object %d", initial);
	move_servo((obj[initial].end_degree - obj[initial].start_degree)/2 + obj[initial].start_degree);


	sprintf(send_string, "number of objects found %d", obj_count);
	uart_sendStr(send_string);
}

void analyze()
{
	int i = 0;
	for(i = 1; i < 179; i++)
	{
		if((ir_measure[i-1] == -1 && ir_measure[i+1] == -1) && ir_measure[i] >=0)
		{
			ir_measure[i] = -1;

			//obj_count++;
		} else if(ir_measure[i-1] >= 0 && ir_measure[i+1] >= 0 && ir_measure[i] == -1) {
			ir_measure[i] = 1;

		} else if((ir_measure[i-1] == -1 && ir_measure[i] >= 0)){
			obj[obj_count].number = obj_count;
			obj[obj_count].start_degree = i;


		} else if((ir_measure[i-1] >= 0 && ir_measure[i] == -1)) {
			obj[obj_count].end_degree = (i-1);
			//calculate width here
			obj_count++;
		} else if(i == 178 && obj[obj_count].start_degree >= 0 && obj[obj_count].end_degree == -1) {
			obj[obj_count].end_degree = 180;
			obj_count++;
		}
	}

	if(obj[obj_count].start_degree >= 170 && obj[obj_count].end_degree == -1)
	{
		obj[obj_count].end_degree = 180;
		obj_count++;
	}

}

void find_width()
{
	int i = 0;


	for(i = 0; i < obj_count; i++)
	{
		state = LOW;
		int degree = (obj[i].end_degree - obj[i].start_degree);
		degree = degree + obj[i].start_degree;
		move_servo(degree);
		timer_waitMillis(500);
		send_pulse();
		degree = degree - obj[i].start_degree;
		while(state != READY){}


		obj[i].distance = time2distance();
		if(obj[i].distance < 80 && obj[i].distance > 34)
		{
//			obj[i].width = sqrt(((ir_measure[(obj[i].start_degree)])*(ir_measure[(obj[i].start_degree)]))+(ir_measure[(obj[i].end_degree)])*(ir_measure[(obj[i].end_degree)]) - 2*(ir_measure[(obj[i].start_degree)])*(ir_measure[(obj[i].end_degree)])*cos((degree)));
				obj[i].width = (2*obj[i].distance*tan((((degree/2)*PI)/180)));	//*PI/180
		//		sprintf(send_string, "Object degree %d\t distance: %d\n");
			sprintf(send_string, "Object degree %d distance %d width %d\r\n", degree + obj[i].start_degree, obj[i].distance, obj[i].width);
		} else {
			skip[i] = 1;
			sprintf(send_string, "Object %d out of range, ignoring values\r\n", i);
		}
		uart_sendStr(send_string);
		lcd_printf("%s\n", send_string);
	//		timer_waitMillis(500);
	}
}


void sonar_init(){
	//initialize the timer
	//initialize the gpio port
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	GPIO_PORTB_DEN_R |= 0x08;
	GPIO_PORTB_AFSEL_R &= (~0x04);

	//Timer
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

void servo_init(){
	//initialize the timer
	//***set GPIO PB5, turn on clk, alt. function, output, enable***

	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1; //turn on clk for timer1
	TIMER1_CTL_R &= ~(TIMER_CTL_TBEN);//disable timer to config

	TIMER1_TBMR_R |= (TIMER_TBMR_TBAMS | TIMER_TBMR_TBMR_PERIOD ); //periodic and PWM enable
	TIMER1_CFG_R = TIMER_CFG_16_BIT; //set size of timer to 16

	TIMER1_CTL_R |= TIMER_CTL_TBEN; //enable timer

	//initialize the gpio port
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	GPIO_PORTB_DEN_R &= 0xDF; //0b1101 1111
	GPIO_PORTB_AFSEL_R |= 0x20;
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB5_T1CCP1;
	GPIO_PORTB_DIR_R |= 0x20; // set PB5 as output 0b0010 0000
	GPIO_PORTB_DEN_R |= 0x20;
}

void TIMER3B_Handler(void){
	TIMER3_ICR_R |= TIMER_IMR_CBEIM;

 	if(state == LOW){
		initial_value = TIMER3_TBV_R;
		state = HIGH;
	} else {
		second_value = TIMER3_TBV_R;
		state = READY;			//COMMENT OUT FOR PART 1
	}

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

void move_servo(double degree)
{
//	unsigned pulse_width; // pulse width in cycles
	// calculate pulse width in cycles
	//TIMER0_TAMATCHR_R = period_width - pulse_width; // set pulse width
	// you need to call timer_waitMillis( ) here to enforce a delay for the servo to
	// move to the position
	 if(degree > 180)
	 {
		 degree = 180;
	 } else if(degree < 0)
	 {
		 degree = 0;
	 }
 	 mid_width = 16000 + (4000/45)*degree;
	 mid_width = (((mid_width - 16000)*(max_servo - min_servo))/(32000-16000)) + min_servo;

	TIMER1_TBILR_R = pulse_period & 0xFFFF; //lower 16 bits of the interval
	TIMER1_TBPR_R = (pulse_period >> 16) & 0xFF; //set the upper 8 bits of the interval

	TIMER1_TBMATCHR_R = ((int)(pulse_period - mid_width)) & 0xFFFF;
	TIMER1_TBPMR_R = (((int)(pulse_period - mid_width)) >> 16) & 0xFF;

	timer_waitMillis(100);

}

double get_distance()
{
//	double dist = pow(getADC(), -1.186);	//cybot 7
//	dist = 120008*dist;
	double dist = pow(getADC(), -0.965);
	dist = 11953*dist;
	state_ir = DONE;
	return dist;
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

unsigned time2distance(){
	int overflow = 0, value=0, final_value=0, distance=0;

	if(second_value < initial_value)
	{
		overflow = 1;
		value = 16000000;
		final_value = (value - initial_value) + second_value;
	} else {
		final_value = second_value - initial_value;
	}
//	initial_value = 0;
//	second_value = 0;
//		lcd_clear();
//		lcd_printf("final_value: %hu\n", final_value);

	distance = ((0.002125 * final_value)/2);
//		final_value = final_value * 0.0000625;
	lcd_printf("final_value: %.2f\ndistance: %.2f\noverflow: %d", (final_value * 0.0000625), distance, overflow);
//	final_value = 0;
//	distance = 0;

	initial_value = 0;
	second_value = 0;
	state = DONE;
	return distance;

//	return 0;
}










