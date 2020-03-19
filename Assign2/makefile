CC = g++
CCFLAGS = -Wall -g

Assign2: Assign2.o
	$(CC) -o Assign2 $(CCFLAGS) Assign2.o

Assign2.o: Assign2.cpp
	$(CC) -c $(CCFLAGS) Assign2.cpp

run:
	./Assign2

clean:
	-rm Assign2.o Assign2
