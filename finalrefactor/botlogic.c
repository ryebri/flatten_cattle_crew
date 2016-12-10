/**
 *@file botlogic.c
 *@brief This header file holds functions to move the robot and change its position. Sends information to GUI on roomba sensors. 
 *@author borseth & sjwarren
 *@date 12/8/2016
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
int getTo(int targetForward, int targetRight, oi_t *sensor){ // attempts to get to the specified forward and right position
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

///Avoids in the dir direction (-1 for right 1 for left)
/**
*Turns the bot 90 degrees
*Moves it foward a distance and then turns it to the original position
*@param b is the bot position
*@param dir is the (direction * 90 degrees) that the robot should turn
*/
int avoid(botpos_t *b,int dir){ // avoids in the dir direction (-1 for right 1 for left
	//forward(b,-150);
	turn(b, 90 * dir);
	//forward(b,250);
	turn(b, -90 * dir);
	return 0;
}

///Sends a json to the GUI updating the current cliff sensor information
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

///Plays the "Final Countdown"
/**
*Saves the notes and duration of each note into an array
*Plays the music from the robot until the song is completed. 
*@param bpos used to update the sensor of the bot to see if the song has completed.
*/
void play_song(botpos_t *bpos){
	int song_n = 0;
	int num_notes = 64;
	unsigned char notes [64] =    {88, 86, 88, 81,  0, 89, 88, 89, 88, 86,  0, 89, 88, 89, 81,  0, 86, 84, 86, 84, 83, 86, 84, 88, 86, 88, 81,  0, 89, 88, 89, 88, 86,  0, 89, 88, 89, 81,  0, 86, 84, 86, 84, 83, 86, 84, 83, 84, 86, 83, 84, 88, 86, 84, 83, 81, 89, 88, 88, 89, 88, 86, 100};
	unsigned char duration [64] = {8,  8,  32, 32, 48,  8,  8, 16, 16, 32, 48,  8,  8, 32, 32, 48,  8,  8, 16, 16, 16, 16, 48,  8,  8, 32, 32, 48,  8,  8, 16, 16, 32, 48,  8,  8, 16, 32, 48,  8,  8, 16, 16, 16, 16, 48,  8, 8, 48,  8,  8, 16, 16, 16, 16, 32, 32, 96,  8,  8,  8,  8, 128};
	oi_loadSong(song_n, num_notes, notes, duration);
	oi_play_song(song_n);

	while(1){
		oi_update(bpos->sensor_data);
		if(bpos->sensor_data->songPlaying == 0){ break;}
	}
}
