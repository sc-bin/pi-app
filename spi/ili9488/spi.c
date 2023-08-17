#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <wiringPi.h>
#include "spi.h"

#define DEV_SPI "/dev/spidev1.1"
static uint32_t SPI_MODE = SPI_MODE_0;
static uint8_t BITS_PER_WORD = 8;
static uint32_t SPEED = 50 * 1000 * 1000;
int fd;
static void LCD_Reset(void)
{
    printf("start reset\r\n");
    digitalWrite(6, HIGH);
    usleep(300000);
    digitalWrite(6, LOW);
    usleep(300000);
    digitalWrite(6, HIGH);
    usleep(300000);
    printf("end reset\r\n");

}
void spi_init()
{
    fd = open(DEV_SPI, O_RDWR);
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
    wiringPiSetup();

    digitalWrite(10, HIGH);
    LCD_Reset();

}
// mosi跟miso同时工作，从tx_buf中取出数据发送的同时，也会读取数据存入rx_buf
int spi_transfer(char *tx_buf, char *rx_buf, int len)
{
    struct spi_ioc_transfer transfer;
    // digitalWrite(10, LOW);
    // printf("buf=0x%x\r\n", tx_buf);
    // printf("len=%d\r\n",len);
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

    digitalWrite(11, HIGH);

    return res;
}
uint8_t tx_buf[10] = {0xd3, 0, 0x00, 0};
uint8_t rx_buf[10];
void LCD_WR_REG(uint8_t Reg)
{
    tx_buf[0] = Reg;
    digitalWrite(5, LOW);
    spi_transfer(tx_buf, rx_buf, 1);
}

void LCD_WR_DATA(uint8_t Data)
{
    tx_buf[0] = Data;
    digitalWrite(5, HIGH);
    spi_transfer(tx_buf, rx_buf, 1);

}

void LCD_WriteReg(uint8_t LCD_Reg, uint8_t LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

void LCD_WRITE_BUF_DATA(uint8_t *buf, int len)
{
    digitalWrite(5, HIGH);

    spi_transfer(buf, buf, len);

}
