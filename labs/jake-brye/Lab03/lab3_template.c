/**
 * lab3_template.c
 * 
 * Template file for CprE 288 lab 3
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"

#warning "Possible unimplemented functions"

/// Initialize PORTC for input from the shaft encoder
void shaft_encoder_init(void) {
	SYSCTL_RCGCGPIO_R |= BIT1;  //turn on PORTB system clock 
	GPIO_PORTB_DIR_R &= 0xFC;	//Setting PB0-PB1 to input
								//need pull up?
	GPIO_PORTB_DEN_R |= 0x03;	//Enabling PB0-PB1
}

/// Read the shaft encoder
/**
 * Reads the two switches of the shaft encoder and compares the values
 * to the previous read.  This function should be called very frequently
 * for the best results.
 *
 * @return a value indicating the shaft encoder has moved:
 * 0 = no rotation (switches did not change)
 * 1 = CW rotation
 * -1 = CCW rotation
 */
int8_t read_shaft_encoder(void) {

#warning "Unimplemented function: char read_shaft_encoder(void)"	// delete this line after implementing
	//
	// DELETE ME - How the shaft encoder works
	// ----------------------------------------
	// The shaft encoder (the knob next to the push buttons) is connected to the right
	// two bits of PORTB.  If you spin the knob, you'll notice that it likes to come to rest
	// at specific angles (we'll call them grooves).  There are two mechanical switches (A and B)
	// that toggle based on which way you rotate the knob.  If you start in a groove and start
	// to rotate the knob CW, one of the switches will drop first (say A), then the other will
	// drop after rotating a tiny bit further (say B).  As you approach the next groove, A rises, then B.
	//
	// In other words, PORTB will look like this if rotating clockwise:
	// GPIO_PORTB_DATA_R => 0bxxxx xx11 (in a groove)
	// GPIO_PORTB_DATA_R => 0bxxxx xx01 (rotating out of the groove CW)
	// GPIO_PORTB_DATA_R => 0bxxxx xx00 (in the middle of two grooves)
	// GPIO_PORTB_DATA_R => 0bxxxx xx10 (approaching the next groove)
	// GPIO_PORTB_DATA_R => 0bxxxx xx11 (in the next groove)
	//

	// static variable to store the old value of A and B.
	// This variable will only be initialized the first time you call this function.
//	static unsigned char old_value = REPLACEME;		// Step 2: Based on how you decided to mask PORTB in step 1, what would new_value be if the switch is in a groove?

	// Function variables
//	unsigned char new_value = REPLACEME;			// Step 1: Decide how to read PINC so that the push buttons are masked
//	int8_t rotation = 0;
//
//	// If the knob was in a groove MIGHT BE REVERSED!!!!!!!!!
//	if (old_value == REPLACEME) {			// Step 3: Enter here what you did for step 2.
//		if (new_value == REPLACEME)		// What will new_value be if you're rotating CW out of a groove
//			rotation = 1;
//		if (new_value == REPLACEME)		// What will new_value be if you're rotating CCW out of a groove
//			rotation = -1;
//	}
//
//	old_value = new_value;
//
//	return rotation;
}



/// Initialize PORTB to control the stepper motor
void stepper_init(void) {
	SYSCTL_RCGCGPIO_R |= BIT1;  //turn on PORTB system clock 
	GPIO_PORTB_DIR_R |= 0xF0;  	//Setting PB4-PB7 to output
	GPIO_PORTB_DEN_R |= 0xF0;
	GPIO_PORTB_DATA_R &= 0x8F;  //Initial postion (0b1000) PB4-PB7
	timer_waitMillis(2);
	GPIO_PORTB_DATA_R &= 0x0F;  //Clear PB4-PB7
}

/// Turn the Stepper Motor
/**
 * Turn the stepper motor a given number of steps.
 *
 * @param num_steps A value between 1 and 200 steps (1.8 to 360 degrees)
 * @param direction Indication of direction: 1 for CW and -1 for CCW
 */
void  move_stepper_motor_by_step(int num_steps, int direction) {

//#warning "Unimplemented function: void move_stepper_motor_by_step(int num_steps, int direction)"	// delete this line after implementing
	//
	// DELETE ME
	// ----------
	// The stepper motor operates off PORTB, which also powers the servo for the sonar.  It's
	// important not to confuse the two.  The stepper motor connects to the iRobot, so the iRobot
	// must be powered on.
	//
	// Pins 4 through 7 of PORTB control the stepper motor.  In order to turn the motor one "step",
	// you'll need to send a series of signals to PORTB.  It's important that you only affect the
	// left 4 bits of PORTE and do not alter the right 4 bits when sending output signals to PORTB.
	//
	// To turn clockwise, send the following pattern to the left four bits of PORTB:
	// 0b0001  - starting position
	// 0b0010  - one step CW
	// 0b0100  - two steps CW
	// 0b1000  - three steps CW
	// 0b0001  - four steps CW
	// ...
	// To turn counter clockwise, simply reverse the order.
	//
    // Remember to wait a short amount of time between each step so that the motor has time to turn.

	//INSERT CODE HERE
	
	// Requirements
	// - Use bitwise operators to ensure the right 4 bits of PORTB are not affected
	// - Set the left 4 bits of PORTB to 0's at the end of your function (to allow the stepper motor to spin freely)

}

int main(void) {
	button_init();
	lcd_init();
	stepper_init();
	shaft_encoder_init();
	
	//TODO
	while(1){
		uint8_t button = button_getButton();
		lcd_printf("Button: %d", button);
	}
}
