/*
 * botlogic.h
 *
 *  Created on: Sep 9, 2016
 *      Author: borseth
 */

#ifndef BOTLOGIC_H_
#define BOTLOGIC_H_
#include "movement.h"

int getTo(int targetForward, int targetRight, oi_t *sensor); // attempts to get to the specified forward and right position
int avoid(botpos_t *b, int dir); // avoids in the dir direction (-1 for right 1 for left




#endif /* BOTLOGIC_H_ */
