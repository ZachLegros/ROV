#ifndef CAM_STREAM
#define CAM_STREAM
#include <iostream>

class CameraStream
{
private:
   int port;
   int fps;
   int x;
   int y;
   int iso;

public:
   CameraStream(int port, int fps, int x, int y, int iso);
   int start();
   static int stop();
};

#endif