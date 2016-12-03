/*
 * floorsensor.c
 *
 *  Created on: Nov 29, 2016
 *      Author: borseth
 */

// posibly make enum for return value
//takes botlogic struct



	oi_t *sensor_data = oi_alloc();
	oi_init(sensor_data);
	lcd_init();

	/*
	 * white over 2600
	 * floor under 2600 above 50
	 * under 50 is open space
	 */
int checkleftfloor(){
	int data = sensor_data->cliffLeftSignal;
	if(data > 2600){ //white floor (boundary)
		return 0;
	}
	else if(data <= 2600 && data > 50){ //under normal floor
		return 1;
	}
	else if(data <=50){ //open floor
		return 2;
	}
	else {return 3;} //error flag
}

int checkleftfrontfloor(){
	int data = sensor_data->cliffFrontLeftSignal;
	if(data > 2600){
		return 0;
	}
	else if(data <= 2600 && data > 50){
		return 1;
	}
	else if(data <=50){
		return 2;
	}
	else {return 3;}
}

int checkrightfloor(){
	int data = sensor_data->cliffRightSignal;
	if(data > 2600){
		return 0;
	}
	else if(data <= 2600 && data > 50){
		return 1;
	}
	else if(data <=50){
		return 2;
	}
	else {return 3;}
}

int checkrightfrontfloor(){
	int data = sensor_data->cliffFrontRightSignal;
	if(data > 2600){
		return 0;
	}
	else if(data <= 2600 && data > 50){
		return 1;
	}
	else if(data <=50){
		return 2;
	}
	else {return 3;}

}
