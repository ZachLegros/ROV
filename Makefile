
output: controllerServer.o
	g++ controllerServer.o -o controllerServer

controllerServer.o: controllerServer.cpp
	g++ -c controllerServer.cpp

clean: 
	rm *.o controllerServer