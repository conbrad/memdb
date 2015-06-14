#include "cache-set.h"
#include <assert.h>
#include <iostream>
#include "../main.h"

using namespace std;

CacheSet::CacheSet() {
	    associativity = ASSOC;
	    cacheLineSize = CACHE_LINE_SIZE;
	    currentTime = 0;
	    cacheLines = new CacheLine[associativity];
}

/* Find a cache line to evict or return a clean time
 * For evictions we use the true LRU policy based on
 * virtual timestamps.
 */
CacheLine* CacheSet::findCleanOrVictim(size_t timeNow) {

    size_t minTime = currentTime;
    size_t minIndex = -1;
	#if VERBOSE
    	cout << "Looking for eviction candidate at time " << timeNow << endl;
	#endif

    /* A clean line will have a timestamp of zero,
     * so it will automatically get selected.
     */
    for(int i = 0; i < associativity; i++) {
    	if(cacheLines[i].timeStamp < minTime) {
    		minTime = cacheLines[i].timeStamp;
    		minIndex = i;
    	}

		#if VERBOSE
    		cout << "block "<< i << " ts is " << cacheLines[i].timeStamp << endl;
		#endif
    }
    assert(minIndex >= 0);

	#if VERBOSE
    	cout << "Eviction candidate is block " << minIndex << endl;
	#endif

    /* Evict the line if it's not empty */
    if(cacheLines[minIndex].timeStamp != 0) {
    	cacheLines[minIndex].evict();
    }
    return &(cacheLines[minIndex]);
}

/* See if any of the existing cache lines hold
 * that address. If so, access the cache line.
 * Otherwise, find someone to evict and populate
 * the cache line with the new data
 */
void CacheSet::access(size_t address, unsigned short accessSize, std::string accessSite, std::string varInfo) {
    currentTime++;

    for(int i = 0; i < associativity; i++) {
    	if(cacheLines[i].valid(address)) {
    		cacheLines[i].access(address, accessSize, currentTime);
    		return;
    	}
    }

    /* If we are here, we did not find the data in cache.
     * See if there is an empty cache line or find someone to evict.
     */
    CacheLine *line = findCleanOrVictim(currentTime);
    line->setAndAccess(address, accessSize, accessSite, varInfo, currentTime);
}

void CacheSet::printParams() {
    cout << "Associativity = " << associativity << endl;
    cout << "Line size = " << cacheLineSize << endl;
}
