#define DEV_I2C "/dev/i2c-1"
#define I2C_ADDR 0

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int fd;

int i2c_read(uint8_t addr, uint8_t reg,  int read_count, uint8_t *value)
{
    struct i2c_rdwr_ioctl_data data;
    struct i2c_msg msgs[2];

    msgs[0].addr = addr;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = &reg;

    msgs[1].addr = addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = read_count;
    msgs[1].buf = value;

    data.msgs = msgs;
    data.nmsgs = 2;

    int res = ioctl(fd, I2C_RDWR, &data);
    if (res < 0)
        printf("Read failed\n");

    return res;
}
uint16_t MLX90614_Data_Process(uint8_t Data[3])
{
	uint16_t temp;
	temp = (Data[1]<<8)+Data[0];//高位与低位结合
	temp = temp*2-27315;//将数据扩大100倍
	return temp;
}
int main()
{
    fd = open(DEV_I2C, O_RDWR);
    if (fd < 0)
    {
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }
    uint8_t buf[3];
    i2c_read(0, 0x07, 3, buf);
 
    printf("temp=%d\r\n", MLX90614_Data_Process(buf));
   
    return 0;
}
