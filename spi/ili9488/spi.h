#ifndef _ILI_SPI_H
#define _ILI_SPI_H
#include <stdint.h>

void spi_init();
void LCD_WR_REG(uint8_t Reg);
void LCD_WR_DATA(uint8_t Data);
void LCD_WRITE_BUF_DATA(uint8_t *buf, int len);

void LCD_WriteReg(uint8_t LCD_Reg, uint8_t LCD_RegValue);

#endif

