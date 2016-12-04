/*
 * main.c
 *
 *  Created on: Oct 25, 2016
 *      Author: borseth
 */
#include "lcd.h"
#include "timer.h"
#include "button.h"
#include "WiFi.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"

typedef struct object{
	int xdir, ydir, width, angle, distance;
}object_t;


typedef struct object_map{
	int num_objects;
	object_t objects[10];

}object_map_t;


//servo
int DEGREE0 = 6950;
int DEGREE180 = 34200;
int DEGREE_90 = ((34200 - 6950)/2) + 6950;
int DEGREE_MULTIPLIER =  (int)(34200 - 6950) / 180;
int degrees = 90;

//sonar
volatile int current_state = 0;
volatile int rising_edge_time = 0;
volatile int falling_edge_time = 0;
volatile int overflow = 0;

//IR code

void irregestersinit(){

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
	//return value;
	return 291724* pow(value,-1.33);//y = 291724x-1.33 bot 3
	//return 102276* pow(value, -.769);// bot 10
	//return 3000000* pow(value, -1.298);// bot 103E+06x-1.298
	//return 11687* pow(value, -0.587);// bot 3 11687x-0.587

}

//Servo code

void servotimerinit(){

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

void servogpioinit(){

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

void degree_servo(double degree){ // moves to degree
	int value = (TIMER1_TBMATCHR_R - DEGREE0) / DEGREE_MULTIPLIER;
	TIMER1_TBMATCHR_R = (degree * DEGREE_MULTIPLIER) + DEGREE0;
	value -= degree;
	if(value < 0 ){
		value = value *-1;
	}
	timer_waitMillis(value * 3);
}

int move_servo(double degree, int dir){ // moves degree number of digrees in direction in dir
	if(degrees + (degree * dir) < 0 || degrees + (degree * dir) > 180){
		return degrees;
	}
	TIMER1_TBMATCHR_R += (degree * DEGREE_MULTIPLIER )* dir;
	return degrees = (TIMER1_TBMATCHR_R - DEGREE0) / DEGREE_MULTIPLIER;
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

//Ping code (sonar)

void pinggpioinit(){


	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	//enable clock to GPIO, R1 = port B
	GPIO_PORTB_DEN_R |= 0x08;
	// set pin 3
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;
	//turn on T3CCP1 for control

	GPIO_PORTB_PCTL_R &= ~GPIO_PCTL_PB3_T3CCP1;
	//turn off T3CCP1 for control

}


void TIMER3B_Handler(void)
{
 // int ctime = TIMER3_TBR_R;

	TIMER3_ICR_R |= TIMER_IMR_CBEIM;
	// clear status flags



	if (current_state == 0)
	{
		// save current count to rising edge
		current_state = 1;
		rising_edge_time = TIMER3_TBV_R;
	}
	else if (current_state == 1)
	{
		// save current count to falling edge
		current_state = 2;
		falling_edge_time = TIMER3_TBV_R;
	}

}

void pingtimerinit(){

	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3; // Turn on clock to TIMER3

	TIMER3_CTL_R &= ~TIMER_CTL_TBEN;
	// set timer3 disabled to port B
	TIMER3_CFG_R |= TIMER_CFG_16_BIT;
	// splits clock in two
	TIMER3_TBMR_R |= (TIMER_TBMR_TBCMR | TIMER_TBMR_TBCDIR | TIMER_TBMR_TBMR_CAP);
	// set to count up
	TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;
	// set timer3 event mode both rising and falling
	TIMER3_ICR_R |= TIMER_ICR_CBECINT;
	// clear status flags
	TIMER3_IMR_R |= TIMER_IMR_CBEIM;
	// set interrupt mask



	NVIC_EN1_R |= 0x10;
	//initilize local intrupts

	IntRegister(INT_TIMER3B, TIMER3B_Handler);
	IntMasterEnable();

	TIMER3_CTL_R |= TIMER_CTL_TBEN;
	// set timer3 enabled to port B

}

void send_pulse()
{
	//leventtime = -1;// sending pulse
	GPIO_PORTB_PCTL_R &= ~GPIO_PCTL_PB3_T3CCP1;


	GPIO_PORTB_AFSEL_R &= 0xF7; //switch off

	GPIO_PORTB_DIR_R |= 0x08; // set PB3 as output
	GPIO_PORTB_DATA_R |= 0x08; // set PB3 to high
	timer_waitMicros(20);// wait at least 5 microseconds based on data sheet
	GPIO_PORTB_DATA_R &= 0xF7; // set PB3 to low
	GPIO_PORTB_DIR_R &= 0xF7; // set PB3 as input

	GPIO_PORTB_AFSEL_R |= 0x08; //switch on
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;

	current_state = 0;

}

int sonar_getDistance()
{
	send_pulse();
	while(current_state != 2);

	if (falling_edge_time < rising_edge_time) // overflow occured
	{
		falling_edge_time += 65535-1; //old value: 16777216
		overflow++;
	}


	return ((17000*(falling_edge_time - rising_edge_time)) / 16000000);

	// ping
	// while 2
	// do math
}


int width_calc(int p1,int p2, int degrees){

	return sqrt(pow(p1,2.0) + pow(p2,2.0) - (2.0 * p1 * p2 * cos(degrees)));
}


int iravgdis(){
	int i, total;
	for(i = 0; i< 3; i++){
		total += irdistance();
		timer_waitMillis(50);
	}
	return total / 3;
}


int sweep(object_map_t *m){ //sweeps ir and returns
	int i, irdis, irdisprev, radialstart, radialsize;
	int ping1, ping2;
	char str[80];
	ping1 = 0;
	degree_servo(0);
	irdisprev = irdistance();

	radialstart = 0;
	m->num_objects = 0;

	for(i = 0; i< 180; i+=1){
		irdis = iravgdis();

		if(irdis <= 515 && !radialstart ){// if within 4 degrees after object and ping is within 5
			ping1 = sonar_getDistance();
			if( (!m->num_objects || (m->objects[m->num_objects - 1].angle + 8 <= i ||
								!(ping1 + 10 > m->objects[m->num_objects - 1].distance && ping1 - 10 < m->objects[m->num_objects - 1].distance ))) ){
				if(m->num_objects && m->objects[m->num_objects - 1].angle + 10 > i){
					m->num_objects--;
				}
				irdisprev = irdis;
				radialstart = i;
				if(ping1 > 80 || ping1 < 0  ){
					radialstart = 0;
				}
			}
		}
		if(irdis > 515 && radialstart){
			ping2 = sonar_getDistance();
			radialsize = i - radialstart;
			m->objects[m->num_objects].angle = i - (radialsize / 2);
			m->objects[m->num_objects].width = width_calc(ping1, ping2, radialsize);
			m->objects[m->num_objects].distance = ping1;
			m->num_objects++;
			radialstart = 0;
		}
		lcd_printf("objects: %d\nIR distance:%d\noverflow: %d" ,m->num_objects, irdis, overflow);
		//timer_waitMillis(150);
		degree_servo(i);
		sprintf(str, "%d\t\t\t%d\t\t\t%d\r\n",i,irdis,ping1);
		uart_sendStr(str);
		//irdisprev = irdis;
	}

	return 0;
}

int pointleast(object_map_t *m){// returns the index of the smallest object
	int i,smallest,smalli;
	char str[80];
	i = 0;
	smallest = m->objects[i].width;
	smalli = 0;
	sprintf(str,"object %d:\nwidth: %d | angle: %d\r\n", i, m->objects[i].width, m->objects[i].angle);
	uart_sendStr(str);
	for(i = 1; i < m->num_objects; i++){
		sprintf(str,"object %d:\nwidth: %d | angle: %d\r\n", i, m->objects[i].width, m->objects[i].angle);
		uart_sendStr(str);
		if(m->objects[i].width < smallest){
			smallest = m->objects[i].width;
			smalli = i;
		}
	}
	degree_servo(m->objects[smalli].angle);
	return smalli;
}

void ircheck(void){
	int val;
	while(1){
		val = irdistance();
		lcd_printf("ir distance: %d",val);
		timer_waitMillis(50);
	}
}


 int main(){
	lcd_init();
	button_init();
	irregestersinit();
	servogpioinit();
	servotimerinit();
	pinggpioinit();
	pingtimerinit();
	WiFi_start("knotchar");
	//WiFi_start("%d\t\t\t%d\t\t%d",1,2,3);
	int index_least;
	object_map_t map;
	map.num_objects = 0;
	/*
	 * sweep ir and count objects
	 * sweep pint and get distance
	 * display smallest object
	 */

	degree_servo(90);

	uart_sendStr("Degrees		IR Distance (cm)		Sonar Distance (cm)\r\n");

	while(1){

		sweep(&map);
		index_least = pointleast(&map);
		//ircheck();
		lcd_printf("smallest object index: %d", index_least);
		timer_waitMillis(5000);
	}

	WiFi_stop();

}





