
output: ctrlServer.o SPI.o CameraStream.o
	g++ ctrlServer.o SPI.o CameraStream.o -o ctrlServer

CameraStream.o: CameraStream.cpp CameraStream.h
	g++ -c CameraStream.cpp

ctrlServer.o: ctrlServer.cpp
	g++ -c ctrlServer.cpp

SPI.o: SPI.cpp SPI.h
	g++ -c SPI.cpp

clean: 
	rm *.o ctrlServer