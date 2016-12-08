/*
 * botlogic.c
 *
 *  Created on: Sep 9, 2016
 *      Author: borseth
 */
#include "movement.h"
#include "open_interface.h"
#include "botlogic.h"
#include <stdint.h>
#include "uart.h"
#include "floorsensor.h"

///Attempts to get to the specified forward and right position
/**
*will try to move foward and change the way it moves depending on which bumper is pressed.
*Will call avoid to move around the object once a bumper is detected. 
*@param targetFoward however far foward the robot is to go
*@param targetRight decides on how far the the right the roomba should move
*@param sensor will be used for the data in the bump sensors for wether or not they have been hit. 
*/
int getTo(int targetForward, int targetRight, oi_t *sensor){
	botpos_t b;
	rtvalue_t r;

	b.angle = 0;
	b.forward = 0;
	b.right = 0;
	b.sensor_data = sensor;
	//botpos_init(&b);

	while(b.forward < targetForward){
		//r = forward(&b, targetForward - b.forward);
		switch(r){
		case bothBump:
		case leftBump:
			avoid(&b,1);
		break;
		case rightBump:
			avoid(&b,-1);
		break;
		case finish:
			return 0;
		}
	}
	return 1;
}

///Avoids in the dir direction (-1 for right 1 for left
/**
*turns the bot 90 degrees
*moves it foward a distance and then turns it to the original position
*@param b is the bot position
*@dir is the dir * 90 degrees that the robot should turn
*/
int avoid(botpos_t *b,int dir){ 
	//forward(b,-150);
	turn(b, 90 * dir);
	//forward(b,250);
	turn(b, -90 * dir);
	return 0;
}

///Sends a json to the GUI updating the cliff sensors
/**
*updates the roomba sensors, saves the cliff sensor data into a json type string
*sends the string to the GUI
*@param bot is used to send data about the cliff sensors (cliff, no cliff, etc) to the cliff functions. 
*@param bpos updates the sensors for the cliff data
*/
int send_sensor(botdata_t bot, botpos_t *bpos){
	oi_update(bpos->sensor_data);
	char send_string[800];

/*
	lcd_printf("CLS: %d\nCFLS: %d\nCFRS: %d\nCRS: %d", cliffleftsurface(&bot),
					cliffleftfrontsurface(&bot),
					cliffrightfrontsurface(&bot), cliffrightsurface(&bot));
	timer_waitMillis(50);
*/


	//lcd_printf("Bumper: %d", bump(&bot) );  //bump(&bot); returns which bumper has been hit. left=0, right=1, both=2, none=3
		//timer_waitMillis(50);




	sprintf(send_string, "{\"sensors\": [%03d, %03d, %03d, %03d]}\n\0",cliffleftsurface(bpos), cliffleftfrontsurface(bpos),
		cliffrightfrontsurface(bpos), cliffrightsurface(bpos));

	uart_sendStr(send_string);
	timer_waitMillis(50);

	//oi_free(sensor_data);
	return 0;
}

