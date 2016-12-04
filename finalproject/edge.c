/*
 * edge.c
 *
 *  Created on: Nov 15, 2016
 *      Author: borseth
 */

/*
 * cliffLeft
 * cliffFrontLeft
 * cliffFrontRight
 * cliffRight
 *
*/
#include "open_interface.h"
#include "lcd.h"
#include "botlogic.h"
#include "movement.h"

void updateedge(botpos_t *b){
	b->edges[0] = b->sensor_data->cliffLeft;
	b->edges[1] = b->sensor_data->cliffFrontLeft;
	b->edges[2] = b->sensor_data->cliffFrontRight;
	b->edges[3] = b->sensor_data->cliffRight;
}

int main(){
	oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);


    getTo(2000,0, sensor_data);


    oi_setWheels(0, 0); // stop
    oi_free(sensor_data);
    return 0;

}




