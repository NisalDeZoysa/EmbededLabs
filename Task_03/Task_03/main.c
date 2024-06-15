/*
 * Task_03.c
 *
 * Created: 4/26/2024 10:25:15 PM
 * Author : CE05 & CE06
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// Define LCD Pins
#define LCD_RS 12
#define LCD_EN 11
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 1

// Function prototypes
void LCD_command(unsigned char cmd);
void LCD_data(unsigned char data);
void LCD_init();
void LCD_print(const char *str);
void LCD_setCursor(uint8_t col, uint8_t row);
void ADC_init();
uint16_t ADC_read(uint8_t channel);

int main(void) {
	// Initialize LCD and ADC
	LCD_init();
	ADC_init();

	// Main loop
	while (1) {
		// Read analog value from A2 pin
		uint16_t val = ADC_read(2);

		// Display analog value on LCD
		LCD_setCursor(0, 0);
		LCD_print("Analog Voltage");
		LCD_setCursor(0, 1);
		char buffer[16];
		snprintf(buffer, sizeof(buffer), "%d", val);
		LCD_print(buffer);

		_delay_ms(500); // Delay for stability
	}

	return 0;
}

void LCD_command(unsigned char cmd) {
	PORTD = cmd & 0xF0;
	PORTB &= ~(1 << LCD_RS); // RS = 0 for command
	PORTB |= (1 << LCD_EN); // EN = 1 to enable
	_delay_us(1);
	PORTB &= ~(1 << LCD_EN); // EN = 0 to disable
	_delay_us(100);

	PORTD = (cmd << 4) & 0xF0;
	PORTB |= (1 << LCD_EN); // EN = 1 to enable
	_delay_us(1);
	PORTB &= ~(1 << LCD_EN); // EN = 0 to disable
	_delay_ms(2);
}

void LCD_data(unsigned char data) {
	PORTD = data & 0xF0;
	PORTB |= (1 << LCD_RS); // RS = 1 for data
	PORTB |= (1 << LCD_EN); // EN = 1 to enable
	_delay_us(1);
	PORTB &= ~(1 << LCD_EN); // EN = 0 to disable
	_delay_us(100);

	PORTD = (data << 4) & 0xF0;
	PORTB |= (1 << LCD_EN); // EN = 1 to enable
	_delay_us(1);
	PORTB &= ~(1 << LCD_EN); // EN = 0 to disable
	_delay_ms(2);
}

void LCD_init() {
	_delay_ms(20);
	LCD_command(0x02); // Initialize LCD in 4-bit mode
	LCD_command(0x28); // 4-bit mode, 2 lines, 5x8 font
	LCD_command(0x0C); // Display on, cursor off
	LCD_command(0x01); // Clear display
	_delay_ms(2);
	LCD_command(0x06); // Entry mode set: increment cursor, no shift
}

void LCD_print(const char *str) {
	while (*str)
	LCD_data(*str++);
}

void LCD_setCursor(uint8_t col, uint8_t row) {
	if (row == 0)
	LCD_command(0x80 + col); // Set cursor to first row address
	else
	LCD_command(0xC0 + col); // Set cursor to second row address
}

void ADC_init() {
	// Set AREF to AVCC, enable ADC, enable ADC interrupt
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t ADC_read(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Select ADC channel
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
	return ADC; // Return ADC value
}
