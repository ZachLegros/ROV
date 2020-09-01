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
	this->fd = fd;
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
}

void SPI::printBytesBuffer(uint8_t buffer[], int len)
{
	for (int i = 0; i < len; i++) {
		std::cout << (int)buffer[i] << " ";
	}
	std::cout << std::endl;
}

void SPI::transfer(uint8_t tx[], int len)
{
	int ret;
	
	uint8_t rx[len] = {0, };

	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = (unsigned int)len,
		.speed_hz = this->speed,
		.delay_usecs = this->delay,
		.bits_per_word = this->bits,
	};

	ret = ioctl(this->fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
		puts("");

	printBytesBuffer(rx, len);
	// do stuff with rx
	
}

void SPI::close_SPI()
{
	close(this->fd);
}

