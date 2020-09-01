#include "SPI.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
	perror(s);
	abort();
}

SPI::SPI()
{
	SPI("/dev/spidev0.0", 0, 8, 1000, 0);
}

SPI::SPI(const char* device, uint8_t mode, uint8_t bits, uint32_t speed, uint16_t delay)
{
	this->device = device;
	this->mode = mode;
	this->bits = bits;
	this->speed = speed;
	this->delay = delay;

	int ret = 0;
	int fd;
	
	fd = open(this->device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

	this->fd = fd;
}

void SPI::printBuffer(uint8_t buffer[])
{
	for (int i = 0; i < ARRAY_SIZE(buffer); i++) {
		std::cout << (int)buffer[i] << " ";
	}
	puts(""); 
}

void SPI::transfer(uint8_t tx[])
{
	int ret;
	// uint8_t tx[] = {
  //       0xff, 0x01, 0x00, 0xbe, 0x01,
  //       0x00, 0x00, 0x00, 0x00, 0x00,
	// 			0x00, 0x00, 0x00, 0x00, 0x00,
  //       0x0A 
	// };

	uint8_t rx[ARRAY_SIZE(tx)] = {0, };

	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = ARRAY_SIZE(tx),
		.speed_hz = speed,
		.delay_usecs = delay,
		.bits_per_word = bits,
	};
	
	printBuffer(tx);
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
	puts("");
	printBuffer(rx);
	// do stuff with rx
	
}

void SPI::close_SPI()
{
	close(this->fd);
}

