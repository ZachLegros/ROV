#ifndef CAM_STREAM
#define CAM_STREAM

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
   int stop();
};

#endif