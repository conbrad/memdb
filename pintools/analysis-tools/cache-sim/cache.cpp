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
    totalBytesBroughtIn = 0;
    totalBytesWasted = 0;
    numAccesses = 0;
	tagMaskBits = log2(lineSize) + log2(numSets);	// how many bits we have to shift the address to compute the tag
}

void Cache::access(size_t address, unsigned short accessSize, logentry accessLog) {
    numAccesses++;
    /* See if the access spans two cache lines.
     */
    int lineOffset = address % lineSize;

    if(lineOffset + accessSize <= lineSize) {
    	__access(address, accessSize, accessLog);
    	return;
    }

    /* If we are here, we have a spanning access.
     * Determine the address of the first byte that
     * spills into another cache line.
     */
    uint16_t bytesFittingIntoFirstLine = lineSize - lineOffset;
    size_t addressOfFirstByteNotFitting = address + bytesFittingIntoFirstLine;
    uint16_t sizeOfSpillingAccess = accessSize - bytesFittingIntoFirstLine;

    if(VERBOSE) {
    	verboseSpanningAccessOutput(address, accessSize, accessLog);
    	verboseSplitAccessOutput(address, bytesFittingIntoFirstLine);
    	verboseSpilledAccessOutput(addressOfFirstByteNotFitting, sizeOfSpillingAccess);
    }

    /* Split them into two accesses */
    __access(address, bytesFittingIntoFirstLine, accessLog);

    /* We recursively call this function in case the spilling access
     * spans more than two lines. */
    access(addressOfFirstByteNotFitting, sizeOfSpillingAccess, accessLog);
}
void Cache::verboseSpanningAccessOutput(size_t address, unsigned short accessSize,
		logentry accessLog) {
	cerr << "SPANNING ACCESS: 0x" << hex << address
			<< dec << " " << accessSize << endl;
}

void Cache::verboseSplitAccessOutput(size_t address, uint16_t bytesFittingIntoFirstLine) {
	cerr << "Split into: " << endl;
	cerr << "\t0x" << hex << address << dec << " "
			<< bytesFittingIntoFirstLine << endl;
}

void Cache::verboseSpilledAccessOutput(size_t addressOfFirstByteNotFitting, uint16_t sizeOfSpillingAccess) {
	cerr << "\t0x" << hex << addressOfFirstByteNotFitting << dec << " "
	    			<< sizeOfSpillingAccess << endl;
}

/* Here we assume that accesses would not be spanning cache
 * lines. The calling function should have taken care of this.
 */
void Cache::__access(size_t address, unsigned short accessSize, logentry accessLog) {
    /* Locate the set that we have to access */
    int setNum = (address >> (int)log2(lineSize)) % numSets;

    assert(setNum < numSets);

    if(VERBOSE) {
    	verboseSetOutput(address, setNum);
    }

    set[setNum].access(address, accessSize, accessLog);
}

void Cache::verboseSetOutput(size_t address, int setNum) {
	cout << hex << address << dec << " maps into set #" << setNum << endl;
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
    for (int i = 0; i < assoc; i++) {
        totalBytesBroughtIn += set[i].getBytesBroughtIn();
    }
    return totalBytesBroughtIn;
}

unsigned int Cache::getTotalBytesWasted() {
    totalBytesWasted = 0; 
    for (int i = 0; i < assoc; i++) {
        totalBytesWasted += set[i].getBytesWasted();
    }
    return totalBytesWasted;
}

void Cache::printFullCacheLines() {
    ofstream cacheLineUsage;
    cacheLineUsage.open("cache-line-usage.txt");
    cacheLineUsage << "Cache line usage: " << endl;
    for (int i = 0; i < numSets; i++) {
        cacheLineUsage << "Set: " << i << endl;
        cacheLineUsage << "----------" << endl;
        cacheLineUsage << set[i].printCacheLineUsage();
        cacheLineUsage << "----------" << endl;
    }
    cacheLineUsage.close();
}

void Cache::printParams() {
    cout << "Line size      = " << lineSize << endl;
    cout << "Number of sets = " << numSets << endl;
    cout << "Associativity  = " << assoc << endl;
}
