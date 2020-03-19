/********************************************
CSCI 480 - Assignment 4 - Spring 2018

Progammer: Victor Amaro
Z-ID: z1747708
Section: 1
Date Due: March 21, 2018

Purpose: Contains class used to compare stuff
	Some function prototypes
	Some global vars
*******************************************/
#ifndef ASSIGN4_H
#define ASSIGN4_H

#define MAX_TIME 500                      //length of whole run
#define AT_ONCE 5                         //max num of processes that can be in play at once
#define QUEUE_SIZE 20                     //larger than the max num of items any queue will hold
#define HOW_OFTEN 25                      //how often to print state of system

#include "Process.h"
#include <queue>
#include <fstream>
using std::queue; using std::priority_queue; using std::vector;
using std::ifstream;

/*************************************************************
Comparator Class
Used to compare objects in priority queues by process priority 
	(the higher the more imporant) 
*************************************************************/
class Compare {
	public:
    	bool operator() (Process* lhs, Process* rhs) {
        	return lhs->priority < rhs->priority;
    	}
};

bool condition(int );

void printProcessContents();
void printQueue(queue<Process*>);
void printPriorityQueue(priority_queue<Process *, vector<Process *>, Compare> );
void printReport();

void moveToQueue(Process*& , string , bool );
void completeBurst(Process*& , HistoryStruct , unsigned int& , int& ,
					bool );
void makeProcessActive(Process*& , priority_queue<Process*, vector<Process*>, Compare>& );
void terminateProcess(Process*& );

void fileReading();
void setProcess(Process* , const char* );
void setProcessHistory(Process* , const char* );

HistoryStruct getProcessHistory(Process* );

Process *Active = nullptr;   //ptrs to Process objects, will hold the active burst Process
Process *IActive = nullptr;
Process *OActive = nullptr; 

queue<Process *> entryQueue; //queues to be used to hold Processes
priority_queue<Process *, vector<Process *>, Compare> readyQueue;  
priority_queue<Process *, vector<Process *>, Compare> inputQueue;
priority_queue<Process *, vector<Process *>, Compare> outputQueue;

int endProcessCnt = 0;      //count of Processes that have ended
unsigned int timer = 0;     //tick counter
#endif
