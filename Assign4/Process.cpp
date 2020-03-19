/********************************************
CSCI 480 - Assignment 4 - Spring 2018

Progammer: Victor Amaro
Z-ID: z1747708
Section: 1
Date Due: March 21, 2018

Purpose: Contains method(s) 
*******************************************/
#include "Process.h"

/*******************************
Method: printInfo()
Arguments: None
Returns: None
Use: 1) Prints out process info
*******************************/
void Process::printInfo() {
	cout << endl <<"Name: " << this->processName << endl
		<< "ID: "<< this->processID << endl
		<< "Priority: "<< this->priority << endl
		<< "Start time: " << this->arrivalTime << endl 
		<< "End time: " << timer << endl
		<< "Time spent Active: " << this->CPUTotal << " in " << this->CPUCount << " CPU bursts." << endl
		<< "Time spent IActive: " << this->ITotal << " in " << this->ICount << " Input bursts." << endl
		<< "Time spent OActive: " << this->OTotal << " in " << this->OCount << " Output bursts." << endl
		<< "Time spent waiting: " << (timer - this->arrivalTime - this->CPUTotal - this->OTotal - this->ITotal) << endl;  //calculation to figure out how much was idle time
}
