#include <SPI.h>
#define RATE 500000
#define MISO 50
#define MOSI 51
#define SCK 52
#define SS 53
#define BUFFER_SIZE 10

unsigned char buffer[BUFFER_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
unsigned char recievedVal;

void setup() {
  Serial.begin(9600);
  // Arduino Mega as master
  SPI.begin();
  SPI.beginTransaction(SPISettings(RATE, MSBFIRST, SPI_MODE0));
  digitalWrite(SS, LOW);
}

void loop() {
  for (int i=0; i<BUFFER_SIZE; i++)
  {
    recievedVal = SPI.transfer(buffer[i]);
    buffer[i] = recievedVal;  
    Serial.print(buffer[i]);
  }
  Serial.println();
}
