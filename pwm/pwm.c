#define pwmchip "/sys/class/pwm/pwmchip0/"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


void pwm_export(int num)
{
    int fd;
    char num_str[10];
    fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
    if (fd < 0)
        exit(-1);

    sprintf(num_str, "%d", num);
    write(fd, num_str, strlen(num_str));
    close(fd);
}

/*
num： 哪个pwm
period:  周期，单位是ns
duty_cycle:  一个周期内高电平的时间，单位是ns
*/
void pwm_set(int num, int period, int duty_cycle)
{
    int fd;
    char period_str[20], duty_cycle_str[20];
    char path[100];

    sprintf(path, "/sys/class/pwm/pwmchip0/pwm%d/period", num);
    fd = open(path, O_WRONLY);
    if (fd < 0)
        exit(-1);

    sprintf(period_str, "%d", period);
    write(fd, period_str, strlen(period_str));
    close(fd);

    sprintf(path, "/sys/class/pwm/pwmchip0/pwm%d/duty_cycle", num);
    fd = open(path, O_WRONLY);
    if (fd < 0)
        exit(-1);

    sprintf(duty_cycle_str, "%d", duty_cycle);
    write(fd, duty_cycle_str, strlen(duty_cycle_str));
    close(fd);
}
void pwm_enable(int num)
{
    int fd;
    char path[100];

    sprintf(path, "/sys/class/pwm/pwmchip0/pwm%d/enable", num);
    fd = open(path, O_WRONLY);
    if (fd < 0)
        exit(-1);
    write(fd, "1", 1);
    close(fd);
}
void pwm_disable(int num)
{
    int fd;
    char path[100];

    sprintf(path, "/sys/class/pwm/pwmchip0/pwm%d/enable", num);
    fd = open(path, O_WRONLY);
    if (fd < 0)
        exit(-1);
    write(fd, "0", 1);
    close(fd);
}

int main()
{
    //linux下的pwm框架在使用前需要先导出
    pwm_export(1);
    pwm_export(2);
    pwm_export(3);
    pwm_export(4);

    pwm_set(1, 200, 100);
    pwm_set(2, 100000, 100);
    pwm_set(3, 100000, 1000000);
    pwm_set(4, 400, 100);

    pwm_enable(1);
    pwm_enable(2);
    pwm_enable(3);
    pwm_enable(4);
    sleep(3);
    // pwm_disable(1);
    // pwm_disable(2);
    // pwm_disable(3);
    // pwm_disable(4);
    
    return 0;
}
