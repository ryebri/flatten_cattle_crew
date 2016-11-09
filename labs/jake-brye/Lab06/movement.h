/*
 * movement.h
 *
 *  Created on: Sep 6, 2016
 *      Author: jgasmus
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#endif
#ifndef OPEN_INTERFACE_H_
#include "open_interface.h"
#endif
#ifndef LCD_H_
#include "lcd.h"
#endif
	/* MOVEMENT_H_ */

void turn_ccw(oi_t *sensor, int degrees);
void turn_cw(oi_t *sensor, int degrees);
void move_forward_normal(oi_t *sensor, int centimeters);
void move_forward_avoid(oi_t *sensor, int centimeters);
void reverse(oi_t *sensor, int centimeters);
int avoid_obstacle(oi_t *sensor);




