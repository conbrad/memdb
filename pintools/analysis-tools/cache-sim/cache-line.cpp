#include "cache-line.h"
#include <bitset>
#include <iostream>
#include <assert.h>
#include <iomanip>

#include "../main.h"
#include "../util/map-summarizer.h"

using namespace std;

const float LOW_UTIL_THRESHOLD = 0.5;

static map<string, int> functionAccessCount;

std::unordered_multimap <std::string, ZeroReuseRecord> zeroReuseMap;
std::unordered_multimap <std::string, LowUtilRecord> lowUtilMap;

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
	initAccessSize = 0;
	accessSite = "";
	varInfo = "";
	timesReusedBeforeEvicted = 0;
	timeStamp = 0;
	bytesUsed = new bitset<MAX_LINE_SIZE>(lineSize);
	bytesUsed->reset();
}

/* Print info about the access that caused this line to be
 * brought into the cache */
void CacheLine::printFaultingAccessInfo() {
	cout << "0x" << hex << address << dec << " "
		 << initAccessSize << " "
		 << accessSite << varInfo << endl;
}

void CacheLine::setAndAccess(size_t address, unsigned short accessSize, string accessSite, string varInfo, size_t timeStamp) {
    this->address = address;
    this->initAccessSize = accessSize;
    this->tag = address >> tagMaskBits;
    this->accessSite = accessSite;
    this->varInfo = varInfo;
    this->timesReusedBeforeEvicted = 0;
    this->bytesUsed->reset();

    access(address, accessSize, timeStamp);
    incrementFunctionCount(accessSite);
}

void CacheLine::incrementFunctionCount(string functionName) {
	map<string,int>::iterator functionKey = functionAccessCount.find(functionName);
	if(functionKey != functionAccessCount.end()) {
		functionKey->second = functionKey->second++;
	} else {
		functionAccessCount.insert(make_pair(functionName, 1));
	}
}

void CacheLine::printFunctionAccessCounts() {
	cout << "*************************************************" << endl;
	cout << "              FUNCTION ACCESS COUNTS             " << endl;
	cout << "*************************************************" << endl;

	for (auto it = functionAccessCount.begin(); it != functionAccessCount.end(); it++) {
		cout << "Function name: " << it->first << endl;
		cout << "Access count: " << it->second << endl;
		cout << endl;

	}
	cout << endl;
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
void CacheLine::access(size_t address, unsigned short accessSize, size_t timeStamp) {
    int lineOffset = address % lineSize;

    assert(valid(address));
    assert(lineOffset + accessSize <= lineSize);

    this->timeStamp = timeStamp;

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
}

void CacheLine::clearLine() {
	address = 0;
	tag = 0;
	accessSite = "";
	varInfo = "";
	timesReusedBeforeEvicted = 0;
	bytesUsed->reset();
}

void CacheLine::printRawOutput() {
	cout << left
			<< setw(15) << bytesUsed->count()
			<< setw(25)	<< timesReusedBeforeEvicted
			<< setw(45) << accessSite << "<"
			<< varInfo  << setw(25) << ">"
			<< setw(0)	<< "[" << "0x" << hex << address << dec << "]" << endl;
}

void CacheLine::evict() {

    /* We are being evicted. Print our stats, update waste maps and clear. */
    if(WANT_RAW_OUTPUT) {
		printRawOutput();
    }

    if(timesReusedBeforeEvicted == 0) {
    	zeroReuseMap.insert(
    			pair<string, ZeroReuseRecord>
    			(accessSite, ZeroReuseRecord(varInfo, address)));
    }

    if((float)(bytesUsed->count()) / (float)lineSize < LOW_UTIL_THRESHOLD) {
    	lowUtilMap.insert(
    			pair<string, LowUtilRecord>
				(accessSite, LowUtilRecord(varInfo, address, bytesUsed->count())));
    }
	clearLine();
}

void CacheLine::summarizeZeroReuseMap() {
	cout << "*************************************************" << endl;
	cout << "         ZERO REUSE MAP SUMMARIZED               " << endl;
	cout << "*************************************************" << endl;

	multimap <int, tuple<string, vector<ZeroReuseRecord>>> groupedZeroReuseMap;
	MapSummarizer::summarizeMap(zeroReuseMap, groupedZeroReuseMap);
	MapSummarizer::printSummarizedMap(groupedZeroReuseMap);
}

void CacheLine::summarizeLowUtilMap() {
	cout << "*************************************************" << endl;
	cout << "         LOW UTILIZATION MAP SUMMARIZED          " << endl;
	cout << "*************************************************" << endl;

	multimap <int, tuple<string, vector<LowUtilRecord>>> groupedLowUtilMap;
	MapSummarizer::summarizeMap(lowUtilMap, groupedLowUtilMap);
	MapSummarizer::printSummarizedMap(groupedLowUtilMap);
}

void CacheLine::printWasteMaps() {
	cout << "*************************************************" << endl;
	cout << "               ZERO REUSE MAP                    " << endl;
	cout << "*************************************************" << endl;
	for (auto it = zeroReuseMap.begin(); it != zeroReuseMap.end(); it++) {
		cout << it->first << endl;
		cout << (ZeroReuseRecord&) (it->second) << endl;
	}
	cout << endl;
	/* Print the waste maps */
	cout << "*************************************************" << endl;
	cout << "               LOW UTILIZATION MAP               " << endl;
	cout << "*************************************************" << endl;
	for (auto it = lowUtilMap.begin(); it != lowUtilMap.end(); it++) {
		cout << it->first << endl;
		cout << it->second << endl;
	}
}

void CacheLine::printParams() {
    cout << "Line size = " << lineSize << endl;
}
