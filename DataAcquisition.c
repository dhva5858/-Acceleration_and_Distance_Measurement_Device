/*
 * DataAcquisition.c
 *
 * Created: Sep. 2023, Bochert
 *
 * Program skeleton for the experiments in maritime 
 * systems laboratory of embedded system design.
 * Prior to modify the program, change the name in
 * the "Created:" line. 
 */ 

#define F_CPU 16000000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "fifo.h"
#include "uart.h"
#include "dataio.h"
#include "timer0interupt.h"

volatile uint16_t intnum=0;
volatile uint8_t *bufcounter;

void InitialiseHardware(void)
{
	sei();							// enables interrupts by setting the global interrupt mask
	AdcInit();						// initializes the a/d converter
	bufcounter = uart_init(19200);	// initializes the UART for the given baudrate
	PortInit();						// initializes the port settings
	StartTimer0Interrupt();			// timer 0 interrupt for 15 ms
}

int main(void)
/*
Example program for first experiments.
After initializing the interfaces and "Hello World" is send to the serial port.
In a period of a second port pin D7 is toggled and sample data are send to the 
serial port. These sample data contain an index, analog data input, digital port 
inputs and an interrupt counter.
*/
{
	char Text[64];
	int32_t V_100,V_0,V_U,T_U;
	int32_t a0;
	int32_t a1;
	
	InitialiseHardware(); 
	sprintf( Text,"\r\nWelocme to Temperature Measurement\r\n");
	uart_puts (Text); _delay_ms(1000);
	sprintf( Text,"\r\nPlease dip the sensor in ice and press the push button\r\n");
	uart_puts (Text); _delay_ms(1000);
	
	while(ReadPortD(3)); // Read Temp at 0  degree
	
	V_0=ReadChannel(6);
	//V_0=(3*Value_0)/1023;
	sprintf( Text,"\r\n V_0 = %ld",V_0);
	uart_puts (Text); _delay_ms(1000);
	
	sprintf( Text,"\r\nPlease dip the sensor in boiling water and press the push button\r\n");
	uart_puts (Text); _delay_ms(1000);
	
	while(ReadPortD(3)); // Read Temp at 100 degree
	
	V_100=ReadChannel(6);
	/*	V_100=(3*Value_100)/1023;*/
	sprintf( Text,"\r\n V_100 = %ld",V_100);
	uart_puts (Text); _delay_ms(1000);
	
	a1=100000/(V_100-V_0);
	sprintf( Text,"\r\n a1 = %ld",a1);
	uart_puts (Text); _delay_ms(1000);
	a0=-a1*V_0;
	sprintf( Text,"\r\n a0 = %ld",a0);
	uart_puts (Text); _delay_ms(1000);
	sprintf( Text,"\r\nPlease dip the sensor in unknown temperature\r\n");
	uart_puts (Text); _delay_ms(5000);
	
	

	while(1)
    {
	V_U=ReadChannel(6);
	//V_U=(3*Value_U)/1023;
	sprintf( Text,"\r\n V_U = %ld",V_U);
	uart_puts (Text); _delay_ms(1000);
	
	T_U=a1*V_U+a0;
	sprintf( Text,"\r\n Unknown Temperature T_U = %04ld  degC",T_U/100);
	Text[33]=Text[32];
	Text[32]='.';
	uart_puts (Text); _delay_ms(1000);	
		
		
    }
}

ISR(TIMER0_COMP_vect)
/*
Interrupt service routine for timer 0 interrupt.
*/
{
	intnum++;
}