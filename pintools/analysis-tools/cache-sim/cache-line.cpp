#include "cache-line.h"
#include <bitset>
#include <iostream>
#include <assert.h>
#include <iomanip>
#include <string>

#include "../main.h"
#include "../util/map-summarizer.h"
#include "../waste-record-collection.h"
#include "function-location.h"

using namespace std;

const float LOW_UTIL_THRESHOLD = 0.5;

static map<string, int> functionAccessCount;

map<int, vector<CacheLineAccess>> lineAccesses;
map<int, map<FunctionLocation, vector<int>>> lineFunctionAccesses;

typedef std::map<int, std::map<std::string, std::vector<CacheLineAccess>>> FunctionLineMap;
FunctionLineMap lineAccessFunctions;

// helpers
string getFunctionName(const char *str);

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
	virtualTimeStamp = 0;
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

void CacheLine::recordLineAccess(int lineOffset, const string& functionAndPath, const CacheLineAccess& cacheLineAccess) {
//	map<int, map<string, vector<CacheLineAccess>>>::iterator key = lineAccessFunctions.find(lineOffset);
//
//	if (key != lineAccessFunctions.end()) {
//		lineAccessFunctions[lineOffset][functionAndPath] = vector<CacheLineAccess>();
//	} else {
//		lineAccessFunctions[lineOffset][functionAndPath].push_back(cacheLineAccess);
//	}
}

void CacheLine::setAndAccess(size_t address, unsigned short accessSize, string accessSite, string varInfo, size_t timeStamp) {
    this->address = address;
    this->initAccessSize = accessSize;
    this->tag = address >> tagMaskBits;
    this->accessSite = accessSite;
    this->varInfo = varInfo;
    this->timesReusedBeforeEvicted = 0;
    this->bytesUsed->reset();

    int lineOffset = access(address, accessSize, timeStamp);
    CacheLineAccess cacheLineAccess = {varInfo, accessSite, timeStamp, accessSize};
    FunctionLocation functionLocation = { getFunctionName(accessSite.c_str()), accessSite };
    string functionAndPath = getFunctionName(accessSite.c_str()) + "-" + accessSite;
	recordLineAccess(lineOffset, functionAndPath, cacheLineAccess);
    incrementFunctionCount(accessSite);
}

string getFunctionName(const char *str) {
    char c = ' ';

    do
    {
        const char *begin = str;

        while(*str != c && *str)
            str++;

        return string(begin, str);
    } while (0 != *str++);
    return str;
}

void CacheLine::incrementFunctionCount(string functionName) {
	map<string,int>::iterator functionKey = functionAccessCount.find(functionName);
	if(functionKey != functionAccessCount.end()) {
		functionKey->second = ++functionKey->second;
	} else {
		functionAccessCount.insert(make_pair(functionName, 1));
	}
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
	accessSite = "";
	varInfo = "";
	timesReusedBeforeEvicted = 0;
	bytesUsed->reset();
}

bool CacheLine::isHotFunction(string accessSite) {
	map<string,int>::iterator functionKey = functionAccessCount.find(accessSite);
	int count = functionKey->second;
	if(functionKey != functionAccessCount.end()) {
		return functionKey->second >= FUNCTION_CALL_THRESHOLD;
	}
	return false;
}

void CacheLine::evict() {

    /* We are being evicted. Print our stats, update waste maps and clear. */
    if(WANT_RAW_OUTPUT) {
		printRawOutput();
    }

    if(timesReusedBeforeEvicted == 0) {
    	WasteRecordCollection::addZeroReuseRecord(accessSite, varInfo, address);
    }

    if((float)(bytesUsed->count()) / (float)lineSize < LOW_UTIL_THRESHOLD) {
    	WasteRecordCollection::addLowUtilRecord(accessSite, varInfo, address, bytesUsed->count());
    }
	clearLine();
}

void CacheLine::printRawOutput() {
	cout << left
			<< setw(15) << bytesUsed->count()
			<< setw(25)	<< timesReusedBeforeEvicted
			<< setw(45) << accessSite << "<"
			<< varInfo  << setw(25) << ">"
			<< setw(0)	<< "[" << "0x" << hex << address << dec << "]" << endl;
}

void CacheLine::printLineAccesses() {
	cout << "*************************************************" << endl;
	cout << "              CACHE LINE ACCESSES             	  " << endl;
	cout << "*************************************************" << endl;
	cout << "# Function calls >=" << FUNCTION_CALL_THRESHOLD << endl;
	for (auto lineIt = lineAccesses.begin(); lineIt != lineAccesses.end(); lineIt++) {
		cout << "Line offset: " << lineIt->first << endl;
	    for (auto &cacheLineAccess : lineIt->second) {
	    	if(isHotFunction(cacheLineAccess.accessSite)) {
	    		cout 	<< "Function: " << cacheLineAccess.accessSite
	    				<< ", access size: " << cacheLineAccess.accessSize
	    				<< endl;
	    	}
	    }
	}
}

void CacheLine::printFunctionAccessCounts() {
	cout << "*************************************************" << endl;
	cout << "              FUNCTION ACCESS COUNTS             " << endl;
	cout << "                 <Name>, <Count>                 " << endl;
	cout << "*************************************************" << endl;

	for(auto &functionAccess : functionAccessCount) {
		cout 	<< getFunctionName(functionAccess.first.c_str())
				<< ", " << functionAccess.second
				<< endl;
	}
	cout << endl;
}
size_t CacheLine::getVirtualTimeStamp() {
	return virtualTimeStamp;
}
void CacheLine::printParams() {
    cout << "Line size = " << lineSize << endl;
}
