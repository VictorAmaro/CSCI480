CC = g++
CCFLAGS = -Wall -pthread -std=c++11 -g

Assign5 : Assign5.o
	$(CC) $(CCFLAGS) -o Assign5 Assign5.o 

Assign5.o : Assign5.cc Assign5.h
	$(CC) $(CCFLAGS) -c Assign5.cc

clean :
	-rm *.o Assign5
