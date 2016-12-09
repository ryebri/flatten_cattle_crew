/*
 * main.h
 *
 *  Created on: Dec 8, 2016
 *      Author: borseth
 */

#ifndef MAIN_H_
#define MAIN_H_


void object_send(botdata_t *bdata, botpos_t *bpos); // analizes sweep data and sends json object
void send_position(botpos_t *bpos);



#endif /* MAIN_H_ */
