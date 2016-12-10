/*
 * botlogic.h
 *
 *  Created on: Sep 9, 2016
 *      Author: borseth
 */

#ifndef BOTLOGIC_H_
#define BOTLOGIC_H_
#include "movement.h"
#include "botdata.h"

int getTo(int targetForward, int targetRight, oi_t *sensor); // attempts to get to the specified forward and right position
int avoid(botpos_t *b, int dir); // avoids in the dir direction (-1 for right 1 for left
int send_sensor(botdata_t bot, botpos_t *bpos);
void play_song(botpos_t *bpos);




#endif /* BOTLOGIC_H_ */
