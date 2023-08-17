
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <wiringPi.h>
#include <time.h>
#include "setting.h"
#include "hw_spi.h"

#include "lcd.h"

#define c1 0xf800
#define c2 0x7e0
#define c3 0x1f
#define use_color c1

int main()
{

    spi_init();
    LCD_Init();
    LCD_FULL_RGB565(0x001f);

    // while(1)
    // {
    //     LCD_Clear(c1);
    //     LCD_Clear(0);
    //     LCD_Clear(c2);
    //     LCD_Clear(0);
    //     LCD_Clear(c3);
    //     LCD_Clear(0);

    // }

    // LCD_SetWindows(50, 50, 100, 100);
    // for (int i = 0; i < 50 * 50; i++)
    // {
    //     LCD_WR_DATA(0XFf);
    //     LCD_WR_DATA(0xe0);  //red


    // }


    uint16_t a = 0xabcd;
    uint8_t *p = &a;
    printf("p=%x\r\n", *p);

    return 0;
}
