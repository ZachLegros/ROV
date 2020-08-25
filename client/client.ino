#define BUFFER_SIZE 4

int incomingByte = 0; // for incoming serial data
int motorSpeedA = 0;
int buffer[BUFFER_SIZE] = {0, 0, 0, 0};
int index = 0;

void printBuffer(int buffer[])
{
  for (int i=0; i<BUFFER_SIZE; i++)
  {
    Serial.print(buffer[i]);
    Serial.print(" ");
  }
  Serial.println();
}


void getBuffer()
{
  bool bufferFull = false;
  
  while (!bufferFull)
  {
    // read the incoming byte:
    incomingByte = int(Serial.read());
    if (incomingByte == 1)
    {
      index = 0;
      bufferFull = true;
    }
    else if (incomingByte > -1 && incomingByte < 256)
    {
      buffer[index] = incomingByte;
      index++;
    }
  }
}


void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

void loop() {
  getBuffer();
  
  printBuffer(buffer);
}

