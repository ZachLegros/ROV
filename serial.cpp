#include <stdio.h>		//Standard input/output definitions
#include <string.h>		//String function definitions
#include <unistd.h>		//UNIX standard function definitions
#include <fcntl.h>		//File control definitions
#include <errno.h>		//Error number definitions
#include <termios.h>	//POSIX terminal control definitions
#include <iostream>		//Input-Output Streams

//open_port() - Opens serial port and returns file descriptor on success or -1 on error
int open_port(void){
	int fd;		//File descriptor for the port
	struct termios options;

	fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd == -1){
		//Could not open the port.
		std::cout << "Port Failed to Open";
	}
	else{
		fcntl(fd, F_SETFL, FNDELAY); // Sets the read() function to return NOW and not wait for data to enter buffer if there isn't anything there.

		//Configure port for 8N1 transmission
		tcgetattr(fd, &options);					//Gets the current options for the port
		cfsetispeed(&options, B115200);				//Sets the Input Baud Rate
		cfsetospeed(&options, B115200);				//Sets the Output Baud Rate
		options.c_cflag |= (CLOCAL | CREAD);		//? all these set options for 8N1 serial operations
		options.c_cflag &= ~PARENB;					//? 
		options.c_cflag &= ~CSTOPB;					//?
		options.c_cflag &= ~CSIZE;					//?
		options.c_cflag |= CS8;						//?

		tcsetattr(fd, TCSANOW, &options);			//Set the new options for the port "NOW"

		std::cout << "seems like everything is ok, keep going\n";
	};

	return (fd);
};

int main(void){
	int sPort = -1;
	int written = 0;

	sPort = open_port();

	char *sendThis;

	char something = 'a';

	sendThis = &something;

	if (sPort != -1){
		std::cout << "we're about to write";

		written = write(sPort, sendThis, 1);

		//sPort.write('a', 1);  // A class method can be used to send as well. This write is cleaner and more C++

		if (written < 0 ){
			//Failed to write data to serial port
			std::cout << "Failed to write to port \n";
		};
		
		std::cout << written << " \n" << sendThis;
		//READ stuff here.
	};
	
	close(sPort);

	return 0;
};