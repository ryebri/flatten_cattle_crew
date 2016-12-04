/*
 * floorsensor.c
 *
 *  Created on: Nov 29, 2016
 *      Author: borseth
 */

// posibly make enum for return value
//takes botlogic struct


	/*
	 * white over 2600
	 * floor under 2600 above 50
	 * under 50 is open space
	 */

#include "movement.h"

void updateedge(botpos_t *b){
	b->edges[0] = b->sensor_data->cliffLeft;
	b->edges[1] = b->sensor_data->cliffFrontLeft;
	b->edges[2] = b->sensor_data->cliffFrontRight;
	b->edges[3] = b->sensor_data->cliffRight;
}

int cliffleftsurface(botpos_t *b){
	int data = b->sensor_data->cliffLeftSignal;
	if(data > 2600){ //white floor (boundary)
		return 0;
	}
	else if(data <= 2600 && data > 50){ //under normal floor
		return 1;
	}
	else if(data <=50){ //open floor
		return 2;
	}
	else {return 3;} //error flag
}

int cliffleftfrontsurface(botpos_t *b){
	int data = b->sensor_data->cliffFrontLeftSignal;
	if(data > 2600){
		return 0;
	}
	else if(data <= 2600 && data > 50){
		return 1;
	}
	else if(data <=50){
		return 2;
	}
	else {return 3;}
}

int cliffrightsurface(botpos_t *b){
	int data = b->sensor_data->cliffRightSignal;
	if(data > 2600){
		return 0;
	}
	else if(data <= 2600 && data > 50){
		return 1;
	}
	else if(data <=50){
		return 2;
	}
	else {return 3;}
}

int cliffrightfrontsurface(botpos_t *b){
	int data = b->sensor_data->cliffFrontRightSignal;
	if(data > 2600){
		return 0;
	}
	else if(data <= 2600 && data > 50){
		return 1;
	}
	else if(data <=50){
		return 2;
	}
	else {return 3;}

}
