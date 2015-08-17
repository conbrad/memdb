/*
 * This tool reads a file containing a memtracker trace, the text version. 
 * The trace has the following format:
 * <access_type> <tid> <addr> <size> <func> <access_source> <alloc_source> <name> <type>
 *
 * It runs the trace through a very simple cache simulator, whose size, associativity
 * and the block-size can be configured via command line options, and for each evicted
 * cache line outputs a record showing:
 * - The number of bytes that were used in that cache line between the time it was
 *   created and evicted.
 * - The number of times the cache line was reused. 
 * - The source code location, which caused this cache line to be created in the cache.
 * - The information on the variable that was accessed upon the faulting access. 
 */

#include <assert.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <sys/time.h>
#include <sstream> 
#include <map>
#include <utility>
#include <bitset>
#include <ctgmath>
#include <algorithm>
#include <unordered_map>
#include <tuple>
#include <memory>

#include "cache-waste-analysis.h"
#include "waste-record-collection.h"
#include "variable/variable-analyzer.h"

/* The following data structures are used to summarize
 * the cache waste per source location.
 */
#include "record/waste-record.h"
#include "record/low-util-record.h"
#include "record/zero-reuse-record.h"

/* These data structures relate to cache simulation.
 * For each line in the cache, we keep track of the address,
 * the corresponding source code location that performed the
 * access and whatever we know about the variable name and type,
 * the number of bytes that were actually used before the cache
 * line was evicted and how many times the cache lines was used
 * before being evicted.
 */
#include "cache-sim/cache.h"
#include "cache-sim/cache-line.h"
#include "cache-sim/cache-set.h"

using namespace std;


multimap <int, tuple<string, vector<LowUtilRecord>>> groupedLowUtilMap;


/* We are assuming the memtracker trace output, the text
 * version. It has the following format:
 *
 * <access_type> <tid> <addr> <size> <func>
 * <access_source> <alloc_source> <name> <type>
 */
const int TID = 1;
const int ADDRESS = 2;
const int SIZE = 3;
const int FUNCTION = 4;
const int ACCESS_SOURCE = 5;
const int ALLOC_SOURCE = 6;
const int NAME = 7;
const int TYPE = 8;

CacheWasteAnalysis::CacheWasteAnalysis(int numSets, int assoc, int cacheLineSize) {
	word = accessSite = varInfo = "";
	accessSize = 0;
    address = 0;
    cache = new Cache(numSets, assoc, cacheLineSize);
    cache->printParams();
}

CacheWasteAnalysis::~CacheWasteAnalysis() {
	delete cache;
}

void CacheWasteAnalysis::parseAndSimulate(string line) {
    istringstream str(line);
    string word;
    size_t address;
    unsigned short accessSize;
    string accessSite;
    string varInfo;

    /* Let's determine if this is an access record */
    if(str.eof()) {
    	return;
    }

    str >> word;
    if(!(word.compare("read:") == 0) && !(word.compare("write:") == 0)) {
    	return;
    }

    int lineColumn = TID;
    while(!str.eof()) {
		str >> word;

		switch(lineColumn++) {
			case TID:	    // Skip the tid
				break;
			case ADDRESS:     // Parse the address
				address = strtol(word.c_str(), 0, 16);
				if(errno == EINVAL || errno == ERANGE) {
					cerr << "The following line caused error when parsing address: " << endl;
					cerr << line << endl;
					exit(-1);
				}
				break;
			case SIZE:     // Parse the size
				accessSize = (unsigned short) strtol(word.c_str(), 0, 10);
				if(errno == EINVAL || errno == ERANGE) {
					cerr << "The following line caused error when parsing access size: " << endl;
					cerr << line << endl;
					exit(-1);
				}
				break;
			case FUNCTION:
			case ACCESS_SOURCE:
				accessSite += word + " ";
				break;
			case ALLOC_SOURCE:
			case NAME:
			case TYPE:
				varInfo += word + " ";
				break;
		}
    }

    if(VERBOSE) {
    	verboseOutput(line);
    }

    cache->access(address, accessSize, accessSite, varInfo);
}
void CacheWasteAnalysis::analyzeVariableAccesses() {
	VariableAnalyzer::analyzeVariables(cache->getNumSets(), cache->getAssoc(), cache->getCacheLineSize());
}

void CacheWasteAnalysis::printFunctionAccessCount() {
	cache->printFunctionAccessCount();
}
void CacheWasteAnalysis::printLineAccesses() {
	cache->printLineAccesses();
}
void CacheWasteAnalysis::printWasteMaps() {
	WasteRecordCollection::printWasteMaps();
}

void CacheWasteAnalysis::summarizeZeroReuseMap() {
	WasteRecordCollection::summarizeZeroReuseMap();
}

void CacheWasteAnalysis::summarizeLowUtilMap() {
	WasteRecordCollection::summarizeLowUtilMap();
}

void CacheWasteAnalysis::inputError() {
	cout << "Error parsing input: " << word << endl;
}

void CacheWasteAnalysis::verboseOutput(const string& line) {
	cout << line << endl;
	cout << "Parsed: " << endl;
	cout << hex << "0x" << address << dec << endl;
	cout << accessSize << endl;
	cout << accessSite << endl;
	cout << varInfo << endl;
}






