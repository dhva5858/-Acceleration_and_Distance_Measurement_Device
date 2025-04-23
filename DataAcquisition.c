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
{
	char Text[64];
	uint16_t index=0;
	uint32_t value;
	uint32_t ADCValue;

	InitialiseHardware();
	sprintf(Text,"\rAcceleration Measurement\r\n");
	uart_puts (Text); _delay_ms(1000);

	while(1)
	{
		if (intnum) {
			ADCValue=ReadChannel(6);
			value=value + ADCValue;
			//TogglePortD(7);
			index++;
			intnum = 0;
			if (index==10){
				sprintf( Text,"%ld\r\n",value);
				uart_puts (Text);
				TogglePortD(7);

				value=0;
				index=0;
			}
		}
	}
}




ISR(TIMER0_COMP_vect)
/*
Interrupt service routine for timer 0 interrupt.
*/
{
	intnum++;
}
