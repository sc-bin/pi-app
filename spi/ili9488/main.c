
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <wiringPi.h>
#include <time.h>
#include "lcd.h"
#include "spi.h"

#define USE_HORIZONTAL 270
#define c1 0xf800
#define c2 0x7e0
#define c3 0x1f
#define use_color c1

// 管理LCD重要参数
// 默认为竖屏
_lcd_dev lcddev;

void LCD_direction(uint16_t direction)
{
    lcddev.setxcmd = 0x2A;
    lcddev.setycmd = 0x2B;
    lcddev.wramcmd = 0x2C;
    switch (direction)
    {
    case 0:
        lcddev.width = LCD_W;
        lcddev.height = LCD_H;
        LCD_WriteReg(0x36, (1 << 3) | (0 << 6) | (0 << 7)); // BGR==1,MY==0,MX==0,MV==0
        break;
    case 90:
        lcddev.width = LCD_H;
        lcddev.height = LCD_W;
        LCD_WriteReg(0x36, (1 << 3) | (0 << 7) | (1 << 6) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
        break;
    case 180:
        lcddev.width = LCD_W;
        lcddev.height = LCD_H;
        LCD_WriteReg(0x36, (1 << 3) | (1 << 6) | (1 << 7)); // BGR==1,MY==0,MX==0,MV==0
        break;
    case 270:
        lcddev.width = LCD_H;
        lcddev.height = LCD_W;
        LCD_WriteReg(0x36, (1 << 3) | (1 << 7) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
        break;
    default:
        break;
    }
}

void LCD_Init(void)
{
    // LCD_Reset(); //初始化之前复位
    //************* ILI9488初始化**********//
    LCD_WR_REG(0XF7);
    LCD_WR_DATA(0xA9);
    LCD_WR_DATA(0x51);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x82);
    LCD_WR_REG(0xC0);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x09);
    LCD_WR_REG(0xC1);
    LCD_WR_DATA(0x41);
    LCD_WR_REG(0XC5);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x80);
    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0xB0);
    LCD_WR_DATA(0x11);
    LCD_WR_REG(0xB4);
    LCD_WR_DATA(0x02);
    LCD_WR_REG(0xB6);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x42);
    LCD_WR_REG(0xB7);
    LCD_WR_DATA(0xc6);
    LCD_WR_REG(0xBE);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x04);
    LCD_WR_REG(0xE9);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0x36);
    LCD_WR_DATA((1 << 3) | (0 << 7) | (1 << 6) | (1 << 5));
    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x66);
    // LCD_WR_DATA(0x66);
    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x41);
    LCD_WR_DATA(0x89);
    LCD_WR_DATA(0x4B);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x0C);
    LCD_WR_DATA(0x0E);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x1B);
    LCD_WR_DATA(0x0F);
    LCD_WR_REG(0XE1);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x1A);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x0E);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x2F);
    LCD_WR_DATA(0x45);
    LCD_WR_DATA(0x43);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x32);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x0F);
    LCD_WR_REG(0x11);

    LCD_WR_REG(0x29);

    // 设置LCD属性参数
    LCD_direction(USE_HORIZONTAL); // 设置LCD显示方向
}
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
}
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(xStar >> 8);
    LCD_WR_DATA(0x00FF & xStar);
    LCD_WR_DATA(xEnd >> 8);
    LCD_WR_DATA(0x00FF & xEnd);

    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(yStar >> 8);
    LCD_WR_DATA(0x00FF & yStar);
    LCD_WR_DATA(yEnd >> 8);
    LCD_WR_DATA(0x00FF & yEnd);

    LCD_WriteRAM_Prepare(); // 开始写入GRAM
}
void LCD_WR_DATA_16Bit(uint16_t Data)
{
    // 18Bit
    LCD_WR_DATA((Data >> 8) & 0xF8); // RED
    LCD_WR_DATA((Data >> 3) & 0xFC); // GREEN
    LCD_WR_DATA(Data << 3);          // BLUE
}
void bit_16_18(uint16_t data, uint8_t *b1, uint8_t *b2, uint8_t *b3)
{
    *b1 = ((data >> 8) & 0xF8); // RED
    *b2 = ((data >> 3) & 0xFC); // GREEN
    *b3 = (data << 3);          // BLUE
}

/*****************************************************************************
 * @name       :void LCD_Clear(uint16_t Color)
 * @date       :2018-08-09
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
 ******************************************************************************/
void LCD_Clear(uint16_t Color)
{
// #define bf_size 480*2
#define bf_size 480 * 320 * 3 
    uint8_t buf[bf_size];

    LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);

    uint8_t b1, b2, b3;
    bit_16_18(Color, &b1, &b2, &b3);

    for (int i = 0; i < bf_size; i++)
    {
        buf[i] = b1;
        i++;
        buf[i] = b2;
        i++;
        buf[i] = b3;
    }
    printf("buf=0x%x\r\n", buf);
    // LCD_WRITE_BUF_DATA(buf, 4000 );

    int blok_size = 0;

    for (int i = 0; i < bf_size; i += blok_size)
    {
        blok_size = bf_size-i;

        if( blok_size >4000 )
            blok_size = 4000;

        LCD_WRITE_BUF_DATA(buf + i, blok_size);
    }
    // LCD_WRITE_BUF_DATA()
}

int main()
{

    spi_init();
    printf("\n");
    LCD_Init();
    LCD_Clear(c2);
    // LCD_SetWindows(50,50, 100,100);
    // for( int i = 0; i< 50*50; i++ )
    // {
    //     LCD_WR_DATA(0xf0);
    //     LCD_WR_DATA(0x00);
    //     LCD_WR_DATA(0x00);
    //     // LCD_WR_DATA_16Bit(c3);
    // }
    return 0;
}
