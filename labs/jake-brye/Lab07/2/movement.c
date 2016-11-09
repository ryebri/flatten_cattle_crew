#include "movement.h"
/*
 * movement.c
 *
 *  Created on: Sep 6, 2016
 *      Author: jgasmus
 */

void turn_ccw(oi_t *sensor, int degrees){

	    oi_init(sensor);

	    int sum = 0;
	    oi_setWheels(150, -150); // move forward;
	    while (sum <= degrees) {
	        oi_update(sensor);
	        sum += sensor->angle;
	    }

	    oi_setWheels(0, 0); // stop


	return;
}

void turn_cw(oi_t *sensor, int degrees){

	    oi_init(sensor);

	    int sum = 0;
	    oi_setWheels(-150, 150); // move forward
	    while (sum >= -1 * degrees) {
	        oi_update(sensor);
	        sum += sensor->angle;
	    }

	    oi_setWheels(0, 0); // stop


	return;
}

void move_forward_normal(oi_t *sensor, int centimeters){
    oi_init(sensor);

    int sum = 0;
    oi_setWheels(150, 150); // move forward; full speed
    while (sum < centimeters*10) {
        oi_update(sensor);
        sum += sensor->distance;
        //check for left or right bump detection
        sum += avoid_obstacle(sensor);
        oi_setWheels(150, 150);
    }

    oi_setWheels(0, 0); // stop


return;
}

void reverse(oi_t *sensor, int centimeters){
    oi_init(sensor);

    int sum = 0;
    oi_setWheels(-150, -150); // move forward; full speed
    while (sum > -1*centimeters*10) {
        oi_update(sensor);
        sum += sensor->distance;
        //check for left or right bump detection
    }

    oi_setWheels(0, 0); // stop
    return;
}

void move_forward_avoid(oi_t *sensor, int centimeters){
    oi_init(sensor);

    int sum = 0;
    oi_setWheels(150, 150); // move forward; full speed
    while (sum < centimeters*10) {
        oi_update(sensor);
        sum += sensor->distance;
        //check for left or right bump detection
        avoid_obstacle(sensor);
    }

    oi_setWheels(0, 0); // stop


return;
}

int avoid_obstacle(oi_t *sensor){
	int dist_difference = 0;

	if(sensor->bumpLeft && sensor->bumpRight){
		oi_setWheels(0,0);
		reverse(sensor, 15);
		turn_cw(sensor, 90);
		move_forward_avoid(sensor, 25);
		turn_ccw(sensor, 90);

		dist_difference = -150;

	} else if(sensor->bumpLeft){
		oi_setWheels(0,0);
		reverse(sensor, 15);
		turn_cw(sensor, 90);
		move_forward_avoid(sensor, 25);
		turn_ccw(sensor, 90);

		dist_difference = -150;

	} else if(sensor->bumpRight){
		oi_setWheels(0,0);
		reverse(sensor, 15);
		turn_ccw(sensor, 90);
		move_forward_avoid(sensor, 25);
		turn_cw(sensor, 90);

		dist_difference = -150;

	}

	return dist_difference;
}

