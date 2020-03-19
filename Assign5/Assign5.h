/********************************************
CSCI 480 - Assignment 5 - Spring 2018

Progammer: Victor Amaro
Z-ID: z1747708
Section: 1
Date Due: April 6, 2018

Purpose: Contains function prototypes and
	constants for Assign5.cpp
*******************************************/
#ifndef ASSIGN5_H
#define ASSIGN5_H

#include <iostream>
#include <pthread.h>
#include <semaphore.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using std::cout; using std::cerr; using std::endl;

#define P_NUMBER 6       //the number of producers
#define C_NUMBER 4       //the number of consumers
#define BUFFER_SIZE 12   //the maximum size of the buffer
#define N_P_STEPS 4      //the number of iterations for each producer thread
#define N_C_STEPS 6      //the number of iterations for each consumer thread

//Global vars
sem_t NotFull, 			//If not 0, ("true"), then buffer is not full, 
						//so there is space left in the buffer, insert is possible 
						
	NotEmpty;			//If not 0 ("true"), then buffer is not empty,
						//so there is at least one widgetr in buffer, remove possible
pthread_mutex_t mutex;
	
int bufferCounter = 0;	//counter of how many widgets are in the buffer (there aren't actually any widgets or buffer)

void insert(void* );
void remove(void* );
void* produce(void* );
void* consume(void* );

#endif
