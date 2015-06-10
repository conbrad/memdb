#include "cache-set.h"
#include <assert.h>
#include <iostream>
#include "../cache-waste-analysis.h"

using namespace std;

CacheSet::CacheSet() {
	    assoc = ASSOC;
	    lineSize = CACHE_LINE_SIZE;
	    curTime = 0;
	    this->assoc = assoc;
	    lines = new CacheLine[assoc];
}

/* Find a cache line to evict or return a clean time
 * For evictions we use the true LRU policy based on
 * virtual timestamps.
 */
CacheLine* CacheSet::findCleanOrVictim(size_t timeNow) {

    size_t minTime = curTime, minIndex = -1;
	#if VERBOSE
    	cout << "Looking for eviction candidate at time " << timeNow << endl;
	#endif

    /* A clean line will have a timestamp of zero,
     * so it will automatically get selected.
     */
    for(int i = 0; i < assoc; i++) {
    	if(lines[i].timeStamp < minTime) {
    		minTime = lines[i].timeStamp;
    		minIndex = i;
    	}

		#if VERBOSE
    		cout << "block "<< i << " ts is " << lines[i].timeStamp << endl;
		#endif
    }
    assert(minIndex >= 0);

	#if VERBOSE
    	cout << "Eviction candidate is block " << minIndex << endl;
	#endif

    /* Evict the line if it's not empty */
    if(lines[minIndex].timeStamp != 0) {
    	lines[minIndex].evict();
    }
    return &(lines[minIndex]);
}

/* See if any of the existing cache lines hold
 * that address. If so, access the cache line.
 * Otherwise, find someone to evict and populate
 * the cache line with the new data
 */
void CacheSet::access(size_t address, unsigned short accessSize, std::string accessSite, std::string varInfo) {
    curTime++;

    for(int i = 0; i < assoc; i++) {
    	if(lines[i].valid(address)) {
    		lines[i].access(address, accessSize, curTime);
    		return;
    	}
    }

    /* If we are here, we did not find the data in cache.
     * See if there is an empty cache line or find someone to evict.
     */
    CacheLine *line = findCleanOrVictim(curTime);
    line->setAndAccess(address, accessSize, accessSite, varInfo, curTime);
}

void CacheSet::printParams() {
    cout << "Associativity = " << assoc << endl;
    cout << "Line size = " << lineSize << endl;
}
