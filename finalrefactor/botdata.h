/*
 * botdata.h
 *
 *  Created on: Dec 3, 2016
 *      Author: borseth
 */

#ifndef BOTDATA_H_
#define BOTDATA_H_

#include <stdlib.h>
///an enume to denote the a state
typedef enum state {LOW, HIGH, DONE, READY} state_t;

///containse the data of an object
/**
 * contains the start and end degree, width, distance, and number of an object
 */
typedef struct{
	int start_degree;
	int end_degree;
	int width;
	int distance;
	int number;
}Objects;

///contains all the data that the bot has collected
/**
 * contains:
 * ir_measure the data from a scan
 * ir_measure_raw the raw data from a scan
 * obj_count the number of objects detected
 * obj an array of objects with the data of each object
 * skip object that need to be skipped
 * edges data from the floor ir sensors
 * commands the current command to be executed
 */
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
