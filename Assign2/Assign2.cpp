/***********************************************************
CSCI 480 - Assignment 2 - Spring 2018

Progammer: Victor Amaro
Z-ID: z1747708
Section: 1
Date Due: Feb 7, 2018

Purpose: involves using the LINUX system function pipe() as well as fork().
	have three processes which communicate with each other using pipes
************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

#define BUFFER_SIZE 10
#define READ_END 0
#define WRITE_END 1
#define PIPE_ARRAY_SIZE 2

//Global vars
char READ_MSG[BUFFER_SIZE];   //Holds the message to be read from
char WRITE_MSG[BUFFER_SIZE];  //Holds the message to write to
int M;  

//Function prototypes
void const PWork(int (&_pipeC)[2], int (&_pipeA)[2]);
void const CWork(int (&_pipeA)[2], int (&_pipeB)[2]);
void const GWork(int (&_pipeB)[2], int (&_pipeC)[2]);
void const workLoop(int (&readFromPipe)[2], int (&writeToPipe)[2], const int processNum);

int main() {
	strcpy(READ_MSG, "1");         //load 1 into the READ_MSG
	M = 1;
	
    int pipeA[PIPE_ARRAY_SIZE],    //3 pipe variables to use
		pipeB[PIPE_ARRAY_SIZE], 
		pipeC[PIPE_ARRAY_SIZE];

    if(pipe(pipeA) == -1) {              //pipeA has failed, exit program              
        cerr << "Pipe #1  failed." << endl; 
        return -5;
    }
    
    if(pipe(pipeB) == -1) {              //pipeB has failed, exit program              
        cerr << "Pipe #2  failed." << endl; 
        return -5;
    }
    if(pipe(pipeC) == -1) {              //pipeC has failed, exit program              
        cerr << "Pipe #3  failed." << endl; 
        return -5;
    }
    
    pid_t pid = fork();                 //First fork
                  
    if(pid < 0) {                       //First fork has failed
        cerr << "Fork #1 failed." << endl;
        return -5;
    }
    else if(pid == 0) {                 //child process
		pid = fork();                   //Second fork
		
		if(pid < 0) {                   //Second fork has failed
        	cerr << "Fork #2 failed." << endl;
        	return -5;
    	}
    	else if(pid == 0) {             //grandchild process
    		close(pipeB[WRITE_END]);    //close ends of pipes that arent needed
			close(pipeC[READ_END]);
		
			GWork(pipeB, pipeC);

			close(pipeB[READ_END]);      //close ends of pipes 
			close(pipeC[WRITE_END]);
			
			exit(0);                     //Exit grandchild process
		}
    	else {                           //child process
    		close(pipeA[WRITE_END]);    //close ends of pipes that arent needed
			close(pipeB[READ_END]);
		
			CWork(pipeA, pipeB);

			close(pipeA[READ_END]);     //close ends of pipes 
			close(pipeB[WRITE_END]);
			
			wait(NULL);                 //Wait for grandchild process to end
			exit(0);                   //Exit child process
		}
    }
    else {                             //parents process 
		close(pipeC[WRITE_END]);       //close ends of pipes that arent needed
		close(pipeA[READ_END]);
		
		PWork(pipeC, pipeA);

		close(pipeC[READ_END]);         //close ends of pipes 
		close(pipeA[WRITE_END]);

		wait(NULL);                     //wait for child process to finish
		exit(0);                       //exit parent process
    }
    return 0;
}


/*********************************************
void const PWork(int (&)[], int (&)[]
Returns: Nothing
Arguments: A reference to a pipe called, _pipeC. This pipe is the one from which the message will be read from
	A reference to a pipe called, _pipeA. This pipe is the one from which the message will be written to
Use: Writes the READ_MSG to the corresponding pipe (to _pipeA)
	Calls to work loop and passes the pipes with the correct process number (see workLoop)
**********************************************/
void const PWork(int (&_pipeC)[2], int (&_pipeA)[2]) {
	write(_pipeA[WRITE_END], READ_MSG, strlen(READ_MSG)+1);    //Write message (READ_MSG) to pipe (pipeA)

	cout << "Parent: " << setw(20) << "M = ";                 //Display "Parent" with contents of READ_MSG
	
	for(unsigned i = 0; i < strlen(READ_MSG); i++) {          //Print out contents of READ_MSG
		cout << READ_MSG[i];
	}
	cout << endl;   
 
	workLoop(_pipeC, _pipeA, 2);
}

/*********************************************
void const GWork(int (&)[], int (&)[]
Returns: Nothing
Arguments: A reference to a pipe called, _pipeB. This pipe is the one from which the message will be read from
	A reference to a pipe called, _pipeC. This pipe is the one from which the message will be written to
Use: Calls to work loop and passes the pipes with the correct process number (see workLoop)
**********************************************/
void const GWork(int (&_pipeB)[2], int (&_pipeC)[2]) {	
	workLoop(_pipeB, _pipeC, 1);
}

/*********************************************
void const CWork(int (&)[], int (&)[]
Returns: Nothing
Arguments: A reference to a pipe called, _pipeA. This pipe is the one from which the message will be read from
	A reference to a pipe called, _pipeB. This pipe is the one from which the message will be written to
Use: Calls to work loop and passes the pipes with the correct process number (see workLoop)
**********************************************/
void const CWork(int (&_pipeA)[2], int (&_pipeB)[2]) {
	workLoop(_pipeA, _pipeB, 0);
}

/*********************************************
void const workLoop(int (&)[], int (&)[], const int)
Returns: Nothing
Arguments: A reference to a pipe called, readFromPipe. This pipe is the one from which the message will be read from
	A reference to a pipe called, writeToPipe. This pipe is the one from which the message will be written to
	A constant int, called processNum. This will decide which message is printed.
		0 -> Child Message
		1 -> Grandchild Message
		2 -> Parents Message 
**********************************************/
void const workLoop(int (&readFromPipe)[2], int (&writeToPipe)[2], const int processNum) {
	while(M < 9999999) {
		read(readFromPipe[READ_END], READ_MSG, BUFFER_SIZE);
		
		M = atoi(READ_MSG);    //convert to int
		
		M = ((3 * M) + 1);     //do math calculation
		
		string tempMsg = to_string(M);     //convert to string
		
		write(writeToPipe[WRITE_END], strcpy(WRITE_MSG, tempMsg.c_str()), tempMsg.length()+1);	
		
		switch(processNum) {
			case 0:
				cout << "Child: " << setw(21) << "M = " << M << endl;           //Display "child" with contents of READ_MSG
				break;
			case 1:
				cout << "Grandchild: " << setw(16) << "M = " << M << endl;     //Display "grandchild" with contents of READ_MSG
				break;
			case 2:
				cout << "Parent: " << setw(20) << "M = " << M << endl;         //Display "Parent" with contents of READ_MSG
				break;
			default:
				cerr << "Invalid number! (0 for Child process, 1 for Grandchild Process, 2 for Parent Process)" << endl;
		}	
	}	
}
