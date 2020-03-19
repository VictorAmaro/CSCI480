/********************************************
CSCI 480 - Assignment 4 - Spring 2018

Progammer: Victor Amaro
Z-ID: z1747708
Section: 1
Date Due: March 21, 2018

Purpose: we are simulating priority scheduling of processes on a single-processor system, without preemption. 
	The idea is that when a process ends its CPU burst (or input burst or output burst), 
	it is succeeded by the highest-priority process that is waiting.
*******************************************/
#include "Assign4.h"

int main() {
    fileReading();               //calls function to open file (see fileReading() function for description on what this does)

    int processesInUse = 0,     //count of how many process are in use (only 5 at a time)
    	idleTimeTotal = 0;      //total idle time count

    while(condition(timer)) {               //while MAX_TIME not reached, there still are active processes, queues not empty
        while(processesInUse < AT_ONCE) {   //while processInUse isnt 5 (there should be 5 processes in readyQueue)
            if(entryQueue.empty()) {        //no more process left ot bring to readyQueue, leave
				break;
			}

            Process* processToMove = entryQueue.front();  //grab front Process object of entryQueue

            if(processToMove->arrivalTime > timer) {      //if Process' arrival time has not been met, break
                break;
            }
            
            entryQueue.pop();                             //arrival time met, remove from entryQueue
            
            cout << endl << "Process " << processToMove->processName << " moved from the Entry to Ready at time: " << timer << endl;

            processToMove->arrivalTime = timer;           //set arrival time of Process

            readyQueue.push(processToMove);               //push this Process into readyQueue
            
            processesInUse++;                             //a process added to readyQueue, add one
        }

        if (timer % HOW_OFTEN == 0) {                     //print status every 25 ticks
            printReport();
        }

        makeProcessActive(Active, readyQueue);            //make a inactive process go active (see function details)

        if (Active == nullptr) {                          //no active process = idle time, increase idle time counter
            idleTimeTotal++;
        } else {                                          // else there's an active process
            HistoryStruct current = getProcessHistory(Active);   //get active process burst info (see function details)

            moveToQueue(Active, current.burstLetter, false);     //time to get the show on the road,
                                                                 //move active process into a queue (see function details)

            completeBurst(Active, current, Active->CPUtimer, processesInUse, false); //complete burst for process (see function details)

            if (Active != nullptr) {                      //if there's an active process, increase some data members
                Active->CPUTotal++;
                Active->CPUtimer++;
            }
        }

        makeProcessActive(IActive, inputQueue);           //move a process from inputQueue to active state (IActive)

        if (IActive != nullptr) {                         //there's currently a IActive process
            HistoryStruct currentInputHistory = getProcessHistory(IActive);                       //get burst info for this IActive process

            completeBurst(IActive, currentInputHistory, IActive->IOtimer, processesInUse,  true); //complete burst for process (see function details)

            if (IActive != nullptr) {                     //if there's an active process, increase some data members
                IActive->ITotal++;
                IActive->IOtimer++;
            }
        }

        makeProcessActive(OActive, outputQueue);
        										

        if (OActive != nullptr) {
            HistoryStruct currentInputHistory = getProcessHistory(OActive);

            completeBurst(OActive, currentInputHistory, OActive->IOtimer, processesInUse,  true);

            if (OActive != nullptr) {
                OActive->IOtimer++;
                OActive->OTotal++;
            }
        }
        timer++;                                 //incrementing the (tick) timer.
    }
   
    cout << endl << endl << endl << "*****************************************" << endl  //printing some stats of the program run
		<< "Program ended at time: " << timer << endl
    	<< "Total amount of time CPU was idle: " << idleTimeTotal << endl
    	<< "Total amount of terminated processes: " << endProcessCnt;

    printReport();  //print one final report to confirm everything is empty/nullptr

	return 0;
}

/************************************
Name: setProcess(Process* processToSet, const char* inLine)
returns: nothing
arguments: 1) 
use: 1) copies inLine (holds Process info) into buffer
	2) grabs processName and sets the processName
	3) grabs ProcessPriority and ProcessArrival and sets them
************************************/
void setProcess(Process* processToSet, const char* inLine) {
    char buffer[256];         //hold inLine (which holds ProcessName, ProcessPriority, ProcessArrival)
    string processName;       //will hold the ProcessName
    char* inData;             //char ptr that will point to next data (ProcessPriority, ProcessArrival)
    
    strcpy(buffer, inLine);  //copy inLine to buffer to make some edits
    
    processName = strtok(buffer, " ");              //grab processName from buffer
    processToSet->processName = processName;        //set processName

	inData = strtok(NULL, " ");                     //grab ProcessPriority from buffer
    processToSet->priority = atoi(inData);      // set ProcessPriority
	
	inData = strtok(NULL, " ");                    //grab ProcessArrival from buffer
    processToSet->arrivalTime = atoi(inData);  // set ProcessArrival
}

/*******************************************
Name: completeBurst(*and a lot of arguments*)
Returns: Nothing
Arguments:
	1) reference to a ptr to a Process, called processToMove, this is the process that will be moved around (the world)
	2) a HistoryStruct, called processToMoveStuct, this holds the burst info for processToMove
	3) reference to a unsigned int, called inTimer, this holds current "time" (tick or whatever)
	4) a reference to a int, called processesInUse, this holds how many process are in use (no more than 5 at once)
	5) a bool, called isIOBurst, holds if a process is a Input/Output burst (true if it is)
*******************************************/
void completeBurst(Process*& processToMove, HistoryStruct processToMoveStruct, unsigned int& inTimer, int& processesInUse,
                	bool isIOBurst) {
    if(processToMove != nullptr) {                          //if there is an active process?
        if(inTimer == processToMoveStruct.burstValue) {     //if burst timer reaches the current burst time, go to next burst
            inTimer = 0;                                    //set burst timer = 0
            processToMove->sub++;                           //move onto next burst
            if(isIOBurst) {                                 //is this a Input or Output burst?
                if(processToMoveStruct.burstLetter == "I") { //is this a Input burst?
                    processToMove->ICount++;                 //add one to running total of Input bursts
                }
                if(processToMoveStruct.burstLetter == "O") { //is this a Output burst?
                    processToMove->OCount++;                 //add one to running total of Output bursts
                }
                moveToQueue(processToMove, processToMoveStruct.burstLetter, isIOBurst); //time to move it
                return;                                                                 
            }
            processToMoveStruct = getProcessHistory(processToMove);       //get the burst info for this process
            if(processToMoveStruct.burstLetter == "N") {                  //if current burstLetter is N (meaning end of process), then end the process
                processToMove->CPUCount++;                                //add one to running total of CPU bursts
                terminateProcess(processToMove);                          //end process 

                processesInUse--;                                         //a process no longer in use, after termination, subtract one                             
            } else {                                                      //burstLetter was a C/I/O
                processToMove->CPUCount++;                                //add one to running total of CPU bursts 
                moveToQueue(processToMove, processToMoveStruct.burstLetter, isIOBurst);  //move process to corresponding queue.
            }
        }
    }
}

/*************************************
Name: setProcessHistory(Process* , const char* )
Arguments: 1) a pointer to a Process object, called newProcess, we update this Process' burst info
			2) a char array, called inLine, this holds burst info for on process
Returns: nothing
Use: 1) copy inLine (all burst info) into a buffer
	2) load in process (burst) history into a temp
	3) grab first burst info (first case, is a letter)
	4) while there is burst info
		store burst data (letter)
		grab burst time
		store burst time
		Then store burst info
***********************************/
void setProcessHistory(Process* inProcess, const char* inLine) {
    char buffer[256];                                  //temp buffer
    int index = 0;
    
    strcpy(buffer, inLine);                            //this copies while line (holding burst info) into the buffer

    HistoryStruct* tempHistory = inProcess->history;  //load in burst info

	char* burstInfo = strtok(buffer, " ");         //get burst letter
												   //but will hold burst info (the char representing which burst type (C, I, O, N) or burst time)
    while (burstInfo != nullptr) {                 //while not null
        HistoryStruct* anotherTempHistory = new HistoryStruct();
        
        anotherTempHistory->burstLetter = burstInfo;       //store burst letter

        burstInfo = strtok(NULL, " ");                     //get burst time
        anotherTempHistory->burstValue = atoi(burstInfo);  //store burst time
        
        burstInfo = strtok(NULL, " ");                     //get burst letter
        
        tempHistory[index] = *anotherTempHistory;          //store burst info (ex, C 4)
        index++;
    }
}

/*************************************
Name: getProcessHistory(Process* processHistoryToReturn)
Arguments: 1) a ptr to a Process, called processHistoryToReturn
				the process who's burst info we want
Returns:
Use: 
*************************************/
HistoryStruct getProcessHistory(Process* processHistoryToReturn) {
	return processHistoryToReturn->history[processHistoryToReturn->sub];
}

/*********************************
Name: fileReading()
Arguments: None
Returns: Nothing
Use: 1) Attemps to open txt file
		if fails, display message, exit
	2) while there is data in the file	
		Creates a new Procces pointer object
			give it a ID
		calls function to set other Process data members
		if "STOPHERE" is found
			do some stuff with ptrs
			then exit
		push Process object into entryQueue
		Get line with burst info
			call function to set it 
		Finally, add one to process ID
*********************************/
void fileReading() {
    string inLine;                                       //string to store line from file
    int startingID = 100;                                //starting process ID number
    
    ifstream inFile("data4.txt");                        //open the file.
    if(inFile.fail()) {                                  //did text file open?
		cerr << "Couldn't open text file!" << endl;      //no, exit
		return;
	}

	getline(inFile, inLine);                             //primary read
    while(inFile) {                                      //while still data in file
        Process* temp = new Process(startingID);         //create a new Process, and set its process ID

        setProcess(temp, inLine.c_str());         		 //calls function to set other Process data members
        												 //inLine holds Process Name, Priority, and arrival time

        if (temp->processName == "STOPHERE") {           //if STOPHERE process is found 
            delete (temp);                               
            temp = nullptr;                              
            break;                                       //exit
        }

        entryQueue.push(temp);                           //push Process ptr into entryQueue

        getline(inFile, inLine);                         //gets line that represents bursts/burt times
        setProcessHistory(temp, inLine.c_str());         //calls function to store the line
        startingID++;                                    //up ID counter
        
        getline(inFile, inLine);                         //secondary read
    }
    inFile.close();                                      //close file
}

/*************************************
Name: terminateProcess(Process*& toTerminate)
Arguments: 1) a reference to a ptr to a Process, called toTerminate
Returns: Nothing
Use: 1) prints process name and what time it was terminated at
	2) prints Process info
	3) add one to terminated process count
	4) deletes Process, then sets ptr to nullptr
*************************************/
void terminateProcess(Process*& toTerminate) {
    cout << endl << toTerminate->processName << " terminated at time: " << timer;
    toTerminate->printInfo();   //print info on terminated process
    endProcessCnt++;            //add one to terminated process count
    delete toTerminate;         //delete
    toTerminate = nullptr;      //active process ptr to nullptr
}

/*************************************
Name: condition(int inTime)
Arguments: 1) an int, called inTime, holds current tick number
Returns: True - if condition has been met
		False - if condition not met
Use: 1) Used to check if MAX_TIME (500) has been reached by the timer
		If queues are not empty
		If there are any active process
*************************************/
bool condition(int inTime) {
    return (inTime < MAX_TIME) &&
           (!entryQueue.empty() || !readyQueue.empty() || !inputQueue.empty() || !outputQueue.empty() ||
            Active != nullptr || IActive != nullptr || OActive != nullptr);
}

/*************************************
Name: makeProcessActive(Process*& toActiveState, priority_queue<Process*, vector<Process*>, Compare>& toMakeActive)
Arguments: 1) reference to a ptr to a Process, Process will be moved here to be marked as active
				called toActiveState
			2) reference to a ptr to a priority_queue of Process, a Process will be moved from here
				called toMakeActive
Returns: nothing
Use: 1) checks if there is currently an active process
		if there is active process, do nothing
	2) If not active process, then checks if priority_queue is empty
		if it is, do nothing
	 	if it is not, move top Process from queue into active Process, then pop off queue
*************************************/
void makeProcessActive(Process*& toActiveState, priority_queue<Process*, vector<Process*>, Compare>& toMakeActive) {
    if(toActiveState == nullptr) {              //is there an active process?
        if(toMakeActive.size() > 0) {           //no, is queue empty?
            toActiveState = toMakeActive.top(); //no, move top of queue into active process
            toMakeActive.pop();                 //remove one from queue
        }
    }
}

/**************************************
Name: moveToQueue(Process*& currentProcess, string nameQueue, bool isIOBurstComplete)
arguments: 1) a reference to a ptr to a Process, called currentProcess, process to be moved
			2) a string, called nameQueue, burst letter to be matched
			3) a bool, called bool backToReadyQueue, decided if a Process goes back into the Ready queue
returns: nothing
use: 1) checks if Input/Output burst is done,
			if so, pushed the item back into the ready queue
	2) checks if nameQueue burst is "O", meaning its an Output burst
			if so, move that process into the Output queue and set ptr = nullptr
	3) checks if nameQueue burst is "I", meaning its an Input burst
			if so, move that process into the Input queue and set ptr = nullptr
***************************************/
void moveToQueue(Process*& currentProcess, string nameQueue, bool backToReadyQueue) {
    if(backToReadyQueue) {
        readyQueue.push(currentProcess);     //push back into ready queue
        currentProcess = nullptr;
    } else if(nameQueue == "O") {           
        outputQueue.push(currentProcess);   //push into output queue
        currentProcess = nullptr;
    } else if(nameQueue == "I") {
        inputQueue.push(currentProcess);    //push into input queue
        currentProcess = nullptr;
    } else {
    	//nothing
	}
}

/************************************
Name: printQueue(queue<Process*> inQueue)
arguments: none
returns: nothing
use: 1) Checks if the queue is empty,
			if it is, prints message, leaves function
	2) While the queue is not empty,
			grabs the top object, places it into a Process point temp var
			prints out the process info
			removes top object from queue
*************************************/
void printQueue(queue<Process*> inQueue) {
    if (inQueue.empty()) {               //is queue empty ?
        cout << "(Empty)" << endl;       //yes, leave function
        return;
    }

    while (!inQueue.empty()) {           // while priority queue is not empty
        Process *temp = inQueue.front(); //store top object
        cout << temp->processName << "  (ID: " << temp->processID << ")" << endl;  //print info
        inQueue.pop();                   //remove top object
    }
}

/************************************
Name: printPriorityQueue(priority_queue<Process*, vector<Process*>, Compare> inPriQueue)
arguments: none
returns: nothing
use: 1) Checks if the priority queue is empty,
			if it is, prints message, leaves function
	2) While the priority queue is not empty,
			grabs the top object, places it into a Process point temp var
			prints out the process name
			removes top object from priority queue
*************************************/
void printPriorityQueue(priority_queue<Process*, vector<Process*>, Compare> inPriQueue) {
    if (inPriQueue.empty()) {              //is priority queue empty ?
        cout << "(Empty)" << endl;         //yes, leave function
        return;
    }
    
    while (!inPriQueue.empty()) {          // while priority queue is not empty
        Process *temp = inPriQueue.top();  //store top object
        cout << temp->processName << "  (ID: " << temp->processID << ")" << endl;  //print info
        inPriQueue.pop();                  //remove top object
    }
}

/*******************************
Name: printProcessContents()
Arguments: none
Returns: none
use: 1) checks for active process
		if none, display empty message
		if a process is active, displays that process name/ID
*******************************/
void printProcessContents() {
	cout << endl << "*****************************************" << endl << endl
    	<< "Status at time " << timer << endl;               //header

    if (Active == nullptr) {                                 //is process null?
        cout << "Active: (Empty)" << endl;                   //yes, display empty
	} else {
        cout << "Active: " << Active->processName << " (ID: " << Active->processID << ")" << endl;  //no, display process info
    }

    if (IActive == nullptr) {
        cout << "IActive: (Empty)" << endl;
    } else {
        cout << "IActive: " << IActive->processName << " (ID: " << IActive->processID << ")" << endl;
    }

    if (OActive == nullptr) {
        cout << "OActive: (Empty)" << endl;
    } else {
        cout << "OActive: " << OActive->processName << " (ID: " << OActive->processID << ")" << endl;
    }
}

/*******************************
Name: printReport()
Arguments: none
Returns: none
use: 1) Calls printProcessContents() to display any active process
	2) Calss printQueue/printPriorityQueue to display contents of that queue
*******************************/
void printReport() {
    printProcessContents();                                 //display any active process

    cout << endl << "EntryQueue contains:" << endl;  //header
    printQueue(entryQueue);                          //display contents of queue

    cout << endl << "ReadyQueue contains:" << endl;
    printPriorityQueue(readyQueue);

    cout << endl << "InputQueue contains:" << endl;
    printPriorityQueue(inputQueue);

    cout << endl <<"OutputQueue contains:" << endl;
    printPriorityQueue(outputQueue);
    
    cout << endl << "*****************************************" << endl;
}
