#include "sonar.h"


void sonar_init(){
	//initialize the timer
	//initialize the gpio port
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	GPIO_PORTB_DEN_R |= 0x08;
	GPIO_PORTB_AFSEL_R &= (~0x04);

	//Timer
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3; // Turn on clock to TIMER3

	//Configure the timer for input capture mode
	TIMER3_CTL_R &= ~(TIMER_CTL_TBEN); //disable timerB to allow us to change settings
	TIMER3_CFG_R |= TIMER_CFG_16_BIT; //set to 16 bit timer

	TIMER3_TBMR_R = (TIMER_TBMR_TBCMR | TIMER_TBMR_TBCDIR | TIMER_TBMR_TBMR_CAP); //set for capture mode, up count TIMER_TBMR_TBAMS?

	TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;

//	TIMER3_TBILR_R = (int)(16000000/(50 * 5));
//	TIMER3_TBILR_R = 0xFFFF;


	TIMER3_IMR_R |= (TIMER_IMR_CBEIM);	//sets the interrupt mask

	TIMER3_ICR_R |= (TIMER_ICR_CBECINT); //clears TIMER3 time-out interrupt flags

	//initialize local interrupts
	//NVIC_EN2_R = 0x000000C0; //#warning "enable interrupts for TIMER4A and TIMER4B" n = 0, 1, 2, 3, or 4
	//go to page 105 and find the corresponding interrupt numbers for TIMER4 A&B
	//then set those bits in the correct interrupt set EN register (p. 142)
	NVIC_EN1_R |= 0x00000010;		//36-32=4
//		TIMER3_CTL_R |= (TIMER_CTL_TBEN); //Enable TIMER3B


	IntRegister(INT_TIMER3B, TIMER3B_Handler); //register TIMER4B interrupt handler
	IntMasterEnable(); //intialize global interrupts

	TIMER3_CTL_R |=   (TIMER_CTL_TBEN); //Enable TIMER3B

}

void TIMER3B_Handler(void){
	TIMER3_ICR_R |= TIMER_IMR_CBEIM;

 	if(state == LOW){
		initial_value = TIMER3_TBV_R;
		state = HIGH;
	} else {
		second_value = TIMER3_TBV_R;
		state = READY;			//COMMENT OUT FOR PART 1
	}

}

// Send pulse to port B
void send_pulse()
{

	GPIO_PORTB_PCTL_R &= ~GPIO_PCTL_PB3_T3CCP1;

	GPIO_PORTB_AFSEL_R &= 0xF7;


	//GPIO_PORTB_PCTL_R &= 0xF7;
	GPIO_PORTB_DIR_R |= 0x08; // set PB3 as output
	GPIO_PORTB_DATA_R |= 0x08; // set PB3 to high
	timer_waitMicros(10);// wait at least 5 microseconds based on data sheet
//	GPIO_PORTB_DEN_R &= 0xF7;
	GPIO_PORTB_DATA_R &= 0xF7; // set PB3 to low
	GPIO_PORTB_DIR_R &= 0xF7; // set PB3 as input
	GPIO_PORTB_AFSEL_R |= 0x08;

	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;


//	GPIO_PORTB_DEN_R |= 0x08;
//	GPIO_PORTB_PCTL_R |= 0x00007000;  //activiating Port mux control 3
//	GPIO_PORTB_DEN_R |= 0x08;
//	NVIC_EN1_R = 0x0001000;
}

unsigned time2distance(){
	int overflow = 0, value=0, final_value=0, distance=0;

	if(second_value < initial_value)
	{
		overflow = 1;
		value = 16000000;
		final_value = (value - initial_value) + second_value;
	} else {
		final_value = second_value - initial_value;
	}
//	initial_value = 0;
//	second_value = 0;
//		lcd_clear();
//		lcd_printf("final_value: %hu\n", final_value);

	distance = ((0.002125 * final_value)/2);
//		final_value = final_value * 0.0000625;
	lcd_printf("final_value: %.2f\ndistance: %.2f\noverflow: %d", (final_value * 0.0000625), distance, overflow);
//	final_value = 0;
//	distance = 0;

	initial_value = 0;
	second_value = 0;
	state = DONE;
	return distance;

//	return 0;
}