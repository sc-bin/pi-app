#ifndef _LCD_H
#define _LCD_H

#include <stdint.h>

void LCD_Init(void);

void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);
void LCD_FULL_RGB565(uint16_t Color);


#endif

