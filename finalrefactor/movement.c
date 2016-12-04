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

void botpos_init(botpos_t *b){
	int i;
	b->angle = 0;
	b->forward = 0;
	b->right = 0;
	/*
	for(i = 0; i < 4; i++){
		b->edges[i] = 0;
	}*/
}


rtvalue_t forward(botpos_t *b, int distance){// moves the bot forward to "distance" mm untill it reaches its destination or bumps into something
	int sum = 0;
	if(distance < 0){
		oi_setWheels(0,0);
		oi_setWheels(-100,-100);
		timer_waitMillis(300);
		distance = distance * -1;
	}
	else{
		oi_setWheels(200,200);
	}

	while (sum < distance) {
	    oi_update(b->sensor_data);
	    //updateedge(b);
		if(b->sensor_data->bumpLeft){
			if(b->sensor_data->bumpRight){
				return bothBump;
			}
			return leftBump;
		}
		if(b->sensor_data->bumpRight){
			return rightBump;
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
		b->right = (cos(b->angle) * b->sensor_data->distance);
		b->forward = (cos(b->angle) * b->sensor_data->distance);
		}
	return finish;
}

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
		b->angle += b->sensor_data->angle;
	}
	return 0;
}



