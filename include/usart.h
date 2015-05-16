/*
 * usart.h
 *
 *  Created on: Sep 7, 2014
 *      Author: caio
 */

#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define SERIAL_RX_RING_SIZE 64
#define SERIAL_TX_RING_SIZE 64

#define common_zero_mem(__ptr, __len) while (__len--) (*(char *)(__ptr)) = 0x00

typedef enum _e_serial_speed {
	E_BAUD_2400 = 2400,
	E_BAUD_4800 = 4800,
	E_BAUD_9600 = 9600,
	E_BAUD_14400 = 14400,
	E_BAUD_19200 = 19200,
	E_BAUD_28800 = 28800,
	E_BAUD_38400 = 38400,
	E_BAUD_57600 = 57600,
} e_serial_speed;

typedef struct _t_buffer {
	volatile unsigned char ring[SERIAL_RX_RING_SIZE];	// data storage space for the ring buffer
	volatile unsigned char head; 						// head index
	volatile unsigned char tail;						// tail index

} t_sBuffer;

int USART_Init(int iSpeed);
int USART_SendByte(unsigned char cData);
unsigned char USART_ReceiveByte(unsigned char *a_data);
void USART_SendString(char* cStringPtr);
void USART_SendInterger(int iValue);
void USART_RingFlush(void);
void USART_SerialFlush (void);
