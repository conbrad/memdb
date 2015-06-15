#include "cache.h"
#include <ctgmath>
#include <assert.h>
#include <iostream>

#include "../main.h"

using namespace std;

Cache::Cache(int numSets, int assoc, int lineSize) {
	this->numSets = numSets;
	this->assoc = assoc;
	this->lineSize = lineSize;
	sets = new CacheSet[numSets];

	/* This is by how many bits we have to shift the
	 * address to compute the tag. */
	tagMaskBits = log2(lineSize) + log2(numSets);
}

void Cache::access(size_t address, unsigned short accessSize,
	string accessSite, string varInfo) {

	Main::incrementFunctionCount(accessSite);
    /* See if the access spans two cache lines.
     */
    int lineOffset = address % lineSize;

    if(lineOffset + accessSize <= lineSize) {
    	__access(address, accessSize, accessSite, varInfo);
    	return;
    }

    /* If we are here, we have a spanning access.
     * Determine the address of the first byte that
     * spills into another cache line.
     */
    uint16_t bytesFittingIntoFirstLine = lineSize - lineOffset;
    size_t addressOfFirstByteNotFitting =
	address + bytesFittingIntoFirstLine;
    uint16_t sizeOfSpillingAccess = accessSize - bytesFittingIntoFirstLine;
	#if VERBOSE
    	cerr << "SPANNING ACCESS: 0x" << hex << address
    			<< dec << " " << accessSize << " " << accessSite
				<< " " << varInfo << endl;
    	cerr << "Split into: " << endl;
    	cerr << "\t0x" << hex << address << dec << " "
    			<< bytesFittingIntoFirstLine << endl;
    	cerr << "\t0x" << hex << addressOfFirstByteNotFitting << dec << " "
    			<< sizeOfSpillingAccess << endl;
	#endif


    /* Split them into two accesses */
    __access(address, bytesFittingIntoFirstLine, accessSite, varInfo);

    /* We recursively call this function in case the spilling access
     * spans more than two lines. */
    access(addressOfFirstByteNotFitting, sizeOfSpillingAccess,
	   accessSite, varInfo);
}

/* Here we assume that accesses would not be spanning cache
 * lines. The calling function should have taken care of this.
 */
void Cache::__access(size_t address, unsigned short accessSize,
	string accessSite, string varInfo) {
    /* Locate the set that we have to access */
    int setNum = (address >> (int)log2(lineSize)) % numSets;

    assert(setNum < numSets);
	#if VERBOSE
    	cout << hex << address << dec << " maps into set #" << setNum << endl;
	#endif
    sets[setNum].access(address, accessSize, accessSite, varInfo);

}

void Cache::printParams() {
    cout << "Line size      = " << lineSize << endl;
    cout << "Number of sets = " << numSets << endl;
    cout << "Associativity  = " << assoc << endl;
}
