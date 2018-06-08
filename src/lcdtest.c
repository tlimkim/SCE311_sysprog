/*
 * Developed by Taeklim Kim
 *
 * lcdtest.c is for sending a message to LCD output.
 * There are two function of lcd_print() and lcd_clear()
 *
 */

#include "main.h"

#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>
#include <string.h>

#define LCD_RS 9
#define LCD_E 8

#define LCD_D4 3
#define LCD_D5 2
#define LCD_D6 1
#define LCD_D7 0

int lcd = 0;

/*
 * lcd_print activate when a baby is in dangerous range, and
 * a baby have pee on his or her pants
 */
void lcd_print (char * msg, int temp)
{   
  int lcd;
  char s1[10];
  char print[50];

  wiringPiSetup();

  // Initiating LCD
  lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, 
      LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

  if (lcd == -1) {
    printf("lcd init failed! \n");
  }
  
  if (temp == 0) {
    lcdPosition(lcd, 0, 0);
    lcdPuts(lcd, msg);
    return;
  }

  strcat(print, msg);
  strcat(print, " temp: ");

  sprintf(s1, "%d", temp);
  strcat(print, s1);
  
  lcdPosition(lcd, 0, 0);
  lcdPuts(lcd, print);
}

/*
 * lcd_clear is for Clearing LCD if the baby is no more close enough
 */ 
void lcd_clear() 
{
  lcdClear(lcd);    
}
