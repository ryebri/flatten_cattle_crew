/*
 * main.c
 */
#include <stdbool.h>
#include "open_interface.h"
#include "movement.h"
#include "timer.h"
#include "lcd.h"
#include "button.h"
#include "uart.h"
#include "WiFi.h"
#include <math.h>
#include "driverlib/interrupt.h"
#include <stdlib.h>
#include "floorsensor.h"
#include "botdata.h"
#include "sweep.h"
#include "main.h"

void object_send(botdata_t *bdata, botpos_t *bpos);
void sonar_init();
void ir_sensor_init();
void servo_init();
void TIMER3B_Handler(void);
int getADC();
void interpret_buttons(int button);

//void ir_sensor_init();
//int getADC();
double get_distance();

int decrementing = 0;	//decrementing = 1, incrementing = 0;

double count = 0;			//location in degrees of the sonar
/*
int pulse_period = 320000;	//clock cycles ~20ms pulse width
double mid_width = 24000;	//up time of clock cycle
double max_servo = 34205;	//cybot 7 36977		cybot 2 34205
double min_servo = 6829;	//cybot 7 8696		cybot 2 6829
*/
volatile state_t state;
volatile unsigned int initial_value = 0;
volatile unsigned int second_value = 0;
volatile unsigned int final_value = 0;
volatile float distance = 0;
volatile int  initial = 1;

int sonar_dist;

/*
int ir_measure[180];
int ir_measure_raw[180];
int obj_count = 0;
Objects obj[10];
*/

//char rec_string[10]; // when using putty

int main(void){

	botdata_t bdata;
	botdata_init(&bdata);

	botpos_t bpos;
	botpos_init(&bpos);

	oi_t *openi = oi_alloc();
	oi_init(openi);

	lcd_init();
	button_init();
	sonar_init();
	ir_sensor_init();
	servo_init();
	uart_init();


//	GPIO_init();\
//	ir_sensor_init();
//	WiFi_stop();
	WiFi_start("PplmcWtft!");
	//interpret_buttons(7);	//so we can move the servo to 90 degrees
//	rec_string[i] = uart_receive();		//when using putty
//	oi_close();
	move_servo(0);
	bpos.sensor_data = openi;
	state = LOW;
	/*
	while(1){	//IR floor sensor testing
		oi_update(openi);
		sprintf(str,"L : %3d  FL: %3d  FR: %3d  R : %3d\n\r", openi->cliffLeftSignal,
													 openi->cliffFrontLeftSignal,
													 openi->cliffFrontRightSignal,
													 openi->cliffRightSignal);
		uart_sendStr(str);
		timer_waitMillis(200);
	}*/
	oi_setWheels(0,0);
	while(1){
//		interpret_movement(&bpos,&bdata,-1,-1);
		recieve_command(&bdata,&bpos);
	}


//	uart_flush();
//	uart_sendStr(send_string);
//	uart_flush();

	initial = 0;
	object_send(&bdata,&bpos);

	oi_close();

	return 0;
}


void object_send(botdata_t *bdata, botpos_t *bpos){ // analizes sweep data and sends json object
	char send_string[45];

	do_sweep(bdata);
	analyze(bdata);
	sendscandata(bdata);
	find_width(bdata, &state, &initial_value, &second_value);

	send_position(bpos);




	sprintf(send_string, "{\"sensors\": [%03d,%03d,%03d,%03d,%03d]}\n\0",cliffleftsurface(bpos), cliffleftfrontsurface(bpos),
		cliffrightfrontsurface(bpos), cliffrightsurface(bpos),3);

	uart_sendStr(send_string);
	uart_flush();
//print smallest object
	/*
	initial = 0;
	for(i = 1; i < bdata->obj_count; i ++)
	{
		if(bdata->skip[i] != 1 && bdata->obj[i].width < bdata->obj[initial].width)
		{
			initial = i;
		}
	}

	lcd_printf("Smallest object, object %d", initial);
	move_servo((bdata->obj[initial].end_degree - bdata->obj[initial].start_degree)/2 +
					bdata->obj[initial].start_degree);
*/
	// moved to find_width
	//sprintf(send_string, "number of objects found %d", bdata->obj_count);
	//uart_sendStr(send_string);
}

void send_position(botpos_t *bpos){
	char send_string[45];
	int rneg,fneg,rnegbool,lnegbool,banglefix;
	rneg = fneg = rnegbool = lnegbool = 0;
	if(bpos->right < 0){
		rneg = -1;
		rnegbool = 1;
	}
	else{
		rneg = 1;
		rnegbool = 0;
	}
	if(bpos->forward < 0){
		fneg = -1;
		lnegbool = 1;
	}
	else{
		fneg = 1;
		rnegbool = 0;
	}
	if(bpos->angle < 0)	banglefix = 360;
	else banglefix = 0;


	sprintf(send_string, "{\"position\": [%03d,%03d,%03d,%03d,%03d,%03d]}\n\0",bpos->right * rneg, rnegbool,bpos->forward * fneg, lnegbool, bpos->angle + banglefix, bpos->bump);
	uart_sendStr(send_string);
	uart_flush();

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




