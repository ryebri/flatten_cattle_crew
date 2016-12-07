/*
 * floorsensor.h
 *
 *  Created on: Dec 3, 2016
 *      Author: sjwarren
 */

#ifndef FLOORSENSOR_H_
#define FLOORSENSOR_H_
#include "movement.h"


int cliffleftsurface(botpos_t *b);
int cliffleftfrontsurface(botpos_t *b);
int cliffrightsurface(botpos_t *b);
int cliffrightfrontsurface(botpos_t *b);
//void updateedge(botpos_t *b);



#endif /* FLOORSENSOR_H_ */
