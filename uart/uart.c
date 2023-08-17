#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>

#define DEV_UART "/dev/ttyS2"

void uart_init(char *path)
{

    int fd = open(path, O_RDWR);
    if (fd < 0)
    {
        printf("Fail to Open\n");
        return;
    }

    struct termios opt;

    // 清空串口接收缓冲区
    tcflush(fd, TCIOFLUSH);
    // 获取串口参数opt
    tcgetattr(fd, &opt);

    // 设置串口输出波特率
    cfsetospeed(&opt, B115200);
    // 设置串口输入波特率
    cfsetispeed(&opt, B115200);
    // 设置数据位数
    opt.c_cflag &= ~CSIZE;
    opt.c_cflag |= CS8;
    // 校验位
    opt.c_cflag &= ~PARENB;
    //    opt.c_iflag &= ~INPCK;
    // 设置停止位
    opt.c_cflag &= ~CSTOPB;

    // 更新配置
    tcsetattr(fd, TCSANOW, &opt);
    close(fd);
}

int main(int argc, char *argv[])
{
    char buf[1024] = "hello";

    uart_init(DEV_UART);

    int fd = open(DEV_UART, O_RDWR);
    if (fd < 0)
    {
        printf("Fail to Open\n");
        return 0;
    }

    write(fd, buf, strlen(buf));

    int res = read(fd, buf, 10); // 阻塞接收，直到遇到回车才返回
    if (res > 0)
    {
        // 给接收到的字符串加结束符
        buf[res] = '\0';
        printf("Receive res = %d bytes data: %s\n", res, buf);
    }

    return 0;
}