/**
 *	@file movement.c
 *	@brief contains the functions for movement of the bot
 *  @Author borseth
 *  @date Sep 9, 2016
 *
 */
#include <stdlib.h>
#include "movement.h"
#include "math.h"
#include "floorsensor.h"
#include "uart.h"
#include "main.h"
#include "botlogic.h"

///initilizes a botpos_t
/**
 * initilizes the botpos_t to be the starting position
 * @param botpos_t* the botpos_t to be initilized
 */

void botpos_init(botpos_t *b){
	b->angle = 0;
	b->forward = 0;
	b->right = 0;
}

///decides which way the bot needs to move based off input
/**
 * takes some parsed input from the user and decides to move forward, backwards, right, or left
 * @param botpos_t* the botpos_t to update current position
 * @param botdata_t* the botdata_t to update with new data
 * @param int left the left speed (0 stay 1 forward -1 backwards)
 * @param int right the right speed (0 stay 1 forward -1 backwards)
 */

rtvalue_t interpret_movement(botpos_t *b, botdata_t *bdata, int left, int right){
	rtvalue_t rval;
	rval = collision_detection(b);
	/*
	if(rval != finish){
		b->bump = rval;
		oi_setWheels(-100,-100);
		timer_waitMillis(500);
		oi_setWheels(0,0);
	}
	*/
	if(left < 0){
		if(right < 0){
			forward(b,bdata,-1);
		}
		else if(rval != finish && rval != extraction) return rval;// only allows reverse when sensors triggerd
		else{
			turn(b,1);
		}
	}
	else if(rval != finish && rval != extraction)return rval; //only allows reverse when sensors triggerd
	else if(left > 0){
		if(right > 0){
			forward(b,bdata,1);
		}
		else{
			turn(b,-1);
		}
	}
	// we know left must be 0
	else if(right < 0){
		turn(b,1);
	}
	else if(right > 0){
		turn(b,-1);
	}
	return finish;
}

///commands the bot to move forward if dir != 0 and none of the sensors are triggerd
/**
 * @param botpos_t* the position to update the current position of the bot
 * @param botdata_t* the bot data to update sensor readings
 * @param int dir the direction the bot should move (1 forward -1 backward)
 */
rtvalue_t forward(botpos_t *b,botdata_t *bot, int dir){// moves the bot forward to "distance" mm untill it reaches its destination or bumps into something
	int i, tempangle;

	if(dir == 0){
		oi_setWheels(0,0);
		return finish;
	}
	else if(dir < 0){
		oi_setWheels(0,0);
		oi_setWheels(-100,-100);
		timer_waitMillis(300);
	}
	else{
		oi_setWheels(150,150);
	}

	/*
	if(b->edges[0] == 1){
	return leftBump;
	}
	*/
	//a^2 + b^2 = distance;
	if(b->angle < 360){
		tempangle = b->angle +360;
	}
	else{
		tempangle = b->angle;
	}
	for(i = 0; i <40; i++){
		if(collision_detection(b) != finish){
			oi_setWheels(-100,-100);
			timer_waitMillis(100);
			break;
		}
		if(tempangle > 270){
			b->right += (cos(tempangle) * b->sensor_data->distance);
			b->forward -= (sin(tempangle) * b->sensor_data->distance);
		}
		else if(tempangle > 180){
			b->right += (sin(tempangle) * b->sensor_data->distance);
			b->forward -= (cos(tempangle) * b->sensor_data->distance);
		}
		else if(tempangle > 90){
			b->right += (cos(tempangle) * b->sensor_data->distance);
			b->forward -= (sin(tempangle) * b->sensor_data->distance);
		}
		else{
			b->right += (sin(tempangle) * b->sensor_data->distance);
			b->forward -= (cos(tempangle) * b->sensor_data->distance);
		}
		timer_waitMillis(5);
	}
	oi_setWheels(0,0);
	return finish;
}

///checks for a collision
/**
 * checks all floor and bump sensors for a collision
 * @param botpos_t* the position of the bot to use the open interface object to get sensor readings
 */

rtvalue_t collision_detection(botpos_t *b){
    oi_update(b->sensor_data);
//    updateedge(b);
	if(b->sensor_data->bumpLeft){
		oi_setWheels(0,0);
		b->bump = 1;
		if(b->sensor_data->bumpRight){
			return bothBump;
		}
			return leftBump;
	}
	if(b->sensor_data->bumpRight){
		oi_setWheels(0,0);
		b->bump = 1;
		return rightBump;
	}
	if((cliffleftsurface(b) > 1       ||
		 cliffleftfrontsurface(b) > 1  ||
		 cliffrightfrontsurface(b) > 1 ||
		 cliffrightsurface(b) > 1)){
		oi_setWheels(0,0);
		b->bump = 1;
		return irfault;
	}
	if((cliffleftsurface(b) > 1       ||
		 cliffleftfrontsurface(b) > 1  ||
		 cliffrightfrontsurface(b) > 1 ||
		 cliffrightsurface(b) > 1)){
		oi_setWheels(0,0);
		b->bump = 1;
		return extraction;
	}
	b->bump = 0;
	return finish;
}

///tells the bot to turn a specifide degrees left or right
/**
 * based off input instructs the bot to turn a specified degrees from the starting direction
 * @param botpos_t* the bot position to update the angle of the bot
 * @param int direction the number of degrees to turn (negative turns right, positive turns left)
 */

int turn(botpos_t *b, int direction){ // direction determens the direction to turn (right is negitive, left is positive) the value is the number of degrees to turn
	int data = 0;
	if (direction == 0){
		return 1;
	}
	else if (direction < 0){
		oi_setWheels(150, -150);
	}
	else{
		oi_setWheels(-150, 150);
	}
	while(data < abs(direction) ){
		oi_update(b->sensor_data);
		data +=  abs(b->sensor_data->angle);
		b->angle -= b->sensor_data->angle;

		if(b->angle >= 360 || b->angle <= -360){
			b->angle = 0;
		}

	}
	oi_setWheels(0,0);
	return 0;
}

///waits for input from the user and executes commands
/**
 * waits for input and calls functions to perform actions based on the commands
 * @param botdata_t* the botdata to update the current command
 * @param botpos_t* the botpos_t to update the current position
 */

void recieve_command(botdata_t *bdata, botpos_t *bot){\
	rtvalue_t rval = finish;
	//weird registers that wont read until data arrives...
	//if((UART1_FR_R & UART_FR_RXFE)) return;

	parse_input(bdata);


	if(bdata->commands & 0x02){ 		//left speed
		rval = interpret_movement(bot,bdata,bdata->commands & 0x02,bdata->commands & 0x10);
		send_position(bot);
	}
	else if(bdata->commands & 0x04){ 		//left reverse
		rval = interpret_movement(bot,bdata,(bdata->commands & 0x04) * -1,(bdata->commands & 0x20) * -1);
		send_position(bot);
	}
	else if(bdata->commands & 0x10){ 		//right speed
		rval = interpret_movement(bot,bdata,bdata->commands & 0x02,bdata->commands & 0x10);
		send_position(bot);
		//call function/s
	}
	else if(bdata->commands & 0x20){ 		//right reverse
		rval = interpret_movement(bot,bdata,(bdata->commands & 0x04) * -1,(bdata->commands & 0x20)* -1);
		send_position(bot);
		//call function/s
	}
	else if(bdata->commands & 0x200){ 		//90 degree right turn
		turn(bot,-45);
		send_position(bot);
	}
	else if(bdata->commands & 0x400){ 		//90 degree left turn
		turn(bot,45);
		send_position(bot);
	}
	else{
		oi_setWheels(0,0);
	}

	if(bdata->commands & 0x40){ 		//start scan
		object_send(bdata,bot);
	}

	if(bdata->commands & 0x80){ 		//free collision alert
		//call function/s
	}

	if(bdata->commands & 0x100){ 		//stop
		play_song(bot);
	}

	if(bot->bump){
		timer_waitMillis(5000);
		bot->bump = 0;
	}
}

///recives input from WIFI
/**
 * waits for input from WIFI and places it into an array to be turned into a bitfield
 * @param botdata_t* the botdata to update current command
 */

int parse_input(botdata_t *bdata){
	char in[10];
	int i = 0;
	char temp = 'a';
	while(temp != '\0') {
		in[i] = temp = uart_receive();
		i++;
	}

	if(i == 10) return 1;
	bdata->commands = atoi(in);
	return 0;
}
