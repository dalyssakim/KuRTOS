#include "raw_debug.h"
#include "itoa.h"
#include "qsk_lcd.h"

void display_int_line1(int i)
{
		DisplayString(LCD_LINE1, itoa(i));	
}


void display_int_line2(int i)
{
		DisplayString(LCD_LINE2, itoa(i));	
}

void diaplsy_char_line1(char *a)
{
		DisplayString(LCD_LINE1, a);	
}

void diaplsy_char_line2(char *a)
{
		DisplayString(LCD_LINE2, a);	
}