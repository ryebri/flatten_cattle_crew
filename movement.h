/*
 * movement.h
 *
 *  Created on: Sep 9, 2016
 *      Author: borseth
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include <stdlib.h>
#include "open_interface.h"
#include "botdata.h"

typedef enum rtvalue {leftBump,rightBump,bothBump,irfault,finish} rtvalue_t;


typedef struct botpos{
	int angle; 		// the angle that the bot is currently faceing (0 -360) with 0 = forward 90 = right 180 = backwards 270 = left
	int forward;	// forward is the distance forward (negitive for backwards)
	int right;  	// right is the distance right traveled (negitive for left)
	oi_t *sensor_data;
}botpos_t;

rtvalue_t forward(botpos_t *b, botdata_t *bot, int dir);// moves the bot forward to "distance" mm untill it reaches its destination or bumps into something
int turn(botpos_t *b, int direction); // direction determens the direction to turn (right is negitive, left is positive) the value is the number of degrees to turn
void botpos_init(botpos_t *b);
rtvalue_t interpret_movement(botpos_t *b, botdata_t *bdata, int left, int right);
rtvalue_t collision_detection(botpos_t *b);
void recieve_command(botdata_t *bdata, botpos_t *bot);



#endif /* MOVEMENT_H_ */
