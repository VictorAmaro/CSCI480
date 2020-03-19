/********************************************
CSCI 480 - Assignment 4 - Spring 2018

Progammer: Victor Amaro
Z-ID: z1747708
Section: 1
Date Due: March 21, 2018

Purpose: Header file for Process.cc
	Contains struct and class definitions
*******************************************/
#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <string.h>
#include <string>
using std::cout; using std::endl; using std::cerr;
using std::string;

#define ARRAY_SIZE 10      //size for history array
extern unsigned int timer; //ticker counter

/****************************
Struct to store burst info
****************************/
struct HistoryStruct {
	unsigned int burstValue;                   //C for CPU burst, I for Input burst, O for Output burst, N for padding
	string burstLetter;                        //Holds the length of that burst
};

class Process {
	public:
		Process(unsigned int inID) : processID(inID) {}  //Set processID for process
			
		string processName;                    //name of process
		unsigned int priority,                 //priority of process, nonnegative, higher values are more important
			arrivalTime = 0;                   //indicates when the request for process first arrived
		int processID;                         //ID for process
		
		HistoryStruct history[ARRAY_SIZE];
		unsigned int sub = 0;		

		unsigned int CPUtimer = 0,            //counts clock ticks til process reaches end of CPU burst 
			IOtimer = 0,                      //counts clock ticks til process reaches end of I/O burst 

			CPUTotal = 0,                     //accumulates number of ticks the process spends as an active CPU burst
			ITotal = 0,                       //accumulates number of ticks the process spends as an active Input burst
			OTotal = 0,                       //accumulates number of ticks the process spends as an active Output burst

			CPUCount = 0,                     //counts the total amount of CPU bursts for this process
			ICount = 0,                       //counts the total amount of Input bursts input burst for this process
			OCount = 0;                       //counts the total amount of Output bursts output burst for this process

		void printInfo();
};
#endif
