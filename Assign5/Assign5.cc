/********************************************
CSCI 480 - Assignment 5 - Spring 2018

Progammer: Victor Amaro
Z-ID: z1747708
Section: 1
Date Due: April 6, 2018

Purpose: use POSIX threads, semaphores and a mutex
	to illustrate the Producer-Consumer Problem.
*******************************************/
#include "Assign5.h"

int main() {	
	cout << endl << "Simulation of Producer and Consumers" << endl << endl;
	
	int notFullVal = sem_init(&NotFull, 0, BUFFER_SIZE);			//create the NotFull semaphore
	if(notFullVal) {												//Failed: display message and exit
		cerr << endl << "Failed: NotFull semaphore creation" << endl;
		exit(-1);
	}
	
	int notEmptyVal = sem_init(&NotEmpty, 0, 0);					//create the NotEmpty semaphore
	if(notEmptyVal) {												//Failed: display message and exit
		cerr << endl << "Failed: NotEmpty semaphore creation" << endl;
		exit(-1);
	}

	int mutexVal = pthread_mutex_init(&mutex, NULL);				//create the mutex
	if(mutexVal) {													//Failed: display message and exit
		cerr << endl << "Failed: Mutex lock creation" << endl;
		exit(-1);
	}


	cout << "The semaphores and mutex have been initialized." << endl << endl;


	pthread_t producerThread[P_NUMBER];								//initialize arrays 
	pthread_t consumerThread[C_NUMBER];								//makes n number of threads for producers and consumers

	int consumerThreadVal = 0;										//creating n of producer threads.
	for(long i = 0; i < C_NUMBER; i++) {
		consumerThreadVal = pthread_create(&consumerThread[i], NULL, consume, (void*) i);
		if(consumerThreadVal) {										//couldn't create a thread, exit
			cerr << endl << "Failed: Creating consumer threads" << endl;
			exit(-1);
		} 
	}

	int producerThreadVal = 0;										//creating n of producer threads
	for(long i =0 ;i < P_NUMBER; i++) {
		producerThreadVal = pthread_create(&producerThread[i], NULL, produce, (void*) i);
		if(producerThreadVal) {										//couldn't create a thread, exit
			cerr << endl << "Failed: Creating producer threads" << endl;
			exit(-1);
		}
	}

	int producerThreadJoinVal = 0;
	for (int i = 0; i < P_NUMBER; i++) {								//cycle thru all threads
		producerThreadJoinVal = pthread_join(producerThread[i], NULL);	//suspend execution of the calling thread until the target thread terminates
		
		if(producerThreadJoinVal) {										//Failed: display message and exit
			cerr << "Failed: Producer couldn't join thread" << endl;
			exit(-1);
		}
	}
	
	int consumerThreadJoinVal = 0;
	for (int i = 0; i < C_NUMBER; i++) {								//cycle thru all threads
		consumerThreadJoinVal = pthread_join(consumerThread[i], NULL);	//suspend execution of the calling thread until the target thread terminates
		
		if(consumerThreadJoinVal) {										//Failed: display message and exit
			cerr << "Failed: Consumer couldn't join thread" << endl;
			exit(-1);
		}
	}
	
	cout << endl << "All the producer and consumer threads have been closed." << endl;

	sem_destroy(&NotFull);										//destroy semaphores and mutex
	sem_destroy(&NotEmpty);	
	pthread_mutex_destroy(&mutex);
	
	cout << endl << "The semaphores and mutex have been deleted." << endl << endl;
	
	return 0;
}


/*******************************************
produce(void* )
Arguments: a variable, called producerThreadID,
				passed into insert
Returns: Nothing
Use: called by the producer thread
*******************************************/
void* produce(void* producerThreadID) {
	for(int i = 0; i < N_P_STEPS; i++) {	//cycle thru threads
		sem_wait(&NotFull);					//if buffer is at capacity, then wait
		
		insert(producerThreadID);			//buffer not at capacity, insert item
		
		sem_post(&NotEmpty);				//change status of NotEmpty, meaning buffer is not empty
		
		sleep(1);							//delay 
	}
	
	pthread_exit(NULL);						//terminate thread
}

/*******************************************
consume(void* )
Arguments: a variable, called consumerThreadID,
				passed into remove
Returns: Nothing
Use: called by the consumer thread
*******************************************/
void* consume(void* consumerThreadID) {
	for(int i = 0; i < N_C_STEPS; i++) {	//cycle thru threads
		sem_wait(&NotEmpty);				//if buffer is empty, then wait
		
		remove(consumerThreadID);			//buffer is not empty, remove one item
		
		sem_post(&NotFull);					//change status of NotFull, meaning buffer is not full
		
		sleep(1);							//delay 
	}
	
	pthread_exit(NULL);						//terminate thread
}

/*******************************************
insert(void* )
Arguments: a variable (casted as a long), called producerThreadID,
				used to signify which producer inserted an item
Returns: Nothing
Use: is called to add one item to the buffer,
	and display who added it and the buffer count
*******************************************/
void insert(void* producerThreadID ){
	pthread_mutex_lock(&mutex);		//locking the mutex
	
	bufferCounter++;				//one item added to buffer
	
	cout << "Producer " << (long)producerThreadID << " inserted one item.  Total is now " << bufferCounter << endl;
	
	pthread_mutex_unlock(&mutex);	//unlocking the mutex
}

/*******************************************
remove(void* )
Arguments: a variable (casted as a long), called consumerThreadID,
				used to signify which producer removed an item
Returns: Nothing
Use: is called to remove one item from the buffer,
	and display who removed it and the buffer count
*******************************************/
void remove(void* consumerThreadID) {
	pthread_mutex_lock(&mutex);		//locking the mutex
	
	bufferCounter--;				//one item removed from buffer
	
	cout << "Consumer " << (long)consumerThreadID << " inserted one item.  Total is now " << bufferCounter << endl;
	
	pthread_mutex_unlock(&mutex);	//unlocking the mutex
}
