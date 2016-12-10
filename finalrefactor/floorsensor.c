/**
 *@file floorsensor.c
 *@brief This header file holds functions for each roomba IR (cliff) sensor
 *@author borseth & sjwarren
 *@date 12/8/2016
 */

	/*
	 * white over 2600
	 * floor under 2600 above 1600 OR less than equal to 1600 and greater than 50
	 * ON black if under 2100 and over 1600
	 * under 50 is open space
	 */

#include "movement.h"

///Returns flag of the ground IR sensors on the roomba
/**
*returns a value based on the value read by the bot of sensor_data of the cliffLeftSignal
*This value determines if the bot is under normal floor, extraction point, white tape, hole, or there was an error
*@param b is used to update the sensors for the cliffLeftSignal
*/
int cliffleftsurface(botpos_t *b){
	int data = b->sensor_data->cliffLeftSignal;
	if(data > 2650){ //white floor (boundary)
		return 2;
	}
	else if((data <= 2650 && data > 1900) || (data <=1600 && data > 50)){ //under normal floor
		return 0;
	}
	else if(data <=50){ //open floor/hole
		return 3;
	}
	else if(data <= 1900 && data > 800){ //Under Extraction Point (BLACK)
		return 1;
	}
	else {return 4;} //error flag
}

///Returns flag of the ground IR sensors on the roomba
/**
*returns a value based on the value read by the bot of sensor_data of the cliffFrontLeftSignal
*This value determines if the bot is under normal floor, extraction point, white tape, hole, or there was an error
*@param b is used to update the sensors for the cliffFrontLeftSignal
*/
int cliffleftfrontsurface(botpos_t *b){
	int data = b->sensor_data->cliffFrontLeftSignal;
	if(data > 2600){ //white floor (boundary)
		return 2;
	}
	else if((data <= 2600 && data > 2000) || (data <=1600 && data > 50)){ //under normal floor
		return 0;
	}
	else if(data <=50){ //open floor/hole
		return 3;
	}
	else if(data <= 2000 && data > 1600){ //Under Extraction Point (BLACK)
		return 1;
	}
	else {return 4;} //error flag
}

///Returns flag of the ground IR sensors on the roomba
/**
*returns a value based on the value read by the bot of sensor_data of the cliffRightSignal
*This value determines if the bot is under normal floor, extraction point, white tape, hole, or there was an error
*@param b is used to update the sensors for the cliffRightSignal
*/
int cliffrightsurface(botpos_t *b){
	int data = b->sensor_data->cliffRightSignal;
	if(data > 2600){ //white floor (boundary)
		return 2;
	}
	else if((data <= 2600 && data > 2000) || (data <=1600 && data > 50)){ //under normal floor
		return 0;
	}
	else if(data <=50){ //open floor/hole
		return 3;
	}
	else if(data <= 2000 && data > 1600){ //Under Extraction Point (BLACK)
		return 1;
	}
	else {return 4;} //error flag
}

///Returns flag of the ground IR sensors on the roomba
/**
*returns a value based on the value read by the bot of sensor_data of the cliffFrontRightSignal
*This value determines if the bot is under normal floor, extraction point, white tape, hole, or there was an error
*@param b is used to update the sensors for the cliffFrontRightSignal
*/
int cliffrightfrontsurface(botpos_t *b){
	int data = b->sensor_data->cliffFrontRightSignal;
	if(data > 2600){ //white floor (boundary)
		return 2;
	}
	else if((data <= 2600 && data > 2000) || (data <=1600 && data > 50)){ //under normal floor
		return 0;
	}
	else if(data <=50){ //open floor/hole
		return 3;
	}
	else if(data <= 20100 && data > 1600){ //Under Extraction Point (BLACK)
		return 1;
	}
	else {return 4;} //error flag
}

