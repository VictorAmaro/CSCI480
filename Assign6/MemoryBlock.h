/********************************************
CSCI 480 - Assignment 6 - Spring 2018

Progammer: Victor Amaro
Z-ID: z1747708
Section: 1
Date Due: April 30, 2018

Purpose: Memory Block class
*******************************************/
#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include <string>
#include <list>
using std::string;
using std::list;

class MemoryBlock{
	public:
    	int startLocation,
    		blockSize;
    	
    	string processID;
    	string memoryBlockID;
    	
    	MemoryBlock(int, int, string, string);
    	MemoryBlock(int ,int );
};
#endif 
