
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <wiringPi.h>
#include <time.h>

#include "setting.h"
#include "lcd.h"
#define WIDTH 240
#define HEIGHT 240

int random_range(int min, int max)
{
    int num = rand() % (max - min + 1) + min;
    srand(time(NULL) + num);
    return num;
}

int main()
{
    LCD_Init();
    LCD_direction(90);
    // LCD_FULL(0x0000);
    int x1, x2;
    int y1, y2;
    while (1)
    {
        // LCD_area(RED, 0, 0, WIDTH / 3 - 1, 100);
        // LCD_area(GREEN, WIDTH / 3, 0, WIDTH / 3 * 2 - 1, 100);
        // LCD_area(BLUE, WIDTH / 3 * 2, 0, WIDTH - 1, 100);

        x1 = random_range(0, WIDTH);
        x2 = random_range(x1, WIDTH);
        y1 = random_range(100, HEIGHT);
        y2 = random_range(y1, HEIGHT);
        // LCD_area(random_range(0, 0xffff), x1, y1, x2, y2);

        LCD_area(0xffff, 0, 0, 240-1, 240-1);
        sleep(1);
        LCD_area(0xf800, 0, 0, 240-1, 240-1);
        sleep(1);

    }


    return 0;
}
