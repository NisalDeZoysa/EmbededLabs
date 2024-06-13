/*
 * Task_4_workshop_6.c
 *
 * Created: 4/22/2024 10:51:36 AM
 * Author : nisal
 */ 

#ifndef F_CPU
#define F_CPU 1600000UL    // Set the Clock frequency to 1.6 MHz
#endif
#include <avr/io.h>
#include <util/delay.h>

#include "LCD/lcd.h"    // Include the LCD library

// Initialize ADC settings
void adc_init()
{
	ADMUX = (1<<REFS0);   // Set reference voltage to AVcc
	
	
	// Enable ADC and set the prescaler to 128 for ADC clock
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

// Read ADC value from a specified channel
uint16_t adc_read(uint8_t ch)
{
	
	ch &= 0b00000111;   // Mask input channel number to lower 3 bits
	ADMUX = (ADMUX & 0xF8)|ch;

	ADCSRA |= (1<<ADSC);    // Start ADC conversion

	// wait for conversion to complete
	// ADSC becomes '0' again
	
	while(ADCSRA & (1<<ADSC));   // Wait until ADC conversion is finished

	return (ADC);
}

int main()
{
	DDRB=0xff;    // Set PORTB as output
	uint16_t adc_result0;
	int temp;
	int far;
	char buffer[10];
	

	// initialize ADC and LCD
	adc_init();
	lcd_init(LCD_DISP_ON_CURSOR); //Initialize LCD display with cursor

	lcd_clrscr();   // Clear LCD screen
	lcd_gotoxy(0,0);

	_delay_ms(50);   // Short delay for LCD to settle

	while(1)
	{
		adc_result0 = adc_read(0);      // Read ADC value from channel 0 (PA0)
		
		temp=adc_result0/2.01;   // finding the temperature
		
		// Display the temperature on LCD
		lcd_gotoxy(0,0);
		itoa(temp,buffer,10);   // Convert integer 'temp' to string
		lcd_puts("Temperature =");   // Display label
		lcd_puts(buffer);    // Display temperature value
		lcd_gotoxy(15,0);
		lcd_puts("C");     // Append 'C' for Celsius
		_delay_ms(1000);   // Wait for 1 second before updating the display
		
		
	}
}