#include <stdlib.h>
#include "open_interface.h"

void sonar_init();
void TIMER3B_Handler(void);
void send_pulse();
unsigned time2distance();