/*
 * botdata.h
 *
 *  Created on: Dec 3, 2016
 *      Author: borseth
 */

#ifndef BOTDATA_H_
#define BOTDATA_H_

#include <stdlib.h>

typedef enum state {LOW, HIGH, DONE, READY} state_t;

typedef struct{
	int start_degree;
	int end_degree;
	int width;
	int distance;
	int number;
}Objects;


typedef struct botdata{
	int ir_measure[180];
	int ir_measure_raw[180];
	int obj_count;
	Objects obj[10];
	int skip[10];//skips objects (find width)
	int edges[4];
	int commands;
}botdata_t;

void botdata_init(botdata_t *bdata);
int getADC();
double get_distance();
void send_pulse();
unsigned time2distance();


#endif /* BOTDATA_H_ */
