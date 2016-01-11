#include "cache.h"
#include <ctgmath>
#include <assert.h>
#include <iostream>
#include <fstream>

#include "../main.h"
#include "../cache-waste-analysis.h"

using namespace std;

Cache::Cache(int numSets, int assoc, int lineSize) {
	this->numSets = numSets;
	this->assoc = assoc;
	this->lineSize = lineSize;
	this->set = new CacheSet[numSets];
    
    setCacheSetIDs(); 

    totalBytesBroughtIn = 0;
    totalBytesWasted = 0;
    numAccesses = 0;

    // Bits we have to shift the address to compute the tag
	tagMaskBits = log2(lineSize) + log2(numSets);	
}

void Cache::setCacheSetIDs() {
    for(int i = 0; i < numSets; i++) {
        set[i].setCacheSetID(i);
    }
}

void Cache::access(size_t address, unsigned short accessSize, logentry accessLog) {
   
    numAccesses++;

    // See if the access spans two cache lines     
    int lineOffset = address % lineSize;

    if(lineOffset + accessSize <= lineSize) {
    	__access(address, accessSize, accessLog);
    	return;
    }

    // Determine the address of the first byte that spills into another cache line
    uint16_t firstLineBytes = lineSize - lineOffset;
    size_t  addressOfExtraBytes = address + firstLineBytes;
    uint16_t sizeOfExtraAccess = accessSize - firstLineBytes;

    // Split into two accesses
    __access(address, firstLineBytes, accessLog);

    // Recur on spilling access
    access(addressOfExtraBytes, sizeOfExtraAccess, accessLog);
}

 // Fits into single cache line
void Cache::__access(size_t address, unsigned short accessSize, logentry accessLog) {    
    // Locate the set that we have to access
    int setNum = (address >> (int)log2(lineSize)) % numSets;
    assert(setNum < numSets);

    set[setNum].access(address, accessSize, accessLog);
}

int Cache::getNumSets() {
	return numSets;
}

int Cache::getAssoc() {
	return assoc;
}

int Cache::getCacheLineSize() {
	return lineSize;
}

unsigned int Cache::getNumAccesses() {
    return numAccesses;
}

unsigned int Cache::getTotalBytesBroughtIn() {
    totalBytesBroughtIn = 0;
    for (int i = 0; i < numSets; i++) {
        totalBytesBroughtIn += set[i].getBytesTransferred();
    }
    return totalBytesBroughtIn;
}

unsigned int Cache::getTotalBytesWasted() {
    totalBytesWasted = 0; 
    for (int i = 0; i < numSets; i++) {
        totalBytesWasted += set[i].getBytesWasted();
    }
    return totalBytesWasted;
}

unsigned int Cache::getNumMisses() {
    unsigned int totalMisses = 0;
    for (int i = 0; i < numSets; i++) {
        totalMisses += set[i].getNumMisses();
    }
    return totalMisses;
}

void Cache::printParams() {
    cout << "Line size      = " << lineSize << endl;
    cout << "Number of sets = " << numSets << endl;
    cout << "Associativity  = " << assoc << endl;
}
