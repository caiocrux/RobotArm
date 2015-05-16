/*
 * usart.c
 *
 *  Created on: Sep 7, 2014
 *      Author: caio
 */
#include "../include/usart.h"
#include <stdio.h>
#include <avr/interrupt.h>

// the RX ring buffer
static volatile t_sBuffer g_rx_buff;
static volatile t_sBuffer g_tx_buff;

int USART_Init(int iSpeed){

	int iBaudRate = 0;
	UCSR0A = 0x00; // Disable double mode

	switch (iSpeed){
	case E_BAUD_2400:	UCSR0A |= (1<<U2X0); iBaudRate = 832; break;
	case E_BAUD_4800:						 iBaudRate = 207; break;
	case E_BAUD_9600:						 iBaudRate = 103; break;
	case E_BAUD_19200:						 iBaudRate =  51; break;
	case E_BAUD_28800:						 iBaudRate =  34; break;
	case E_BAUD_38400:						 iBaudRate =  25; break;
	case E_BAUD_57600:	UCSR0A |= (1<<U2X0); iBaudRate =  34; break;
	default : break;
	}
	UBRR0H = (iBaudRate >> 8) & 0xff;
	UBRR0L = iBaudRate & 0xff;
	UCSR0B |= (1 << RXEN0 ) | (1 << TXEN0 ); 		// Turn on the transmission and reception

	UCSR0B &= ~_BV(TXCIE0);			// disable transmission complete interrupt
	UCSR0B |= (1 << RXCIE0);		// Enablet the reception ISR

	UCSR0C = (1 << UCSZ00 ) | (1 << UCSZ01 ) ;  	// Set frame format to 8 data bits, no parity, 1 stop bit
	//UBRR0= UBRR_VALUE;							// In AVR GCC we can simply assign UBRR_VALUE value to UBRR0 and leave to compiler to take care of writing sequence. ;)
	USART_RingFlush();
	USART_SerialFlush();


	return 1;
}

int USART_SendByte(unsigned char cData){
	unsigned int status = 0x00;
	unsigned int next = ((g_tx_buff.head + 1) % SERIAL_TX_RING_SIZE);

	if (next != g_tx_buff.tail) { // do not overflow the buffer
		g_tx_buff.ring[g_tx_buff.head] = cData;
		g_tx_buff.head = next;
		status = 1;
		UCSR0B |= _BV(UDRIE0); // enable data register empty interrupt
	}
	return status;
}

unsigned char USART_ReceiveByte(unsigned char *a_data){
	//PORTB ^= _BV(PB4);
	if (g_rx_buff.head == g_rx_buff.tail){
		return 0;
	}
	*a_data = g_rx_buff.ring[g_rx_buff.tail];
	g_rx_buff.tail = (g_rx_buff.tail + 1) % SERIAL_RX_RING_SIZE;
	return 1;
}

void USART_SendString(char* cStringPtr){
	while(*cStringPtr != 0x00){    		//Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
		USART_SendByte(*cStringPtr);    //Using the simple send function we send one char at a time
		cStringPtr++;					//We increment the pointer so we can read the next char
	}
}

void USART_SendInterger(int iValue){
	char cInterger[5];
	sprintf(cInterger, "%d", iValue);
	USART_SendString(cInterger);
	USART_SendString("\r\n");
}

void USART_RingFlush (void){
	 int FlushRing = sizeof(g_rx_buff);
	 common_zero_mem(&g_rx_buff, FlushRing);

	 FlushRing = sizeof(g_tx_buff);
	 common_zero_mem(&g_tx_buff, FlushRing);

}
void USART_SerialFlush (void){
	unsigned char dummy __attribute__((unused)) = 0x00;
	g_rx_buff.tail = g_rx_buff.head = 0x00;
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}

ISR (USART0_RX_vect ){
//	PORTB ^= _BV(PB3);

	if (bit_is_clear(UCSR0A, FE0)) { 			// must read the data in order to clear the interrupt flag
		volatile unsigned char data = UDR0;		// calculate the next available ring buffer data bucket index
		volatile unsigned char next =((g_rx_buff.head + 1) % SERIAL_RX_RING_SIZE);		/// do not overflow the buffer
		if (next != g_rx_buff.tail) {
			g_rx_buff.ring[g_rx_buff.head] = data;
			g_rx_buff.head = next;
		}
	}
	else {
		volatile unsigned char data __attribute__((unused)) = UDR0;/// must read the data in order to clear the interrupt flag
	}
}

ISR (USART0_UDRE_vect){
//	PORTB ^= _BV(PB4);
	if (g_tx_buff.head != g_tx_buff.tail) { // proceed if there still is data to be send
		UDR0 = g_tx_buff.ring[g_tx_buff.tail];
		g_tx_buff.tail = (g_tx_buff.tail + 1) % SERIAL_TX_RING_SIZE;
	}
	else {
		UCSR0B &= ~_BV(UDRIE0);// mask the interrupt everything has been send

	}
}


