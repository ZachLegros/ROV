#include "SPI.h"
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <iostream>
#define PORT 8080 
#define BUFFER_SIZE 16
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define SPI_MODE 0
#define BITS_PER_WORDS 8
#define SPEED 25000
#define DELAY 0

int server_fd, new_socket, valread; 
struct sockaddr_in address;
int opt = 1; 
int addrlen = sizeof(address); 
uint8_t ctrl_buffer[BUFFER_SIZE] = {0, }; 
const char *hello = "Hello from server";

void handleConnection(int socket, SPI *com) {
  while (1)
    {
      valread = read(socket, ctrl_buffer, BUFFER_SIZE); 
      if (valread == -1) {
        close(socket);
        std::cout << "Client disconnected.";
        break;
      }

      com->transfer(ctrl_buffer, BUFFER_SIZE);

      // send data back to client
      send(socket, hello, strlen(hello), 0); 
    }
}

int main(int argc, char const *argv[]) 
{ 
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
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    std::cout << "Server started on port " << PORT << std::endl;

    while (true)
    {
      if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                        (socklen_t*)&addrlen))<0) 
      { 
          perror("Accept failed."); 
          continue;
      } 
      std::cout << "Connected to client.\n";

      SPI *spi_ptr;
      SPI com("/dev/spidev0.0", SPI_MODE, BITS_PER_WORDS, SPEED, DELAY);
      spi_ptr = &com;
      handleConnection(new_socket, spi_ptr);
    }

    return 0; 
} 