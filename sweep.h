/*
 * sweep.h
 *
 *  Created on: Dec 3, 2016
 *      Author: borseth
 */

#ifndef SWEEP_H_
#define SWEEP_H_

#include "botdata.h"
#define PI 3.14159265


void do_sweep(botdata_t *bdata);
void analyze(botdata_t *bdata); // checks for objects
void sendscandata(botdata_t *bdata);
void find_width(botdata_t *bdata, state_t volatile *state,volatile unsigned int *initial_value,
		volatile unsigned int *second_value); 	//finds widht of object
void move_servo(double degree);
unsigned time2distance(unsigned int *initial_value, unsigned int *second_value,
							state_t *state );

#endif /* SWEEP_H_ */
