#include "cache-line.h"
#include <bitset>
#include <iostream>
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

static map<string, int> functionAccessCount;

map<int, vector<CacheLineAccess>> lineAccesses;
map<int, map<FunctionLocation, vector<int>>> lineFunctionAccesses;

typedef std::map<int, std::map<std::string, std::vector<CacheLineAccess>>> FunctionLineMap;
FunctionLineMap lineAccessFunctions;

CacheLine::CacheLine() {
    /* this is ugly, but C++ doesn't
     * allow to allocate an array and
     * initialize all members with the
     * same constructor at the same time.
    */

    /* Size is given in bytes */
	lineSize = CACHE_LINE_SIZE;
	address = 0;
	tag = 0;
	tagMaskBits = 0;
    accessLog = { NULL_LOGENTRY };
	initAccessSize = 0;
    timesReusedBeforeEvicted = 0;
	virtualTimeStamp = 0;
	bytesUsed = new bitset<MAX_LINE_SIZE>(lineSize);
	bytesUsed->reset();
}


/* Print info about the access that caused this line to be
 * brought into the cache */
void CacheLine::printFaultingAccessInfo() {
	cout << "0x" << hex << address << dec << " "
		 << initAccessSize << " "
		 /* << accessSite << varInfo */ << endl;
}

void CacheLine::setAndAccess(size_t address, unsigned short accessSize, logentry accessLog, size_t timeStamp) {
    this->address = address;
    this->initAccessSize = accessSize;
    this->tag = address >> tagMaskBits;
    this->accessLog = accessLog;
    this->timesReusedBeforeEvicted = 0;
    this->bytesUsed->reset();

    int lineOffset = access(address, accessSize, timeStamp);
}

void CacheLine::printAmountUsed() {
    cout << amountUsed() / MAX_LINE_SIZE << endl;
}

size_t CacheLine::amountUsed() {
    return bytesUsed->count();
}


bool CacheLine::valid(size_t address) {
    if(address >> tagMaskBits == tag) {
    	return true;
    }
    return false;
}

/* Set to '1' the bits corresponding to this address
 * within the cache line, to mark the corresponding bytes
 * as "accessed".
 * If those bits are already marked as accessed, we increment
 * the reuse counter.
 */
int CacheLine::access(size_t address, unsigned short accessSize, size_t timeStamp) {
    int lineOffset = address % lineSize;

    assert(valid(address));
    assert(lineOffset + accessSize <= lineSize);

    this->virtualTimeStamp = timeStamp;

    /* We only check if the first bit is set, assuming that if
     * we access the same valid address twice, the data represents
     * the same variable (and thus the same access size) as before
     */
    if(bytesUsed->test(lineOffset)) {
    	timesReusedBeforeEvicted++;
    } else {
    	for(int i = lineOffset; i < min(lineOffset + accessSize, lineSize); i++) {
    		bytesUsed->set(i);
    	}
    }
    return lineOffset;
}

void CacheLine::clearLine() {
	address = 0;
	tag = 0;
    accessLog = { NULL_LOGENTRY };
    timesReusedBeforeEvicted = 0;
	bytesUsed->reset();
}

void CacheLine::evict() {

    /* We are being evicted. Print our stats, update waste maps and clear. */
    if(WANT_RAW_OUTPUT) {
		printRawOutput();
    }

    if(timesReusedBeforeEvicted == 0) {
    	WasteRecordCollection::
            addZeroReuseRecord(accessLog, address);
    }

    if((float)(bytesUsed->count()) / (float)lineSize < LOW_UTIL_THRESHOLD) {
    	WasteRecordCollection::
        addLowUtilRecord(accessLog, address, bytesUsed->count());
    }
	clearLine();
}

void CacheLine::printRawOutput() {
	cout << left
			<< setw(15) << bytesUsed->count()
			<< setw(25)	<< timesReusedBeforeEvicted
//			<< setw(45) << accessSite << "<"
//			<< varInfo  << setw(25) << ">"
			<< setw(0)	<< "[" << "0x" << hex << address << dec << "]" << endl;
}

size_t CacheLine::getVirtualTimeStamp() {
	return virtualTimeStamp;
}
void CacheLine::printParams() {
    cout << "Line size = " << lineSize << endl;
}
