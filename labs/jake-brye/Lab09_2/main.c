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
#include "sonar.h"
#include "ir.h"
#define PI 3.14159265

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
double max_servo = 36977;
double min_servo = 8696;
volatile enum {LOW, HIGH, DONE, READY} state, state_ir;
volatile unsigned int initial_value = 0;
volatile unsigned int second_value = 0;
volatile unsigned int final_value = 0;
volatile float distance = 0;
volatile int ir_adc = 0, initial = 1;

int ir_measure[90];
int obj_count = 0;


int main(void){

	Objects obj[10];

	int i = 0;
	for(i; i<90; i++)
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
	int sonar_dist = 0;
	double ir_dist = 0;
//	ir_sensor_init();
//	WiFi_stop();
	WiFi_start("PplmcWtft!");
//	interpret_buttons(7);	//so we can move the servo to 90 degrees
	move_servo(0);
	timer_waitMillis(1000);
	state_ir = state = LOW;
	uart_flush();
	char send_string[100] = "Degrees\t\tIR Distance (cm)\tSonar Distance (cm)\r\n";
	uart_sendStr(send_string);
	uart_flush();
	initial = 0;
// make part 1 a function and make part 2 a function
//	while(1)
	for(i = 0; i < 90; i++)
	{
//		if(state_ir == LOW && state == LOW)
//		{
//			send_pulse();
//			ir_adc = getADC();
//			state_ir = HIGH;
//			timer_waitMillis(500);
//
//		} else if(state_ir == HIGH && state == HIGH) {
//			sonar_dist = time2distance();
//			ir_dist = get_distance();
////			sonar_dist = time2distance();
//			sprintf(send_string, "%d\t\t%d\t\t%d\r\n\0", (int)count, (int)ir_dist, sonar_dist);
//			uart_sendStr(send_string);
//			interpret_buttons(2);
//			state_ir = LOW;
//			state = LOW;
////			timer_waitMillis(500);
//		}

//			send_pulse();

		//part 2 ir stuff
			ir_adc = getADC();

			timer_waitMillis(200);

			ir_dist = get_distance();
			if(ir_dist > 34 && ir_dist < 80)
			{
				ir_measure[i] = ir_dist;

//				obj_count++;
			}
			interpret_buttons(2);

	}


	//make this a function
	for(i = 1; i < 90; i++)
	{
		if(ir_measure[i-1] == -1 && ir_measure[i] >= 0)
		{
//			obj[obj_count] = (Objects*)malloc(sizeof(Objects));

			obj[obj_count].number = obj_count;
			obj[obj_count].start_degree = i*2;

			//obj_count++;
		} else if(ir_measure[i-1] >= 0 && ir_measure[i] == -1) {
			obj[obj_count].end_degree = (i-1)*2;
			//calculate width here
			obj_count++;
		}
	}
	//make this a separate function
	for(i = 0; i < obj_count; i++)
	{
		state = LOW;
		int degree = (obj[i].end_degree - obj[i].start_degree)/2;
		degree = degree + obj[i].start_degree;
		move_servo(degree);
		timer_waitMillis(500);
		send_pulse();

		while(state != READY){}


		obj[i].distance = time2distance();
		obj[i].width = sqrt(((ir_measure[(obj[i].start_degree/2)])*(ir_measure[(obj[i].start_degree/2)]))+(ir_measure[(obj[i].end_degree/2)])*(ir_measure[(obj[i].end_degree/2)]) - 2*(ir_measure[(obj[i].start_degree/2)])*(ir_measure[(obj[i].end_degree/2)])*cos((degree)));
//		obj[i].width = (2*obj[i].distance*tan(((degree)/2)));	//*PI/180
//		sprintf(send_string, "Object degree %d\t distance: %d\n");
		lcd_printf("Object degree %d\ndistance %d\n", degree, obj[i].distance);
//		timer_waitMillis(500);
	}

	//make this a separate function
	initial = 0;
	for(i = 0; i < obj_count; i ++)
	{
		if(obj[i].width < obj[initial].width)
		{
			initial = i;
		}
	}

	lcd_printf("Smallest object, object %d", initial);


	sprintf(send_string, "number of objects found %d", obj_count);
	uart_sendStr(send_string);


	return 0;
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











