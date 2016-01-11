/*
 * <old>
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
 * </old>
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
#include "util/access-log-receiver.h"

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

/* <old>
 * We are assuming the memtracker trace output, the text
 * version. It has the following format:
 *
 * <access_type> <tid> <addr> <size> <func>
 * <access_source> <alloc_source> <name> <type>
 * </old>
 
const int TID = 1;
const int ADDRESS = 2;
const int SIZE = 3;
const int FUNCTION = 4;
const int ACCESS_SOURCE = 5;
const int ALLOC_SOURCE = 6;
const int NAME = 7;
const int TYPE = 8;
 */

CacheWasteAnalysis::CacheWasteAnalysis(int numSets, int assoc, int cacheLineSize) {
    cache = new Cache(numSets, assoc, cacheLineSize);
    cache->printParams();
}

void CacheWasteAnalysis::simulate(logentry accessLog) {
    size_t address = (size_t) accessLog.entry.access.ptr;

    // TODO getting the wrong size?
    unsigned short accessSize = (unsigned short) AccessLogReceiver::sizeOf(accessLog.entry.access);

    cache->access(address, accessSize, accessLog);
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

void CacheWasteAnalysis::printMissTotal() {
	WasteRecordCollection::printMissTotal();
}

void CacheWasteAnalysis::printNumAccesses() {
    cout << "-----------------" << endl;
    cout << "Number of Accesses" << endl;
    cout << cache->getNumAccesses() << endl;
    cout << "-----------------" << endl;
}

void CacheWasteAnalysis::printTotalBytes() {
    cout << cache->getTotalBytesBroughtIn() << endl;
}

void CacheWasteAnalysis::printWastedBytes() {
    cout << cache->getTotalBytesWasted() << endl;
}

void CacheWasteAnalysis::printNumMisses() {
    cout << cache->getNumMisses() << endl;
}

CacheWasteAnalysis::~CacheWasteAnalysis() {
    delete cache;
}
