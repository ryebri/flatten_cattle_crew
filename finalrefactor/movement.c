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
}

void interpret_movement(botpos_t *b,int left, int right){
	if(left < 0){
		if(right < 0){
			forward(b,1);
		}
		else{
			turn(b,1);
		}
	}
	if(left > 0){
		if(right > 0){
			forward(b,1);
		}
		else{
			turn(b,-1);
		}
	}
	// we know left must be 0
	if(right < 0){
		turn(b,1);
	}
	if(right > 0){
		turn(b,-1);
	}
}


rtvalue_t forward(botpos_t *b, int dir){// moves the bot forward to "distance" mm untill it reaches its destination or bumps into something
	if(dir < 0){
		oi_setWheels(0,0);
		oi_setWheels(-100,-100);
		timer_waitMillis(300);
	}
	else{
		oi_setWheels(200,200);
	}

    oi_update(b->sensor_data);
    //updateedge(b);
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
	if(((cliffleftsurface(&bot) > 0       ||
		 cliffleftfrontsurface(&bot) > 0  ||
		 cliffrightfrontsurface(&bot) > 0 ||
		 cliffrightsurface(&bot) > 0)){
		//stop wheels!
		oi_setWheels(0,0);

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
		if(b->angle > 360 || b->angle < -360){
			b->angle = 0;
		}
	}
	return 0;
}



