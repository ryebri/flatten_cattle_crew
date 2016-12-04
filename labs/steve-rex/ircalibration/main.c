/*
 * main.c
 *
 *  Created on: Nov 29, 2016
 *      Author: borseth
 */


#include "open_interface.h"
#include "lcd.h"
#include "WiFi.h"

// testing ir cliff sensors

void main(){
	oi_t *sensor_data = oi_alloc();
	oi_init(sensor_data);
	lcd_init();

	//char *str;
	int crs, cfrs, cfls, cls;
	int i;
	/*
	 * white over 2600
	 * floor under 2600 above 50
	 * under 50 is open space
	 */
	for(i = 0; i<5000; i++){
		timer_waitMillis(200);
		oi_update(sensor_data);
		cls = sensor_data->cliffLeftSignal;
		cfls = sensor_data->cliffFrontLeftSignal;
		cfrs = sensor_data->cliffFrontRightSignal;
		crs = sensor_data->cliffRightSignal;
		//sprintf(str,"crs:  %8d/ncfrs: %8d/ncfls: %8d/ncls: %8d",crs, cfrs, cfls, cls);
		lcd_printf("crs: %8d\ncfrs: %8d\ncfls: %8d\ncls:  %8d",crs, cfrs, cfls, cls);
	}
    oi_free(sensor_data);


}

