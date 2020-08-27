#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <iostream>
#define PORT 8080 
#define BUFFER_SIZE 1024

int server_fd, new_socket, valread; 
struct sockaddr_in address; 
int opt = 1; 
int addrlen = sizeof(address); 
unsigned char buffer[BUFFER_SIZE] = {0}; 
const char *hello = "Hello from server";


void handleConnection(int socket) {
  while (true)
    {
      valread = read( socket , buffer, BUFFER_SIZE); 
      if (valread == -1) {
        close(socket);
        std::cout << "Client disconnected.";
        break;
      }
      std::cout << (int)buffer[0] << std::endl; 
      send(socket , hello , strlen(hello) , 0 ); 
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
      handleConnection(new_socket);
    }

    return 0; 
} 