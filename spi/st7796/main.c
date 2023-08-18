
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

int random_range(int min, int max)
{
    int num = rand() % (max - min + 1) + min;
    srand(time(NULL) + num);
    return num;
}

int main()
{
    LCD_Init();
    LCD_FULL(0x0000);

    int x1, x2;
    int y1, y2;
    while (1)
    {
        LCD_area(RED, 0, 0, 480 / 3 - 1, 100);
        LCD_area(GREEN, 480 / 3, 0, 480 / 3 * 2 - 1, 100);
        LCD_area(BLUE, 480 / 3 * 2, 0, 480 - 1, 100);

        x1 = random_range(0, 480);
        x2 = random_range(x1, 480);
        y1 = random_range(100, 320);
        y2 = random_range(y1, 320);
        LCD_area(random_range(0, 0xffff), x1, y1, x2, y2);
        sleep(1);
    }


    return 0;
}
