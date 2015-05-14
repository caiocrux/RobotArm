#include <avr/io.h>
#include <util/delay.h>
static const int MIN_PULSE_WIDTH = 250;
static const int MAX_PULSE_WIDTH = 500;
static const int INT_PULSE = 250; // 	(MAX_PULSE_WIDTH – MIN_PULSE_WIDTH)

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void servoInit(void) {
	//Configure TIMER1
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);        //NON Inverted PWM
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //PRESCALER=64 MODE 14(FAST PWM)

	TCCR3A = (1<<COM3A1)|(1<<COM3B1)|(1<<WGM31);
	TCCR3B = (1<<WGM33)|(1<<WGM32)|(1<<CS31)|(1<<CS30); //PRESCALER=64 MODE 14(FAST PWM)

	ICR1=4999;  //fPWM=50Hz (Period = 20ms Standard).
	ICR3=4999;  //fPWM=50Hz (Period = 20ms Standard).

	DDRB|=(1<<PB5)|(1<<PB6);   //PWM Pins as Out
	DDRE|=(1<<PE3)|(1<<PE4);   //PWM Pins as Out

	OCR1A=375;   //0 degree
	OCR1B=375;   //0 degree
	OCR3A=375;   //0 degree
	OCR3B=375;   //0 degree
}

void servoSetPosition(int servo,int position) {

//uint16_t servo_pos= (uint16_t) ( (float) ( ((position*INT_PULSE)/180.0) + MIN_PULSE_WIDTH) );

uint16_t servo_pos= map( position, -90, 90, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
	switch(servo) {
		case 0:
		OCR1A=servo_pos;
		break;

		case 1:
		OCR1B=servo_pos;
		break;

		case 2:
		OCR3A=servo_pos;
		break;

		case 3:
		OCR3B=servo_pos;
		break;

		default:

		break;

	}
}
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

