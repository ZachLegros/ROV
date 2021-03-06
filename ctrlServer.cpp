#include "SPI.h"
#include "CameraStream.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <csignal>

#define PORT 8080
#define BUFFER_SIZE 16
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define SPI_MODE 0
#define BITS_PER_WORDS 8
#define SPEED 500
#define DELAY 0
#define CAM_PORT 3000
#define CAM_FPS 30
#define CAM_X 1080
#define CAM_Y 720
#define CAM_ISO 3200

bool client_connected = false;
int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
uint8_t ctrl_buffer[BUFFER_SIZE] = {
    0,
};

void exiting(int i)
{
  std::cout << "aborting" << std::endl;
  CameraStream::stop();
  exit(0);
}

void start_stream()
{
  CameraStream::stop();
  CameraStream raspi_cam(CAM_PORT, CAM_FPS, CAM_X, CAM_Y, CAM_ISO);
  raspi_cam.start();
}

void client_thread(int socket)
{
  std::cout << "Connected to client.\n";

  SPI com("/dev/spidev0.0", SPI_MODE, BITS_PER_WORDS, SPEED, DELAY);

  while (1)
  {
    valread = read(socket, ctrl_buffer, BUFFER_SIZE);
    if (valread <= 0)
    {
      close(socket);
      std::cout << "\nClient disconnected.\n";
      break;
    }

    uint8_t rx[BUFFER_SIZE] = {
        0,
    };
    com.transfer(ctrl_buffer, rx, BUFFER_SIZE);
    SPI::printBytesBuffer(rx, BUFFER_SIZE);

    // send data back to client
    send(socket, rx, BUFFER_SIZE, 0);
  }
  com.close_SPI();
}

int main(int argc, char const *argv[])
{
  //^C
  signal(SIGINT, exiting);
  //sent by "kill" command
  signal(SIGTERM, exiting);
  //^Z
  signal(SIGTSTP, exiting);

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                 &opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address,
           sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  std::thread stream(start_stream);
  stream.detach();

  std::cout << "Server started on port " << PORT << std::endl;
  while (1)
  {
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
      perror("Accept failed.");
      continue;
    }
    std::thread client(client_thread, new_socket);
    client.join();
  }

  return 0;
}