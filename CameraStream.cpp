#include "CameraStream.h"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

CameraStream::CameraStream(int port, int fps, int x, int y, int iso)
{
   this->port = port;
   this->fps = fps;
   this->x = x;
   this->y = y;
   this->iso = iso;
};

int CameraStream::start()
{
   char command[256];
   sprintf(command, "mjpg_streamer -i 'input_raspicam.so -fps %d -x %d -y %d -ISO %d' -o 'output_http.so -p %d'", this->fps, this->x, this->y, this->iso, this->port);
   if (system(command) <= 0)
   {
      std::cout << "Error: failed to start stream" << std::endl;
      return -1;
   }
   return 0;
};

int CameraStream::stop()
{
   if (system("sudo killall mjpg_streamer") <= 0)
   {
      return -1;
   }
   return 0;
}