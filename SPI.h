#ifndef SPI_H
#define SPI_H
#include <stdint.h>

static void pabort(const char *s);

class SPI
{
  private:
    int fd;
    const char *device;
    uint8_t mode;
    uint8_t bits;
    uint32_t speed;
    uint16_t delay;

  public:
    SPI();
    SPI(const char* device, uint8_t mode, uint8_t bits, uint32_t speed, uint16_t delay);
    void transfer(uint8_t tx[]);
    void close_SPI();
    void printBuffer(uint8_t buffer[]);
};

#endif