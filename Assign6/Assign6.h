/********************************************
CSCI 480 - Assignment 6 - Spring 2018

Progammer: Victor Amaro
Z-ID: z1747708
Section: 1
Date Due: April 30, 2018

Purpose: Contains constants and Assign6.cpp function prototypes
*******************************************/
#include "MemoryBlock.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <sstream>
using std::cout; using std::cerr; using std::endl;
using std::setw;
using std::ifstream; using std::istringstream; 
using std::numeric_limits; using std::max;
using std::iterator;

#define HOW_OFTEN 5
#define MB 1048576
#define KB 1024

list<MemoryBlock*> inUse;	//blocks that are presently in use
list<MemoryBlock*> available;	//not presently in use

MemoryBlock* findProcess(string, string );

list<MemoryBlock*>::iterator firstFit(int );
list<MemoryBlock*>::iterator bestFit(int ); 

void printList(list<MemoryBlock*>);
void algorithmHeader(string);
void endCall();

void initialize();
void allocate(list<MemoryBlock>*, string, int, string);
void load(string, int, string, string);
void attach(MemoryBlock*);
void normalize();

void dellocate(string, string, string);
void killProcess(string);

void read(string);
void processLine(string, string);
