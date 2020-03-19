CC = g++
CCFLAGS = -Wall -g

Assign1: Assign1.o
	$(CC) -o Assign1 $(CCFLAGS) Assign1.o

Assign1.o: Assign1.cpp
	$(CC) -c $(CCFLAGS) Assign1.cpp

run:
	./Assign1

clean:
	-rm Assign1.o Assign1 
