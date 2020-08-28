
output: ctrlServer.o
	g++ ctrlServer.o -o ctrlServer

ctrlServer.o: ctrlServer.cpp
	g++ -c ctrlServer.cpp

clean: 
	rm *.o ctrlServer