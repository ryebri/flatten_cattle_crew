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
#include "uart.h"
#include "floorsensor.h"


int getTo(int targetForward, int targetRight, oi_t *sensor){ // attempts to get to the specified forward and right position
	botpos_t b;
	rtvalue_t r;

	b.angle = 0;
	b.forward = 0;
	b.right = 0;
	b.sensor_data = sensor;
	//botpos_init(&b);

	while(b.forward < targetForward){
		//r = forward(&b, targetForward - b.forward);
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
	//forward(b,-150);
	turn(b, 90 * dir);
	//forward(b,250);
	turn(b, -90 * dir);
	return 0;
}

int send_sensor(botdata_t bot, botpos_t *bpos){
	oi_update(bpos->sensor_data);
	char send_string[800];

/*
	lcd_printf("CLS: %d\nCFLS: %d\nCFRS: %d\nCRS: %d", cliffleftsurface(&bot),
					cliffleftfrontsurface(&bot),
					cliffrightfrontsurface(&bot), cliffrightsurface(&bot));
	timer_waitMillis(50);
*/


	//lcd_printf("Bumper: %d", bump(&bot) );  //bump(&bot); returns which bumper has been hit. left=0, right=1, both=2, none=3
		//timer_waitMillis(50);




	sprintf(send_string, "{\"sensors\": [%03d, %03d, %03d, %03d]}\n\0",cliffleftsurface(bpos), cliffleftfrontsurface(bpos),
		cliffrightfrontsurface(bpos), cliffrightsurface(bpos));

	uart_sendStr(send_string);
	timer_waitMillis(50);

	//oi_free(sensor_data);
	return 0;
}

void play_song(botpos_t *bpos){
	int song_n = 0;
	int num_notes = 64;
	unsigned char notes [64] =    {88, 86, 88, 81,  0, 89, 88, 89, 88, 86,  0, 89, 88, 89, 81,  0, 86, 84, 86, 84, 83, 86, 84, 88, 86, 88, 81,  0, 89, 88, 89, 88, 86,  0, 89, 88, 89, 81,  0, 86, 84, 86, 84, 83, 86, 84, 83, 84, 86, 83, 84, 88, 86, 84, 83, 81, 89, 88, 88, 89, 88, 86, 100};
	unsigned char duration [64] = {8,  8,  32, 32, 48,  8,  8, 16, 16, 32, 48,  8,  8, 32, 32, 48,  8,  8, 16, 16, 16, 16, 48,  8,  8, 32, 32, 48,  8,  8, 16, 16, 32, 48,  8,  8, 16, 32, 48,  8,  8, 16, 16, 16, 16, 48,  8, 8, 48,  8,  8, 16, 16, 16, 16, 32, 32, 96,  8,  8,  8,  8, 128};
	oi_loadSong(song_n, num_notes, notes, duration);
	oi_play_song(song_n);

	while(1){
		oi_update(bpos->sensor_data);
		if(bpos->sensor_data->songPlaying == 0){ break;}
	}
}
