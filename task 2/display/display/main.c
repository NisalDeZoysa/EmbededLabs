#include "LCD.h"
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	lcd_init();
	char word_to_print[] = "Embedded Systems";
	int word_length = strlen(word_to_print);
	
	while(1)
	{
		

		for (int i = 0; i <= word_length; i++)
		{
			lcd_clear();
			lcd_goto_xy(0, i);
			lcd_write_word(word_to_print);
			_delay_ms(500);
		}
		for (int i = 0; i <= word_length; i++) {
			lcd_clear();
			lcd_goto_xy(0, 0);
			lcd_write_word(&word_to_print[i]);
			_delay_ms(500);
		}
		lcd_clear();
		lcd_goto_xy(1, 0);
		lcd_write_word("DEIE");
		_delay_ms(1000);
	}
}
