/***********************************************************
CSCI 480 - Assignment 3 - Spring 2018

Progammer: Victor Amaro
Z-ID: z1747708
Section: 1
Date Due: Feb 23, 2018

Purpose: implement a microshell. 
	practice with system calls such as fork(), pipe(), and dup()
************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <sys/utsname.h>
#include <unistd.h>
using namespace std;

#define BUFFER_SIZE 1024
#define PIPE_SIZE 2

void firstCommandWork(int _pipe[], char *command[], const bool pipeExists);
void secondCommandWork(int _pipe[], char *command2[]);

int main(void) {
	char *userCommand[BUFFER_SIZE] = {NULL, NULL, NULL},   //char array ptrs that will hold the user desired command
		*userCommand2[BUFFER_SIZE] = {NULL, NULL, NULL},   
		buffer[BUFFER_SIZE];  //buffer that will hold user input
	int status,               //status of the child process in it's parents process
		status2;
	int mainPipe[PIPE_SIZE];  //pipe variable
  	pid_t fork1,             //fork variables
		fork2;

	cout << "480shell>";     //shell header

	while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {     //continue to get input from user until none is entered
		bool userWantsPipe = false;      //variable that will be set to true if the user wishes to use pipe command

	    buffer[strlen(buffer) -1] = 0;   //remove last character in buffer

	    int tokenIndex = 0;              //used to seperate tokens
	    char *bufferPtr;                 //char array to hold saved token made by strok
	    bufferPtr = strtok(buffer, " "); //gets first command from buffer up to a white space and saves it in bufferPtr
	
		while(bufferPtr != NULL) {       //continue as long as there's something in the bufferPtr
	    	if( (strcmp(bufferPtr, "q") == 0) || (strcmp(bufferPtr, "quit") == 0) ) {      //if user entered "q" or "quit, exit program
	    		cout << "Exiting.." << endl << endl;
	        	return 0;
	      	}
	    	if(strcmp(bufferPtr, "||") == 0) {     //if user entered "||" (meaning they wish to use pipe)
	        	userWantsPipe = true;              //set userWantsPipe to true
                tokenIndex = 0;                   
	        	bufferPtr = strtok(NULL, " "); 
	      	}
	      	if(!userWantsPipe) {                      //if user has not entered "||" (meaning they do not wish to pipe)
	        	userCommand[tokenIndex] = bufferPtr;  //grab first command (in the bufferPtr) and store it in the first user command variable @ tokenIndex (0)
	        	bufferPtr = strtok(NULL, " ");
	        	tokenIndex++;                         //add one to tokenIndex, to represent one command already gotten
	      	}
			else {                                    //user has entered "||" (meaning they wish to pipe)
	        	userCommand2[tokenIndex] = bufferPtr; //grab first command (in the bufferPtr) and store it in the first user command variable @ tokenIndex (0)
	        	bufferPtr = strtok(NULL, " ");
	        	tokenIndex++;                         //add one to tokenIndex, to represent one command already gotten
	        	int val = pipe(mainPipe);             //create pipe from pipe variable
	        	
				if(val == -1) {                     
        			cout << "Failed: Pipe. Exiting..." << endl; 
				}
	      	}
	    }
	
	    fork1 = fork();    //create first fork
	    if(fork1 < 0) {   //first fork has failed, exit program
	    	cerr << "Failed: First Fork. Exiting..." << endl;
	    	return -1;
	    }
	    if(fork1 == 0) {   //child process of first fork
	    	firstCommandWork(mainPipe, userCommand, userWantsPipe);
	    }

	    if(userWantsPipe) {      //if the user used the pipe command
			fork2 = fork();      //create second fork
	    	if(fork2 == 0) {     //child process of second fork
	    		secondCommandWork(mainPipe, userCommand2);
	    	}
	    	if(fork2 < 0) {      //second fork has failed, exit program
	        	cerr << "Failed: Second Fork. Exiting..." << endl << endl;
	        	return -1;
	      	}
	    }
	
	    if(userWantsPipe) {        //if the user used the pipe command
	    	close(mainPipe[0]);    //close ends of the pipe
	    	close(mainPipe[1]);
	    }
	
	    fork1 = waitpid(fork1, &status, 0);     //wait for first child to execute
	    if(fork1 < 0) {                         //waiting on first child to finish has failed, exit program
	    	cerr << "Failed: Waiting First Child. Exiting..." << endl;
	    	return -1;
		}
	
	    if(userWantsPipe) {                         //if the user used the pipe command
	    	fork2 = waitpid(fork2, &status2, 0);    //wait for second child to execute
	    	if (fork2 < 0) {                        //waiting on second child to finish has failed, exit program
	    		cerr << "Failed: Waiting Second Child. Exiting..." << endl << endl;
	        	return -1;
	    	}
	    }
	    cout << "480shell>";                        //shell header
	  }
	  return 0;
} 

/*********************************************************
void const firstCommandWork(int [], char* [], const bool)
returns: nothing
arguments: 1) a int array representing a pipe (_pipe)
	2) a ptr to a char array (command), holds the commands entered by the user
	3) a constant bool (pipeExists), holds true if user used pipe command
		otherwise holds false
use: used to execute the first command entered by user
	first, if the user wanted to use the pipe command (pipeExists will be true in this case)
		we will first call dup2() on the pipe (_pipe) to create copy of the file descriptor
		then we will close the end of the pipe (_pipe)	
	second, then we will execute the command @ location 0 (this will be the first command) in command array
		if the command has failed, display a message, exit with -1
*********************************************************/
void firstCommandWork(int _pipe[], char *command[], const bool pipeExists) {
	if(pipeExists) {
		int val2 = dup2(_pipe[1], 1);
		
		if(val2 < 0) {
			cerr << "Error:" << endl;
			exit(-1);
		}
		close(_pipe[0]);                    //close pipe
	}

	int val = execvp(command[0], command); //store value of whether or not command could execute
	
	if(val < 0) {                          //couldn't execute command, display message, exit with a value of -1
		cerr << "Error: Could Not Execute First Command!" << endl;
		exit(-1);
	}
}

/*********************************************************
void const secondCommandWork(int [], char* [])
returns: nothing
arguments: 1) a int array representing a pipe (_pipe)
	2) a ptr to a char array (command2), holds the commands entered by the user
use: used to execute the second command entered by user 
	first, we will first call dup2() on the pipe (_pipe) to create copy of the file descriptor
		then we will close the end of the pipe (_pipe)	
	second, then we will execute the command @ location 0 (this will be the second command) in command2 array
		if the command has failed, display a message, exit with -1
*********************************************************/
void secondCommandWork(int _pipe[], char *command2[]) {
	int val2 = dup2(_pipe[0], 0);
	
	if(val2 < 0) {
		cerr << "Error:" << endl;
		exit(-1);
	}
	
	close(_pipe[1]);                         //close pipe
	
	int val = execvp(command2[0], command2); //store value of whether or not command could execute
	
	if(val < 0) {                            //couldn't execute command, display message, exit with a value of -1
		cerr << "Error: Could Not Execute Second Command!" << endl;
		exit(-1);
	}
}
