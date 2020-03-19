CC = g++
CCFLAGS = -Wall -std=c++11

Assign4: Assign4.o Process.o
	$(CC) $(CCFLAGS) -o Assign4 Assign4.o Process.o
Process.o: Process.cpp
	 $(CC) $(CCFLAGS) -c Process.cpp
Assign4.o: Assign4.cpp
	$(CC) $(CCFLAGS) -c Assign4.cpp
clean:
	-rm *.o Assign4
