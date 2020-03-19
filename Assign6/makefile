CC = g++
CCFLAGS = -Wall -std=c++11

Assign6: Assign6.o MemoryBlock.o
	$(CC) $(CCFLAGS) -o Assign6 Assign6.o MemoryBlock.o
	
MemoryBlock.o: MemoryBlock.cpp
	 $(CC) $(CCFLAGS) -c MemoryBlock.cpp
	 
Assign6.o: Assign6.cpp
	$(CC) $(CCFLAGS) -c Assign6.cpp
	
clean:
	-rm *.o Assign6
