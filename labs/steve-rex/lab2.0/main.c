/*
 * main.c
 */

#include "open_interface.h"
#include "lcd.h"
#include "botlogic.h"
#include "movement.h"

int main(void) {
	oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
	lcd_init();

    lcd_clear();
    lcd_puts("Welcom");

	botpos_t b;

	b.angle = 0;
	b.forward = 0;
	b.right = 0;
	b.sensor_data = sensor_data;

    getTo(2000,0, sensor_data);

    oi_setWheels(0, 0); // stop
    oi_free(sensor_data);

    return 0;
}
