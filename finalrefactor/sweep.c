/**
 * @file sweep.c
 * @brief this file sweeps the servo and sends back iR and Ping data back in an array
 * @author  borseth, sjwarren
 * @date 11/20/2016
 */
 
#include "sweep.h"
#include "botdata.h"
#include "timer.h"
#include "math.h"
#include "lcd.h"
#include "uart.h"
#include "open_interface.h"
#include <stdio.h>
#include <string.h>

///Sweeps the field of view for objects
/**
*This is how we sweep the motor that will turn the IR sensor
*this function will measure how far away 'objects' are but does not define
*what an object is or what is not an object
*@param bdata, botdata_t * holds the raw data from the IR sweep
*/
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
		move_servo(i);
		timer_waitMillis(10);

	}
}

///Analyze data from sweep, and tells array what counts an object and does not
/**
*Sweeps through the entire sweep function checking for objects.
*objects are stored as -1, otherwise the non-objects are 1's
*Calculates the width of the object
*@param bdata uses sweep data and updates the object count
*/
void analyze(botdata_t *bdata) // checks for objects
{
	int i = 0;
	bdata->obj_count = 0;
	for(i = 1; i < 179; i++)
	{
		if((bdata->ir_measure[i-1] == -1 && bdata->ir_measure[i+1] == -1) && bdata->ir_measure[i] >=0)
		{
			bdata->ir_measure[i] = -1;

			//obj_count++;
		} else if(bdata->ir_measure[i-1] >= 0 && bdata->ir_measure[i+1] >= 0 && bdata->ir_measure[i] == -1) {
			bdata->ir_measure[i] = 1;

		} else if((bdata->ir_measure[i-1] == -1 && bdata->ir_measure[i] >= 0)){
			bdata->obj[bdata->obj_count].number = bdata->obj_count;
			bdata->obj[bdata->obj_count].start_degree = i;


		} else if((bdata->ir_measure[i-1] >= 0 && bdata->ir_measure[i] == -1)) {
			bdata->obj[bdata->obj_count].end_degree = (i-1);
			//calculate width here
			bdata->obj_count++;
		} else if(i == 178 && bdata->obj[bdata->obj_count].start_degree >= 0 && bdata->obj[bdata->obj_count].end_degree == -1) {
			bdata->obj[bdata->obj_count].end_degree = 180;
			bdata->obj_count++;
		}
	}

	if(bdata->obj[bdata->obj_count].start_degree >= 170 && bdata->obj[bdata->obj_count].end_degree == -1)
	{
		bdata->obj[bdata->obj_count].end_degree = 180;
		bdata->obj_count++;
	}

	if(bdata->obj[bdata->obj_count].start_degree >= 170 &&
					bdata->obj[bdata->obj_count].end_degree == -1)
	{
		bdata->obj[bdata->obj_count].end_degree = 180;
		bdata->obj_count++;
	}
}

///Sends the analyzed data to the json array and sends it to the GUI
/**
* Uses send_string to store the string that will be sent
* middle of code prepares the values that were in the analyzed code into send_string to be sent
* end part completes the formatting necessary to semd out send_string to the GUI
* @param bdata uses the (analyzed)ir_measure data to give correct values to send_string
*/
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
		uart_flush();


}

///Finds the width, angle and distance of the physical object and sends it out in a json array to the GUI
/**
*Similiar to sendscandata(), this function will send the data to the GUI in a json array
*For each object it will send out a sonar signal-send_pulse() to the object to determine the distance of robot to object 
*Puts the distance, angle and width into a json to be sent to the GUI. Prepares end of json. Sends json to GUI
*@param bdata uses the stored data to help with distance, width, and angle calculations for the json
*@param state when the state is not equal to READY, the program will then continue on. Acts as a pause
*@param initial_value used in time2distance() to store the initial value of the ping sensor
*/
void find_width(botdata_t *bdata, state_t volatile *state,volatile unsigned int *initial_value,
		volatile unsigned int *second_value) 	//finds widht of object
					//generates json object
{
	int i = 0, j = 0, temp = 0;
	char send_string[800] = "Degrees\t\tIR Distance (cm)\tSonar Distance (cm)\r\n";
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


		bdata->obj[i].distance = time2distance((unsigned int *)initial_value, (unsigned int *)second_value,(state_t *)state);
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
//		lcd_printf("%s\n", send_string);
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

	uart_sendStr(send_string); //Moved lines below here from object send
	uart_flush();
/*
	sprintf(send_string, "number of objects found %d", bdata->obj_count); //TODO
	uart_sendStr(send_string);
	*/
}

///Moves the servo a certain number of degrees. 180 will be left, 0 will be right, 90 is center
/**
*Will make the servo move a certain number of degrees depending on the degree sent to the function
*This is primarily done with mid_width. subtracting pulseperiod from the mid_width will have the servo move a certain amount of degrees
*@param degrees Will take in a degree that the user wants the servo to turn. 
*/
void move_servo(double degree)
{

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

/// Calculates the distance from Robot to Object
/**
*Uses the speed of sound to calculate the ping distance from the ping sensor/robot to object and back to ping sensor/robot
*Saves this in a json that could be sent to the GUI
*
*@param initial_value holds the ping sensor's first value
*@param second_value holds the ping sensor's second value for calculation of distance 
*@param state lets the rest of the program know when it has completed this task
*/
unsigned time2distance(unsigned int *initial_value, unsigned int *second_value, state_t *state ){
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

