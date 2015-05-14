#include <avr/io.h>
#include <util/delay.h>
#include "include/pwm.h"
#include "include/adc.h"

const int time = 100;
int main (void) {
	int i = 0;
	servoInit();
	adc_init();
while(1) {
	while ( i < 4 ) {
	servoSetPosition(i,read_adc(i));
	_delay_ms(time);
	i++;
	}
	i=0;
}
	return 0;
}

