
#include "hw_spi.h"
#include "setting.h"
#include <stdio.h>
#define LCD_W 240
#define LCD_H 240
#include <unistd.h>

int lcd_width;
int lcd_height;

void LCD_direction(uint16_t direction)
{

    switch (direction)
    {
    case 0:
        lcd_width = LCD_W;
        lcd_height = LCD_H;
        LCD_WriteReg(0x36, (1 << 3) | (1 << 6));

        break;
    case 90:
        lcd_width = LCD_H;
        lcd_height = LCD_W;
        LCD_WriteReg(0x36, (1 << 3) | (1 << 5));
        break;
    case 180:
        lcd_width = LCD_W;
        lcd_height = LCD_H;

        LCD_WriteReg(0x36, (1 << 3) | (1 << 7));
        break;
    case 270:
        lcd_width = LCD_H;
        lcd_height = LCD_W;
        LCD_WriteReg(0x36, (1 << 3) | (1 << 7) | (1 << 6) | (1 << 5));
        break;
    default:
        break;
    }
}

void LCD_Init(void)
{
    spi_init();

LCD_WR_REG(0x11);
LCD_WR_REG(0x11);

LCD_WR_REG(0x36);
 LCD_WR_DATA(0xA0);

LCD_WR_REG(0x3a);
LCD_WR_DATA(0x65);

LCD_WR_REG(0x21);
LCD_WR_REG(0x2a);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0xef);
LCD_WR_REG(0x2b);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0xef);

//--------------------------------ST7789V Frame rate setting----------------------------------//
LCD_WR_REG(0xb2);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x33);
LCD_WR_DATA(0x33);
LCD_WR_REG(0xb7);
LCD_WR_DATA(0x35);
//---------------------------------ST7789V Power setting--------------------------------------//
LCD_WR_REG(0xbb);
LCD_WR_DATA(0x1f);
LCD_WR_REG(0xc0);
LCD_WR_DATA(0x2c);
LCD_WR_REG(0xc2);
LCD_WR_DATA(0x01);
LCD_WR_REG(0xc3);
LCD_WR_DATA(0x12);
LCD_WR_REG(0xc4);
LCD_WR_DATA(0x20);
LCD_WR_REG(0xc6);
LCD_WR_DATA(0x0f);
LCD_WR_REG(0xd0);
LCD_WR_DATA(0xa4);
LCD_WR_DATA(0xa1);
//--------------------------------ST7789V gamma setting--------------------------------------//
LCD_WR_REG(0xe0);
LCD_WR_DATA(0xd0);
LCD_WR_DATA(0x08);
LCD_WR_DATA(0x11);
LCD_WR_DATA(0x08);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x15);
LCD_WR_DATA(0x39);
LCD_WR_DATA(0x33);
LCD_WR_DATA(0x50);
LCD_WR_DATA(0x36);
LCD_WR_DATA(0x13);
LCD_WR_DATA(0x14);
LCD_WR_DATA(0x29);
LCD_WR_DATA(0x2d);
LCD_WR_REG(0xe1);
LCD_WR_DATA(0xd0);
LCD_WR_DATA(0x08);
LCD_WR_DATA(0x10);
LCD_WR_DATA(0x08);
LCD_WR_DATA(0x06);
LCD_WR_DATA(0x06);
LCD_WR_DATA(0x39);
LCD_WR_DATA(0x44);
LCD_WR_DATA(0x51);
LCD_WR_DATA(0x0b);
LCD_WR_DATA(0x16);
LCD_WR_DATA(0x14);
LCD_WR_DATA(0x2f);
LCD_WR_DATA(0x31);

// LCD_WR_REG(0x2A); //Column Address Set
// LCD_WR_DATA(0x00);
// LCD_WR_DATA(0x00); //0
// LCD_WR_DATA(0x00);
// LCD_WR_DATA(0xEF); //239

// LCD_WR_REG(0x2B); //Row Address Set
// LCD_WR_DATA(0x00);
// LCD_WR_DATA(0x00); //0
// LCD_WR_DATA(0x00);
// LCD_WR_DATA(0xEF); //239

// LCD_WR_REG(0x29);	//Display on	

    // 设置LCD属性参数
    // LCD_direction(LCD_ROTATIONS); // 设置LCD显示方向
}

void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{

    LCD_WR_REG(0x2A);
    LCD_WR_DATA(xStar >> 8);
    LCD_WR_DATA(0x00FF & xStar);
    LCD_WR_DATA(xEnd >> 8);
    LCD_WR_DATA(0x00FF & xEnd);

    LCD_WR_REG(0x2B);
    LCD_WR_DATA(yStar >> 8);
    LCD_WR_DATA(0x00FF & yStar);
    LCD_WR_DATA(yEnd >> 8);
    LCD_WR_DATA(0x00FF & yEnd);

    LCD_WR_REG(0x2C);
}
void _LCD_SEND_DATA(uint16_t Color, int num)
{
    if (num > (LCD_H * LCD_W))
        return;

    uint16_t buf[LCD_W * LCD_H];
    for (int i = 0; i < num; i++)
    {
        buf[i] = Color;
    }

    // 颠倒字节序
    uint16_t buf2[LCD_W * LCD_H];
    uint8_t *b1, b2;
    uint8_t *s;
    uint8_t *d;
    for (int i = 0; i < num; i++)
    {
        s = (uint8_t *)&buf[i];
        d = (uint8_t *)&buf2[i];
        *d = *(s + 1);
        *(d + 1) = *s;
    }
    LCD_WR_REG(0x2C);

    // 发送
    uint8_t *p;
    p = (uint8_t *)buf2;
    int blok_size = 0;
    int p_size = num * 2;
    for (int i = 0; i < p_size; i += blok_size)
    {
        blok_size = p_size - i;

        if (blok_size > 4000)
            blok_size = 4000;

        LCD_WRITE_BUF_DATA(p + i, blok_size);
    }
}
void LCD_FULL(uint16_t Color)
{
    LCD_SetWindows(0, 0, lcd_width - 1, lcd_height - 1);
    _LCD_SEND_DATA(Color, LCD_W * LCD_H);
}

int _num_scope(int num, int small, int big)
{

    if (num < small)
    {
        return small;
    }
    if (num > big)
    {
        return big;
    }

    return num;
}

void LCD_area(uint16_t Color, int startx, int starty, int endx, int endy)
{

    startx = _num_scope(startx, 0, lcd_width);
    starty = _num_scope(starty, 0, lcd_height);
    endx = _num_scope(endx, 0, lcd_width);
    endy = _num_scope(endy, 0, lcd_height);

    LCD_SetWindows(startx, starty, endx, endy);
    _LCD_SEND_DATA(Color, (endx - startx + 1) * (endy - starty));
LCD_WR_REG(0x29);	//Display on	

}
