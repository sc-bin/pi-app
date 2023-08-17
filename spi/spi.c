#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define DEV_SPI "/dev/spidev1.1"
static uint32_t SPI_MODE = SPI_MODE_0;
static uint8_t BITS_PER_WORD = 8;
static uint32_t SPEED = 10*1000*1000;

void spi_init()
{
    int fd = open(DEV_SPI, O_RDWR);
    if (fd < 0)
    {
        printf("err: spidev打不开\r\n");
        exit(1);
    }

    int ret = ioctl(fd, SPI_IOC_WR_MODE32, &SPI_MODE);
    if (ret == -1)
        printf("err: can't set spi mode");
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &BITS_PER_WORD);
    if (ret == -1)
        printf("err: can't set bits per word");
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &SPEED);
    if (ret == -1)
        printf("err: can't set max speed hz");

    close(fd);
}
//mosi跟miso同时工作，从tx_buf中取出数据发送的同时，也会读取数据存入rx_buf
int spi_transfer(char *tx_buf, char *rx_buf, int len)
{
    struct spi_ioc_transfer transfer;

    int fd = open(DEV_SPI, O_RDWR);
    if (fd < 0)
    {
        printf("spidev打不开\r\n");
        exit(1);
    }

    transfer.tx_buf = (unsigned long)tx_buf;
    transfer.rx_buf = (unsigned long)rx_buf;
    transfer.len = len;
    transfer.delay_usecs = 1;
    transfer.speed_hz = SPEED;
    transfer.bits_per_word = BITS_PER_WORD;
    transfer.tx_nbits = 1;
    transfer.rx_nbits = 1;

    int res = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
    if (res < 0)
        printf("err: spi_transfer failed");
    close(fd);
    return res;
}
int main()
{

    unsigned char tx_buf[10] = {0x12, 0x34, 0x00, 0xff};
    unsigned char rx_buf[10];

    spi_init();
    // while(1)
    spi_transfer(tx_buf, rx_buf, sizeof(rx_buf));


    printf("Received data: ");
    for (int i = 0; i < sizeof(rx_buf); i++)
    {
        printf("0x%02x ", rx_buf[i]);
    }
    printf("\n");

    return 0;
}
