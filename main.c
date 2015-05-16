#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "include/pwm.h"
#include "include/adc.h"
#include "include/usart.h"

const int time = 1000;
int main (void) {
	int i = 0;
	servoInit();
	adc_init();
	USART_Init(9600);
	sei();
	USART_SendString("Starting the Robot Arm !");
	while(1) {
	
		while ( i < 4 ) {
			servoSetPosition(i,read_adc(i));
			//USART_SendString("Value received :");
			//USART_SendInterger(read_adc(i));
			//USART_SendString("\r\n");
			//_delay_ms(time);
			i++;
		}
	i=0;

}
	return 0;
}

