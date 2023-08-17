#define DEV_I2C "/dev/i2c-1"

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

void detect(char *devpath)
{
    struct i2c_smbus_ioctl_data io;
    int fd = open(devpath, O_RDWR);

    io.read_write = I2C_SMBUS_WRITE;
    io.command = 0;
    io.size = I2C_SMBUS_QUICK;
    io.data = NULL;
    for (int addr = 0x3; addr < 0x78; addr++)
    {
        ioctl(fd, I2C_SLAVE, addr);
        if (ioctl(fd, I2C_SMBUS, &io) >= 0)
        {
            printf("探测到地址:\t0x%x\r\n", addr);
        }
    }
}

int main()
{
    detect(DEV_I2C);
    return 0;
}