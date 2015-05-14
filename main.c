#include <avr/io.h>
#include <util/delay.h>
#include "include/pwm.h"

const int time = 1000;
int main (void) {
	servoInit();
	int i = 0;
	//servoSetPosition(0,90);
	//servoSetPosition(1,45);
	//servoSetPosition(0,-90);
while(1) {
	while ( i < 4 ) {
	servoSetPosition(i,-90);
	_delay_ms(time);

	servoSetPosition(i,-45);
	_delay_ms(time);

	servoSetPosition(i,0);
	_delay_ms(time);

	servoSetPosition(i,45);
	_delay_ms(time);

	servoSetPosition(i,90);
	_delay_ms(time);
	i++;
	}
	i=0;
}
	return 0;
}

