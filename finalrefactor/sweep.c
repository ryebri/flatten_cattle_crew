/*
 * sweep.c
 *
 *  Created on: Dec 3, 2016
 *      Author: borseth
 */
#include "sweep.h"
#include "botdata.h"
#include "timer.h"
#include "math.h"
#include "lcd.h"
#include "uart.h"
#include "open_interface.h"

void do_sweep(botdata_t *bdata){

	int ir_dist = 0, ir_dist2 = 0, ir_dist3 = 0;
	int i = 0;

	for(i = 0; i < 180; i++)
	{
	//part 2 ir stuff
		ir_dist = get_distance();
		timer_waitMillis(3);

		ir_dist2 = get_distance();
		timer_waitMillis(3);

		ir_dist3 = get_distance();

		ir_dist = (ir_dist + ir_dist2 + ir_dist3)/3;

//		ir_dist = get_distance();

		bdata->ir_measure_raw[i] = ir_dist;
		if(ir_dist > 34 && ir_dist < 80)
		{
			bdata->ir_measure[i] = ir_dist;

//				obj_count++;
		} else if(ir_dist <= 10){
			bdata->ir_measure[i] = 0;
		}
//		sprintf(send_string, "%d\t\t%d\r\n\0", (int)i, (int)ir_dist);
//		uart_sendStr(send_string);
		interpret_buttons(1);
		timer_waitMillis(10);

	}
}


void analyze(botdata_t *bdata) // checks for objects
{
	int i = 0;
	for(i = 1; i < 179; i++)
	{
		if((bdata->ir_measure[i-1] == -1 && bdata->ir_measure[i+1] == -1) &&
					bdata->ir_measure[i] >=0)
		{
			bdata->ir_measure[i] = -1;

			//obj_count++;
		} else if(bdata->ir_measure[i-1] >= 0 && bdata->ir_measure[i+1] >= 0 &&
					bdata->ir_measure[i] == -1) {
			bdata->ir_measure[i] = 1;

		} else if((bdata->ir_measure[i-1] == -1 && bdata->ir_measure[i] >= 0)){
			bdata->obj[bdata->obj_count].number = bdata->obj_count;
			bdata->obj[bdata->obj_count].start_degree = i;


		} else if((bdata->ir_measure[i-1] >= 0 && bdata->ir_measure[i] == -1)) {
			bdata->obj[bdata->obj_count].end_degree = (i-1);
			//calculate width here
			bdata->obj_count++;
		} else if(i == 178 && bdata->obj[bdata->obj_count].start_degree >= 0 &&
					bdata->obj[bdata->obj_count].end_degree == -1) {
			bdata->obj[bdata->obj_count].end_degree = 180;
			bdata->obj_count++;
		}
	}

	if(bdata->obj[bdata->obj_count].start_degree >= 170 &&
					bdata->obj[bdata->obj_count].end_degree == -1)
	{
		bdata->obj[bdata->obj_count].end_degree = 180;
		bdata->obj_count++;
	}
}

void sendscandata(botdata_t *bdata){
// start of json object
	int i, j, temp;
	char send_string[800];
	sprintf(send_string, "{\"ir\": [");
		j = 8;
		for(i = 0; i < 180; i++)
		{
			temp = bdata->ir_measure[i]/10;
			if(temp == 0){
				send_string[j] = '0';
				j++;
				send_string[j] = '0';
				j++;
				send_string[j] = (char)(bdata->ir_measure[i] + '0');
				j++;

			} else if(temp > 0 && temp < 10) {
				send_string[j] = '0';
				j++;
				send_string[j] = (char)(temp + '0');
				j++;
				send_string[j] = (char)(bdata->ir_measure[i]%10 + '0');
				j++;

			} else {
				send_string[j] = (char)(bdata->ir_measure[i]/100 + '0');
				j++;
				temp = bdata->ir_measure[i]%100;
				send_string[j] = (char)(temp/10 + '0');
				j++;
				send_string[j] = (char)(temp%10 + '0');
				j++;
			}

			if(i == 179) {
				send_string[j] = ']';
				j++;
				send_string[j] = '}';
				j++;
				send_string[j] = '\n';
				j++;
				send_string[j] = '\0';
			} else {
				send_string[j] = ',';
			}

			j++;


		}

		uart_sendStr(send_string);

}



void find_width(botdata_t *bdata, state_t volatile *state,volatile unsigned int *initial_value,
		volatile unsigned int *second_value) 	//finds widht of object
					//generates json object
{
	int i = 0, j = 0, temp = 0;
	char send_string[800];
	//send object array in json form
	sprintf(send_string, "{\"objects\": {");
	j = 13;
	for(i = 0; i < bdata->obj_count; i++)
	{
		*state = LOW;

		int degree = (bdata->obj[i].end_degree - bdata->obj[i].start_degree);
		degree = degree + bdata->obj[i].start_degree;
		move_servo(degree);
		timer_waitMillis(500);
		send_pulse();
		degree = degree - bdata->obj[i].start_degree;
		while(*state != READY){}


		bdata->obj[i].distance = time2distance(initial_value, second_value, state);
		if(bdata->obj[i].distance < 80 && bdata->obj[i].distance > 10)
		{
//			obj[i].width = sqrt(((ir_measure[(obj[i].start_degree)])*(ir_measure[(obj[i].start_degree)]))+(ir_measure[(obj[i].end_degree)])*(ir_measure[(obj[i].end_degree)]) - 2*(ir_measure[(obj[i].start_degree)])*(ir_measure[(obj[i].end_degree)])*cos((degree)));
			bdata->obj[i].width = (2*bdata->obj[i].distance*tan((((degree/2)*PI)/180)));	//*PI/180
		//		sprintf(send_string, "Object degree %d\t distance: %d\n");

				send_string[j] = '[';
				j++;

				//put the distance in the array
				send_string[j] = (char)(bdata->obj[i].distance/100 + '0');
				j++;
				temp = bdata->obj[i].distance%100;
				send_string[j] = (char)(temp/10 + '0');
				j++;
				send_string[j] = (char)(temp%10 + '0');
				j++;
				send_string[j] = ',';
				j++;

				//put the angle into an array
				send_string[j] = (char)(((degree/2) + bdata->obj[i].start_degree)/100 + '0');
				j++;
				temp = ((degree/2) + bdata->obj[i].start_degree) %100;
				send_string[j] = (char)(temp/10 + '0');
				j++;
				send_string[j] = (char)(temp%10 + '0');
				j++;
				send_string[j] = ',';
				j++;

				//put the width into an array
				send_string[j] = (char)(bdata->obj[i].width/100 + '0');
				j++;
				temp = bdata->obj[i].width%100;
				send_string[j] = (char)(temp/10 + '0');
				j++;
				send_string[j] = (char)(temp%10 + '0');
				j++;
				send_string[j] = ']';
				j++;
				send_string[j] = ',';
				j++;
				send_string[j] = ' ';
				j++;

//			sprintf(send_string, "Object degree %d distance %d width %d\r\n", degree + obj[i].start_degree, obj[i].distance, obj[i].width);

		} else {
			bdata->skip[i] = 1;
//			sprintf(send_string, "Object %d out of range, ignoring values\r\n", i);
		}
//		uart_sendStr(send_string);
		lcd_printf("%s\n", send_string);
	//		timer_waitMillis(500);
	}

	j-=2;	//decrement j so that you end the json array properly
	send_string[j] = '}';
	j++;
	send_string[j] = '}';
	j++;
	send_string[j] = '\n';
	j++;
	send_string[j] = '\0';

	uart_sendStr(send_string);
}

void move_servo(double degree)
{
//	unsigned pulse_width; // pulse width in cycles
	// calculate pulse width in cycles
	//TIMER0_TAMATCHR_R = period_width - pulse_width; // set pulse width
	// you need to call timer_waitMillis( ) here to enforce a delay for the servo to
	// move to the position

	int pulse_period = 320000;	//clock cycles ~20ms pulse width
	double mid_width = 24000;	//up time of clock cycle
	double max_servo = 34205;	//cybot 7 36977		cybot 2 34205
	double min_servo = 6829;	//cybot 7 8696		cybot 2 6829

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

unsigned time2distance(unsigned int *initial_value, unsigned int *second_value,
						state_t *state ){
	int overflow = 0, value=0, final_value=0, distance=0;

	if(*second_value < *initial_value)
	{
		overflow = 1;
		value = 16000000;
		final_value = (value - *initial_value) + *second_value;
	} else {
		final_value = *second_value - *initial_value;
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

	*initial_value = 0;
	*second_value = 0;
	*state = DONE;
	return distance;

//	return 0;
}

