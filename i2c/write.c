#define DEV_I2C "/dev/i2c-1"
#define I2C_ADDR 0X3C

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int fd;

int i2c_write(uint8_t addr, uint8_t reg, uint8_t value)
{

    struct i2c_msg msg;
    uint8_t buf[2];

    buf[0] = reg;
    buf[1] = value;

    msg.addr = addr;
    msg.flags = 0;
    msg.len = 2;
    msg.buf = buf;

    struct i2c_rdwr_ioctl_data data;
    data.msgs = &msg;
    data.nmsgs = 1;

    int ret = ioctl(fd, I2C_RDWR, &data);
    if (ret < 0)
        printf("i2c write failed");
    return ret;
}

int main()
{

    fd = open(DEV_I2C, O_RDWR);
    if (fd < 0)
    {
        printf("节点打开失败\r\n");
        exit(1);
    }
    i2c_write(I2C_ADDR, 0, 0X8D);
    i2c_write(I2C_ADDR, 0, 0X14);
    i2c_write(I2C_ADDR, 0, 0XAF);
    return 0;
}
