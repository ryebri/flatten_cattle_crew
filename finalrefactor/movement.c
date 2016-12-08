/*
 * movement.c
 *
 *  Created on: Sep 9, 2016
 *      Author: borseth
 */
#include <stdlib.h>
#include "movement.h"
#include "lcd.h"
#include "math.h"
#include "floorsensor.h"
#include "uart.h"

///Initializes the angle, foward and right values of the roomba
/**
*sets the angle, foward and right to 0. This should probably only be called 1x at the beginning of the program. 
*@param  b holds the andle, foward, and right values in a pointer to be used elsewhere. 
*/
void botpos_init(botpos_t *b){
	b->angle = 0;
	b->forward = 0;
	b->right = 0;
}

///Tells the bot to move left or right
/**
*if left or right are 1, it will move foward based off of the value of left or right.
*otherwise it turns the bot
*will only allow reverse if the sensor is triggered
*@param b holds the position of the bot to be used as a pointer
*@param bdata holds the data of the robot
*/
rtvalue_t interpret_movement(botpos_t *b, botdata_t *bdata, int left, int right){
	rtvalue_t rval;
	rval = collision_detection(b);
	if(left < 0){
		if(right < 0){
			forward(b,bdata,1);
		}
		else if(rval != finish) return rval;// only allows reverse if sensors triggerd
		else{
			turn(b,1);
		}
	}
	else if(rval != finish)return rval; //only allows reverse when sensors triggerd
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

///Moves the bot forward to "distance" mm untill it reaches its destination or bumps into something
/**
*moves the robot fowards and updates the sensor data to see if it has hit anything on its bumpers
*Then checks to see the ground ir sensors to see if there are cliffs or tape.
*will help with turning using b->angle and the trig below
*@param b keeps tabs on the position of the robot
*@param bot stores the data of the bot, distance in this case
*@param dir stores the direction, if less than 0 it will stop and reverse. 
*/
rtvalue_t forward(botpos_t *b,botdata_t *bot, int dir){

	if(dir == 0){
		return finish;
	}
	else if(dir < 0){
		oi_setWheels(0,0);
		oi_setWheels(-100,-100);
		timer_waitMillis(300);
	}
	else{
		oi_setWheels(200,200);
	}

	/*
	if(b->edges[0] == 1){
	return leftBump;
	}
	*/
	//a^2 + b^2 = distance;
	if(b->angle > 360){
	}
	if(b->angle > 270){
		b->right += (cos(b->angle) * b->sensor_data->distance);
		b->forward -= (cos(b->angle) * b->sensor_data->distance);
	}
	else if(b->angle > 180){
		b->right -= (cos(b->angle) * b->sensor_data->distance);
		b->forward -= (cos(b->angle) * b->sensor_data->distance);
	}
	else if(b->angle > 90){
		b->right -= (cos(b->angle) * b->sensor_data->distance);
		b->forward += (cos(b->angle) * b->sensor_data->distance);
	}
	else{
		b->right += (cos(b->angle) * b->sensor_data->distance);
		b->forward += (cos(b->angle) * b->sensor_data->distance);
	}

	return finish;
}

///If there is a collision it stops
/**
*checks if there is a left bump, right bump or both. Then stops.
*If there are any cliffs it reurns it stops the roomba
*Depending on the type of collision it will return a different value. 
*@param b accesses sensor data on the roomba such as bump sensors or cliffs
*/
rtvalue_t collision_detection(botpos_t *b){
    oi_update(b->sensor_data);
//    updateedge(b);
	if(b->sensor_data->bumpLeft){
		oi_setWheels(0,0);
		if(b->sensor_data->bumpRight){
			return bothBump;
		}
			return leftBump;
	}
	if(b->sensor_data->bumpRight){
		oi_setWheels(0,0);
		return rightBump;
	}
	if((cliffleftsurface(b) > 0       ||
		 cliffleftfrontsurface(b) > 0  ||
		 cliffrightfrontsurface(b) > 0 ||
		 cliffrightsurface(b) > 0)){
		oi_setWheels(0,0);
		return irfault;
	}

	return finish;
}

///Direction determines the direction to turn (right is negitive, left is positive) the value is the number of degrees to turn
/**
*updates sensor data as turning the correct amount of degrees and updates the b->angle for future use
*@param b used for updating sensor data and access the angle of the roomba
*@param direction to turn the roomba, right is negative, left is positive
*/
int turn(botpos_t *b, int direction){
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
		b->angle += b->sensor_data->angle;
		if(b->angle >= 360 || b->angle <= -360){
			b->angle = 0;
		}
	}
	oi_setWheels(0,0);
	return 0;
}

///Recieves a command from GUI
/**
*Depending on the command within bdata->commands, it will either call the part of code to run the action or ignore the if statement.
*the command is masked so as to see which bit field is registered as a 'high' from the GUI. This is what we do with the recieve code from the GUI
*@param bdata holds the command. Easy to mask to see which bitfield is 'active'
*@param bot will be used to help with knowing the position of the bot in the field
*/
void recieve_command(botdata_t *bdata, botpos_t *bot){
int i = 0;
char commands[2];
	//weird registers that wont read until data arrives...
	if((UART1_FR_R & UART_FR_RXFE)) return;

	for( i = 0; i < 2; i++){
		commands[i] = (char)uart_receive();
	}
	bdata->commands = (int)commands;


	if(bdata->commands & 0x02){ 		//left speed
		interpret_movement(bot,bdata,bdata->commands & 0x02,bdata->commands & 0x10);
	}
	else if(bdata->commands & 0x04){ 		//left reverse
		interpret_movement(bot,bdata,(bdata->commands & 0x04) * -1,(bdata->commands & 0x20));
	}
	if(bdata->commands & 0x10){ 		//right speed
		interpret_movement(bot,bdata,bdata->commands & 0x02,bdata->commands & 0x10);
		//call function/s
	}
	if(bdata->commands & 0x20){ 		//right reverse
		interpret_movement(bot,bdata,(bdata->commands & 0x04) * -1,(bdata->commands & 0x20));
		//call function/s
	}

	if(bdata->commands & 0x40){ 		//start scan
		//call function/s
	}
	else{
		// other functions
	}

	if(bdata->commands & 0x80){ 		//free collision alert
		//call function/s
	}
	else{
		// other functions
	}

	if(bdata->commands & 0x200){ 		//90 degree left turn
		//call function/s
	}
	else{
		// other functions
	}

	if(bdata->commands & 0x400){ 		//90 degree right turn
		turn(bot,90);
	}
	else{
		// other functions
	}

	if(bdata->commands & 0x800){ 		//stop
		turn(bot,-90);
	}
	else{
		// other functions
	}

}
