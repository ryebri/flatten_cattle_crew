/*
 * edge.c
 *
 *  Created on: Nov 15, 2016
 *      Author: borseth
 */

/*
 * cliffLeft
 * cliffFrontLeft
 * cliffFrontRight
 * cliffRight
 *
*/
#include "open_interface.h"
#include "lcd.h"
#include "botlogic.h"
#include "movement.h"
#include "floorsensor.h"
#include "uart.h"



int main(){

	oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
	lcd_init();
    botpos_t bot;
    bot.sensor_data = sensor_data;
	botpos_init(&bot);
	uart_init();
	char send_string[800];
//	oi_free(sensor_data);

    //getTo(2000,0, sensor_data);
   // oi_setWheels(0, 0); // stop


    while(1){
		oi_update(sensor_data);
/*
		lcd_printf("CLS: %d\nCFLS: %d\nCFRS: %d\nCRS: %d", cliffleftsurface(&bot),
						cliffleftfrontsurface(&bot),
						cliffrightfrontsurface(&bot), cliffrightsurface(&bot));
		timer_waitMillis(50);
*/


		lcd_printf("Bumper: %d", bump(&bot));  //bump(&bot); returns which bumper has been hit. left=0, right=1, both=2, none=3
			timer_waitMillis(50);


		sprintf(send_string, "{\"sensors\":[%03d, %03d, %03d, %03d, %03d]}\n\0",cliffleftsurface(&bot), cliffleftfrontsurface(&bot),
				cliffrightfrontsurface(&bot), cliffrightsurface(&bot), bump(&bot) );
		uart_sendStr(send_string);
		if(((cliffleftsurface(&bot) || cliffleftfrontsurface(&bot) || cliffrightfrontsurface(&bot) || cliffrightsurface(&bot)) == (0 || 2 || 4))) {
			//stop wheels!
			oi_setWheels(0,0);

		}
		timer_waitMillis(50);

		//oi_free(sensor_data);
    }
    return 0;

}




