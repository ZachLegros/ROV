
output: ctrlServer.o SPI.o
	g++ ctrlServer.o SPI.o -o ctrlServer

ctrlServer.o: ctrlServer.cpp
	g++ -c ctrlServer.cpp

SPI.o: SPI.cpp SPI.h
	g++ -c SPI.cpp SPI.h

clean: 
	rm *.o ctrlServer