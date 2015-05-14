#ifndef INCLUDE_PWM_H_
#define INCLUDE_PWM_H_

static const int MIN_PULSE_WIDTH = 250;
static const int MAX_PULSE_WIDTH = 500;
static const int INT_PULSE = 250; // 	(MAX_PULSE_WIDTH – MIN_PULSE_WIDTH)

void servoInit(void);
void servoSetPosition(int servo,int position);

#endif /* INCLUDE_PWM_H_ */
