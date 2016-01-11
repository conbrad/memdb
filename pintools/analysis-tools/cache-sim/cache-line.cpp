#include "cache-line.h"
#include <bitset>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <iomanip>
#include <string>

#include "../main.h"
#include "../util/map-summarizer.h"
#include "../waste-record-collection.h"
#include "../variable/variable-analyzer.h"
#include "../variable/function.h"
#include "../variable/variable-access.h"
#include "../util/access-parser.h"

using namespace std;

CacheLine::CacheLine() {
    // TODO Builder pattern for complex parameters

    // Size is given in bytes
	lineSize = CACHE_LINE_SIZE;
	address = 0;
	tag = 0;
	tagMaskBits = 0;
	initAccessSize = 0;
    reused = 0;
	timestamp = 0;
	bytesUsed = new bitset<MAX_LINE_SIZE>(lineSize);
}

void CacheLine::setLineID(int id) {
    this->id = id;
}

int CacheLine::getLineID() {
    return id; 
}

void CacheLine::setTag(size_t address, unsigned short accessSize) {
    this->tag = address >> tagMaskBits;
    this->address = address;
    this->initAccessSize = accessSize;
    this->reused = 0;
    this->bytesUsed->reset();
}

size_t CacheLine::amountUsed() {
    return bytesUsed->count();
}

bool CacheLine::valid(size_t address) {
    if(address >> tagMaskBits == tag) {
    	assert(tag > 0);
        return true;
    }
    return false;
}

// Bits set to 1 corresponding to address in cache line to denote them as accessed
// If bits are set, increment reuse counter
int CacheLine::access(size_t address, unsigned short accessSize, size_t timestamp) {
    int lineOffset = address % lineSize;
    assert(valid(address));
    assert(lineOffset + accessSize <= lineSize);

    this->timestamp = timestamp;

    // If first bit is set, assume same variable (and its size) accessed before 
    if(bytesUsed->test(lineOffset)) {
    	reused++;
        return lineOffset;
    }

    int byteThreshold = min(lineOffset + accessSize, lineSize);
    for (int i = lineOffset; i < byteThreshold; i++) {
        bytesUsed->set(i);
    }

    return lineOffset;
}

void CacheLine::evict() {
	address = 0;
    tag = 0;
    reused = 0;
    bytesUsed->reset();
}

int CacheLine::getSize() {
    return lineSize;
}

int CacheLine::unused() {
    return lineSize - bytesUsed->count();
}

size_t CacheLine::getTimestamp() {
	return timestamp;
}

bool CacheLine::isClean() {
    return timestamp == 0;
}

void CacheLine::printParams() {
    cout << "Line size = " << lineSize << endl;
}
