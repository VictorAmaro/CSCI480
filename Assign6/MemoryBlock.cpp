/********************************************
CSCI 480 - Assignment 6 - Spring 2018

Progammer: Victor Amaro
Z-ID: z1747708
Section: 1
Date Due: April 30, 2018

Purpose: Constains Memory block constructors
*******************************************/
#include "MemoryBlock.h"

/*********************************************************
Constructor
Set the start adress and the block size
**********************************************************/
MemoryBlock::MemoryBlock(int startAddress, int blockSize) {
    this->startLocation = startAddress;
    this->blockSize = blockSize;
}

/*********************************************************
Constructor
Set the start adress, blockSize, processID, and blockID
**********************************************************/
MemoryBlock::MemoryBlock(int startAddress, int blockSize, string processID, string blockID) {
    this->startLocation = startAddress;
    this->blockSize = blockSize;
    this->memoryBlockID = blockID;
    this->processID = processID;
}
