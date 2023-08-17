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

int main()
{
    fd = open(DEV_I2C, O_RDWR);
    if (fd < 0)
    {
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }
    uint8_t buf[3];

    i2c_read(I2C_ADDR, 0x07, 3, buf);
    printf("read buf[0]:\t0x%x\r\n", buf[0]);
    printf("read buf[1]:\t0x%x\r\n", buf[1]);
    printf("read buf[2]:\t0x%x\r\n", buf[2]);

    
    return 0;
}
