/********************************************
CSCI 480 - Assignment 6 - Spring 2018

Progammer: Victor Amaro
Z-ID: z1747708
Section: 1
Date Due: April 30, 2018

Purpose: Simulating memory management. When a process is loaded into memory or a process requests a block of memory dynamically, 
	the system must allocate memory, and when a process terminates or frees memory previously requested, the system must deallocate memory.
*******************************************/
#include "Assign6.h"

int main(int argc, char **argv) {
	if (argc != 2) {	//incorrect number of arguments entered, exit
		cerr << "ERROR: USAGE: ./Assign6: INCORRECT NUMBER OF ARGUMENTS!" << endl;
		exit(-1);
	}
	
	initialize();	//call initialize
	
	read(argv[1]);	//call read and pass it the first argument (B or F)
	
	endCall();	//end of program

	return 0;
}

/***************************************************
normalize()
returns: nothing
arguments: none
use: cleans up avaiable list
***************************************************/
void normalize() {
	MemoryBlock *currentBlock = *(available.begin());	//set a current block to start of available list
	
	int endAddress = currentBlock->startLocation + currentBlock->blockSize;	//set the end address to currentBlock's end address
	int startAddress;
	
	list<MemoryBlock*>::iterator it = available.begin();	//set up iterator
	it++;	//move iterator forward to skip first block
	
	while (it != available.end()) {	//cycle thru available list
		currentBlock = *it;	//reset current block
		startAddress = currentBlock->startLocation;	//set the start address to currentBlock's start address

		if (endAddress == startAddress) {	//start address is == to end address
			int blockSize = currentBlock->blockSize;	//set blockSize to currentBlock's block size
			it--;	//move iterator backwords

			MemoryBlock *prevBlock = *it;	//get previous memory block
			int newSize = prevBlock->blockSize + blockSize;	//set size to previous memory block's size + blockSize
			
			if (newSize < 4 * MB) {	//if mergeable
				cout << "Merging free memory block at address " << prevBlock->startLocation << " with free memory block at address " << currentBlock->startLocation << endl;	//header
				
				MemoryBlock *newBlock = new MemoryBlock(prevBlock->startLocation, newSize);	//new memory block
				available.erase(it++);	//remove from the available list

				delete (prevBlock);	//delete
				prevBlock = *it;	//reset prevoous block

				available.erase(it++);	//remove from the available list
				delete (prevBlock);	//delete

				prevBlock = nullptr;	//nullptr
				available.insert(it, newBlock);	//insert into the available list
				endAddress = newBlock->startLocation + newBlock->blockSize;	//reset end address
				
				continue;	//whoops
			} else {
				it++;	//move iterator forward
				endAddress = startAddress + blockSize;	//reset end address
				
				continue;	//whoops
			}
		}
		endAddress = currentBlock->blockSize + startAddress;	//set the ending address
		it++;	//move iterator forward
	}
}

/***************************************************
processLine(string, string)
returns: nothing
arguments: a string, holding a line from a text file
			a string, holding the trnasaction type
use: called to process each line of the txt file
	and call the correct method for it based off the transaction
***************************************************/
void processLine(string line, string type) {
	istringstream inStream(line);	//input string stream.
	string transactionType;	//hold transaction	
	
	inStream >> transactionType;	//set transaction

	cout << endl;

	if (transactionType == "L" || transactionType == "A") {	//transaction was to load or allocate process
		string processName,	//block info
			processID;
		int blockSize;		

		inStream >> processID;	//set processID
		inStream >> blockSize;	//set block size
		inStream >> processName;	//set process name
		
		if (transactionType == "L") {	//transaction was to load
			cout << endl << "Transaction: Request to load process " << processID << ", Block ID " << processName << " using " << endl
				<< blockSize << " bytes of memory" << endl;
		} else { //transaction was to allocate
			cout << endl << endl << "Transaction: Request to Allocate additional " << blockSize << " bytes for process " << processID << ", " << endl 
				<< "Block ID: " << processName;
		}

		load(processID, blockSize, processName, type);	//call load for this process
	} else if (transactionType == "D") {	//transaction was to deallocate process
		string processID,
			processName;
		inStream >> processID;	//set up processID and processName
		inStream >> processName;
		
		cout << endl << endl << "Transaction: Request to DeAllocate Block ID: " << processName << " for process " << processID << endl;	//print header

		dellocate(processID, processName, transactionType);	//call deallocate for this process
	} else {	//transaction was to terminate process
		string processID;
		inStream >> processID; //set up process ID
		
		cout << endl << endl << "Transaction: Request to Terminate process " << processID << endl; //print header
		
		killProcess(processID);	//call killProcess for this processID
	}
}

/***************************************************
allocate(string, int, string, transaction)
returns: nothing:
arguments: a MemoryBlock iterator, MemoryBlock to allocate
			a string, holding the process name
			a int, holding the required block size
			a string, holding the process name
use: called to allocate space for a MemoryBlock
***************************************************/
void allocate(list<MemoryBlock*>::iterator it, string processID, int requiredBlockSize, string processName) {
	MemoryBlock *current = *it;	//set up a iterator
	MemoryBlock *memoryBlock = new MemoryBlock(current->startLocation, requiredBlockSize, processID, processName);	//create new MemoryBlock	
	int start = current->startLocation + requiredBlockSize;	//set up the start location
	
	inUse.push_front(memoryBlock);	//push block into inUse list
	available.erase(it++);	//remove block from available list

	int remaningBlock = current->blockSize - requiredBlockSize;	// remaining memory
	if (remaningBlock > 0) {	//memory leftover
		MemoryBlock *memoryBlock2 = new MemoryBlock(start, remaningBlock, "", "");	//create new MemoryBlock
		available.insert(it, memoryBlock2);	//insert memoryBlock2 where the iterator is pointing to
	}
}

/***************************************************
load(string, int, string, transaction)
returns: nothing:
arguments: a string, holding the processID
	a int, holding the required block size
	a string, holding the process name
	a string, holding the transaction (B or F)
use: based on transaction, finds a suitable block, then calls allocate
***************************************************/
void load(string processID, int blockSize, string processName, string transaction) {
	list<MemoryBlock *>::iterator it;	//set up iterator

	if (transaction == "F") {	//use the FirstFit algorithm
		it = firstFit(blockSize);	//find a suitable block to insert into
		if (it == available.end()) {	//if block was at the end of avilable list, print error message, then leave
			cerr << "ERROR: ATTEMPT TO ALLOCATE MEMORY FAILED" << endl
				<< "NO BLOCK OF SUFFICIENT MEMORY AVAILABLE!";
				
			return;
		}
	} else if (transaction == "B") {	//use the BestFit algorithm
		it = bestFit(blockSize);	//find a suitable block to insert into
		if (it == available.end()) {	//if block was at the end of avilable list, print error message, then leave
			cerr << "ERROR: ATTEMPT TO ALLOCATE MEMORY FAILED" << endl
				<< "NO BLOCK OF SUFFICIENT MEMORY AVAILABLE!";
				
			return;
		} else {	//print message that a block was found
			cout << endl << "Sufficient memory block found! Size: " << (*it)->blockSize << "\n";
			cout << "Allocating Memory to Block Successful!\n";
		}
	}
		
	allocate(it, processID, blockSize, processName);	//call allocate for the process
}

/***************************************************
firstFit(int)
returns: a MemoryBlock iterator
arguments: a int, the required block size
use: called to find a space to insert the MemoryBlock,
	baed on a FirstFit algorithm
***************************************************/
list<MemoryBlock*>::iterator firstFit(int requiredBlockSize) {
	list<MemoryBlock*>::iterator it = available.begin();	//iterator to use

	while (it != available.end()) {	//cycle thru the avilable list
		MemoryBlock *current = *it;	//set current block

		if (current->blockSize >= requiredBlockSize) {	//found space for block
			cout << "Sufficient memory block found! Size: " << current->blockSize << endl
				<< "Allocating Memory to Block Successful!" << endl;
			return it;	//return block
		}
		it++;	//move iterator forward
	}
	
	return it;	//return block
}

/***************************************************
bestFit(int)
returns: a MemoryBlock iterator
arguments: a int, the required block size
use: called to find a space to insert the MemoryBlock,
	baed on a FirstFit algorithm
***************************************************/
list<MemoryBlock*>::iterator bestFit(int requiredBlockSize) {
	list<MemoryBlock*>::iterator it = available.begin();	//create iterators to use
	list<MemoryBlock*>::iterator minIterator = available.end();
	
	int minValue = numeric_limits<int>::max();	//set iterator

	while (it != available.end()) {	//cycle thru the available list
		MemoryBlock *currentBlock = *it;	//set a current block

		int currentBlockSize = currentBlock->blockSize;	//set the block size for the currentBlock

		if (currentBlockSize >= requiredBlockSize) {	//currentBlock size greater than required block size
			if (currentBlockSize < minValue) {	//currentBlockSize is less than the minimum size
				minIterator = it;
				minValue = (*minIterator)->blockSize;
			}
		}
		it++;	//move iterator forward
	}
	
	return minIterator;
}

/***************************************************
findProcess(string, string)
returns: if found, a ptr to a MemoryBlock
			if not, null
arguments: a string, the process ID to search for
			a string, the process name to search for
use: called to find a MemoryBlock object in inUse list
***************************************************/
MemoryBlock* findProcess(string processID, string processName) {
	for (list<MemoryBlock *>::iterator it = inUse.begin(); it != inUse.end(); it++) {	//cycle thru list
		MemoryBlock *currentBlock = *it;	//set a current block
		if ( (currentBlock->processID == processID) && (currentBlock->memoryBlockID == processName) ) {	//found a matching memory block
			inUse.erase(it);	//remove
			return currentBlock;	//return the found block
		}
	}
	
	return nullptr; //process not found, return null
}

/***************************************************
attach(MemoryBlock* )
returns: nothing
arguments: a MemoryBlock ptr, the block to insert back
use: called to reattach a memory block into the list
***************************************************/
void attach(MemoryBlock *memoryBlock) {
	for (list<MemoryBlock*>::iterator it = available.begin(); it != available.end(); it++) {	//cycle thru the avilable list
		MemoryBlock *currentBlock = *it;	//set current block

		if (currentBlock->startLocation > memoryBlock->startLocation) {	//a spot is found, called insert to bring back in the memory block
			available.insert(it, memoryBlock);
			return;
		}
	}
}

/***************************************************
dellocate(strin, string, string )
returns: nothing
arguments: a string, containing the processID
			a string containing the processName
			a string, containing the transaction type
use: called to dellocate a process
***************************************************/
void dellocate(string processID, string processName, string transaction) {
	MemoryBlock *memoryBlock;

	memoryBlock = findProcess(processID, processName);	//attempt to find memory block
	
	if (memoryBlock== nullptr) {	//not found, display messahe then leave	
		cerr << endl << "ERROR: ATTEMPT TO DEALLOCATE MEMORY FAILED. BLOCK NOT FOUND!" << endl;
		return;
	}

	attach(memoryBlock);	//make that memory block avilable again

	normalize();	//normalize
	
	if(transaction == "D" ) {	//tansaction was to deeolcate dellocate process
		cout << endl << endl << "Memory block is found!!" << endl
		<< "Deallocation of block successful!" << endl;
	}
}

/***************************************************
killProcess(string )
returns: nothing
arguments: a string, the ID of the process to kill
use: called to find a process and kill it
***************************************************/
void killProcess(string processID) {
	list<MemoryBlock*>::iterator it = inUse.begin();	//start iterator to start of inUse
	bool foundProcess = false;	//flag for a found process
	
	while (it != inUse.end()) {	//while not at end of list
		MemoryBlock *currentBlock = *it;
		if (currentBlock->processID == processID) { //process ID of desired block to kill is found
			foundProcess = true;	//process was found
			it++;	//move the iterator forward
			dellocate(processID, currentBlock->memoryBlockID, "T");	//deallocate process
		} else {	//process ID not found	
			it++; //move the iterator forward
		}
	}
	
	if (foundProcess) {	//a process was terminated, print header
		cout<< "Process "<< processID <<" successfully terminated" << endl;
	} else {	//no process was terminated, print header
		cerr << "ERROR: ATTEMPT TO TERMINATE PROCESS FAILED. PROCESS NOT FOUND!" << endl;
	}
}

/***************************************************
printList(list<MemoryBlock*>)
returns: nothing
arguments: a list of MemoryBlock ptr objects
use: prints all the info on the list, (blocks and size)
***************************************************/
void printList(list<MemoryBlock*> inList) {
	int size = 0;	// size of list

	if (!inList.empty()) {	//list was not empty
		cout << endl << "Start Address" << setw(20) << "Size" << endl;

		for (list<MemoryBlock *>::iterator it = inList.begin(); it != inList.end(); it++) {	//cycle thru curent list
			MemoryBlock *currentBlock = *it;	//set current block
		
			size += currentBlock->blockSize;	//add to total size		

			cout << setw(10) << currentBlock->startLocation << setw(25) << currentBlock->blockSize << endl;	//print info on currentBlock
		}
	} else {	//list was empty
		cout << endl << "(NONE)" << endl;
	}
	
	cout << "Total size of the list = " << size << endl;
}

/***************************************************
read(string )
returns: nothing
arguments: a string, the user input for which algorithm to use
Use: opens text file,
	Prints header on user input,
	loads in txt file data
***************************************************/
void read(string type) {
	ifstream inFile;
	inFile.open("data6.txt");	//attempt to open file
	
	if (inFile.fail()) {	//couldn't open file
		cerr << "ERROR: Couldn't open file!" << endl;
		exit(-1);
	}
	
	algorithmHeader(type);	//print header
	
	string line;	//hold line from txt file

	while (getline(inFile, line)) {	//while data in file
		if (line == "?") {	//stop when ? is found
			break;
		}
		
		processLine(line, type);	//process text file line
		
		cout << endl << "List of available blocks";	//print header
		
		printList(available);	//print available list
		
		cout << endl << endl << "List of blocks currently in use";	//print header
		
		printList(inUse);	//print inUse list
	}
}

/***************************************************
initialize()
returns: nothing
arguments: none
use: load MemoryBlock objects into available
***************************************************/
void initialize() {
	MemoryBlock *memoryBlock1 = new MemoryBlock(3 * MB, MB);	//Memory block objects
	MemoryBlock *memoryBlock2 = new MemoryBlock(4 * MB, 2 * MB);
	MemoryBlock *memoryBlock3 = new MemoryBlock(6 * MB, 2 * MB);
	MemoryBlock *memoryBlock4 = new MemoryBlock(8 * MB, 4 * MB);
	MemoryBlock *memoryBlock5 = new MemoryBlock(12 * MB, 4 * MB);

	available.push_back(memoryBlock1);	//push objects into available
	available.push_back(memoryBlock2);
	available.push_back(memoryBlock3);
	available.push_back(memoryBlock4);
	available.push_back(memoryBlock5);
}

/***************************************************
algorithmHeader(string )
returns: nothing
arguments: a string, the user input for which algorithm to use
Use: prints a header depending on what user entered
***************************************************/	
void algorithmHeader(string type) {
	if (type == "B") {	//user entered B
		cout << endl << "You chose to run the Best-Fit algorithm for Memory Management!" << endl;
	} else if (type == "F") {	//user entered F
		cout << endl << "You chose to run the First-Fit algorithm for Memory Management!" << endl;
	} else { //user something not supported, exit
		cerr << "ERROR: Invalid arguments, use B or F!" << endl;
		exit(-1);
	}	
	
	cout << endl << "Beginning the Simulation Run now." << endl;	//start header
}

/***************************************************
endCall()
returns: nothing
arguments: none
Use: prints one final stats 
***************************************************/	
void endCall() {
	cout << endl << endl << "End of the Simulation!" << endl	//print end header
		<< endl << "List of available blocks";
		
	printList(available);	//show avilable blocks
	
	cout << endl << "List of blocks currently in use";
	
	printList(inUse);	//show any blocks in use (should be none)
}