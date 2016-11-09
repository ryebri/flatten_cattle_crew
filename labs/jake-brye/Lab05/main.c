/*
 * main.c
 */
#include "open_interface.h"
#include "timer.h"
#include "lcd.h"
#include "button.h"

void uart_init(void) {     //enable clock to GPIO, R1 = port B
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

		//enable clock to UART1, R1 = UART1. ***Must be done before setting Rx and Tx
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;
		//enable alternate functions on port b pins 0 and 1
	GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1);
		//enable Rx and Tx on port B on pins 0 and 1
	GPIO_PORTB_PCTL_R |= 0x00000011;
		//set pin 0 and 1 to digital
	GPIO_PORTB_DEN_R |= (BIT0 | BIT1);
		//set pin 0 to Rx or input
	GPIO_PORTB_DIR_R &= ~BIT0;
		//set pin 1 to Tx or output
	GPIO_PORTB_DIR_R |= BIT1;
	//calculate baudrate
	uint16_t iBRD = 8;//use equations
	uint16_t fBRD =  44;//use equations
	//turn off uart1 while we set it up
	UART1_CTL_R &= ~(UART_CTL_UARTEN);
	//set baud rate
	UART1_IBRD_R = iBRD;
	UART1_FBRD_R = fBRD;
	//set frame, 8 data bits, 1 stop bit, no parity, no FIFO
	UART1_LCRH_R = UART_LCRH_WLEN_8 ;
	//use system clock as source
	UART1_CC_R = UART_CC_CS_SYSCLK;
	//re-enable enable RX, TX, and uart1
	UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);

}

void uart_sendChar(char data){
	//check to see if room to send
	while((UART1_FR_R & 0x20) != 0);
    //send
	UART1_DR_R = data;
}

//blocking call to receive over uart1/
//returns char with data
char uart_receive(void){
	char data = 0;
	int timer = 0;
    //wait to receive
	while(UART1_FR_R & UART_FR_RXFE)
		{
			timer++;
			if(timer > 150)
			{
				return '\0';
			}
		}
	//mask the 4 error bits and grab only 8 data bits
	data = (char)(UART1_DR_R & 0xFF);
	return data;
}

void uart_sendString(char msg[])
{
	char *c_ptr = msg;
	while(*c_ptr != '\0')
	{
		uart_sendChar(*c_ptr);
		c_ptr++;
	}
}

//Start Part 4
void WiFi_start(){
	char response;

	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	//set pin pb2 direction
	GPIO_PORTB_DIR_R |=0x4;
	GPIO_PORTB_DEN_R |= 0x4;
	GPIO_PORTB_DATA_R |= 0x4;
	GPIO_PORTB_PCTL_R |= 0x4; //Enter command mode
	uart_sendChar(0x00); //Send command
	uart_sendString("PplmcWtft!"); //Send WiFi PSK
	uart_sendChar(0x00); //NULL terminator
	response = uart_receive(); //Wait for response
	GPIO_PORTB_PCTL_R &= 0xFFFFFFFB; //Leave command mode
	if(response != 0) {
	//An error occurred…
	}
}

void WiFi_stop(){
	char response;
	//set pin pb2 direction
	GPIO_PORTB_DIR_R |=0x4;
	GPIO_PORTB_DEN_R |= 0x4;
	GPIO_PORTB_DATA_R |= 0x4;
	GPIO_PORTB_PCTL_R |= 0x4; //Enter command mode

	uart_sendChar(0x01); //Send command
	uart_sendChar(0x00);
	response = uart_receive(); //Wait for response
	GPIO_PORTB_PCTL_R &= 0xFFFFFFFB;//Leave command mode
	if(response != 0) {
	//An error occurred…
	}
}
// End Part 4








char message[20];
int count;
int main(void) {
	count = 0;
//	message[0] = '\0';
	lcd_init();
	uart_init();
	button_init();
	int button = 0;
	int prev_button = -1;

//	WiFi_start();
//	WiFi_stop();
	while(1)
	{

		//if transmission is not IDLE && count !=19
		//a way to exit out of the receive loop
		message[count] = uart_receive();
		if(message[count] != '\0' && message[count] != '\r')
		{
			lcd_printf("%c, %d", message[count], count);
			uart_sendChar(message[count]);
			//if count = 19 print message
			if(count == 19)
			{
				lcd_clear();
				lcd_puts(message);
				count = 0;
			}
			count++;
		} else if(message[count] == '\r') {
			uart_sendChar('\r');
			uart_sendChar('\n');
		}

		//print character and count number

		button = button_getButton();

		if(button != prev_button)
		{
			prev_button = button;
			switch (button) {
			case 1:
				uart_sendString("Error: Sleep not found\0");
				break;

			case 2:
				uart_sendString("Maximum Effort -DP\0");
				break;

			case 3:
				uart_sendString("500 Error: Maybe?\0");
				break;

			case 4:
				uart_sendString("Blue, no green, Ahhhhh!!!\0");
				break;

			case 5:
				uart_sendString("No\0");
				break;

			case 6:
				uart_sendString("Yes\0");
				break;

			}
			uart_sendChar('\r');
			uart_sendChar('\n');

		}
		button = 0;

		//print total
	}

	
	return 0;
}
