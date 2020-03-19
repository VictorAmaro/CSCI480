/***********************************************************
CSCI 480 - Assignment 1 - Spring 2018

Progammer: Victor Amaro
Z-ID: z1747708
Section: 1
Date Due: Jan 26, 2018

Purpose: assignment involves using LINUX system calls such as 
	fork(), getpid(), getppid(), wait() and system().
************************************************************/

#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

int main () {
	pid_t pid;

	cout << "I am the original process. My PID " << getpid() << " and my parent's PID is " << getppid() << endl;   //Print out original pids

	cout << "Now we have the first fork." << endl;
	
	pid = fork();

	if (pid < 0) {  //fork failed, exit
		cerr << "The first fork failed." << endl;
		exit(-1);
	}
	else if (pid == 0) { //child process
    	cout << "I am the child. My PID " << getpid() << " and my parent's PID is " << getppid() << endl;
    	cout << "Now we have the second fork." << endl;
    	
    	pid = fork();
    	
    	if(pid < 0) { //second fork failed, exit
			cerr << "The second fork failed." << endl;
			exit(-1);
		}
		else if(pid == 0) { //grandchild process (child process of second fork) 
			cout << "I am the grandchild. My PID " << getpid() << " and my parent's PID is " << getppid() << endl;
			cout << "I am the grandchild about to exit." << endl;
			
			exit(0);
		}
		else { //child process (parent process of second fork)
			cout << "I am the child. My PID " << getpid() << " and my parent's PID is " << getppid() << endl;
			
			wait(0);
			
			cout << "I am still the child about to exit." << endl;
			exit(0);
		}	
	}
	else { //parent process
		cout << "I am the parent. My PID " << getpid() << " and my parent's PID is " << getppid() << endl;
		sleep(2);
		
		cout << "I am the parent about to call ps." << endl;
		system("ps");

		wait(0);
		cout << "I am the parent about to exit." << endl << endl;
		
		exit(0);
	}
  
	return 0;
}
