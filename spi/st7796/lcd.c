
#include "hw_spi.h"
#include "setting.h"
#define LCD_W 320
#define LCD_H 480

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

    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x55);
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
    LCD_direction(LCD_ROTATIONS); // 设置LCD显示方向
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

void _num_scope(int *num, int small, int big)
{
    if (*num < small)
    {
        *num = small;
    }
    if (*num > big)
    {
        *num = big;
    }
}

void LCD_area(uint16_t Color, int startx, int starty, int endx, int endy)
{
    _num_scope(&startx, 0, lcd_width);
    _num_scope(&starty, 0, lcd_height);
    _num_scope(&endx, 0, lcd_width);
    _num_scope(&endy, 0, lcd_height);
    LCD_SetWindows(startx, starty, endx, endy);
    _LCD_SEND_DATA(Color, (endx - startx+1) * (endy - starty));
}
