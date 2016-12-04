/*
 * botlogic.c
 *
 *  Created on: Sep 9, 2016
 *      Author: borseth
 */
#include "movement.h"
#include "open_interface.h"
#include "botlogic.h"
#include <stdint.h>


int getTo(int targetForward, int targetRight, oi_t *sensor){ // attempts to get to the specified forward and right position
	botpos_t b;
	rtvalue_t r;

	b.angle = 0;
	b.forward = 0;
	b.right = 0;
	b.sensor_data = sensor;
	//botpos_init(&b);

	while(b.forward < targetForward){
		r = forward(&b, targetForward - b.forward);
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

int avoid(botpos_t *b,int dir){ // avoids in the dir direction (-1 for right 1 for left
	forward(b,-150);
	turn(b, 90 * dir);
	forward(b,250);
	turn(b, -90 * dir);
	return 0;
}
