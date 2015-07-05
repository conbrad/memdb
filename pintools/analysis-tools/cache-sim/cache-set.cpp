#include "cache-set.h"
#include <assert.h>
#include <iostream>
#include "../main.h"

using namespace std;

CacheSet::CacheSet() {
	    associativity = ASSOC;
	    cacheLineSize = CACHE_LINE_SIZE;
	    currentTime = 0;
	    cacheLine = new CacheLine[associativity];
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
    	if(cacheLine[i].getVirtualTimeStamp() < minTime) {
    		minTime = cacheLine[i].getVirtualTimeStamp();
    		minIndex = i;
    	}

		#if VERBOSE
    		cout << "block "<< i << " ts is " << cacheLine[i].virtualTimeStamp << endl;
		#endif
    }
    assert(minIndex >= 0);

	#if VERBOSE
    	cout << "Eviction candidate is block " << minIndex << endl;
	#endif

    /* Evict the line if it's not empty */
    if(cacheLine[minIndex].getVirtualTimeStamp() != 0) {
    	cacheLine[minIndex].evict();
    }
    return &(cacheLine[minIndex]);
}

bool CacheSet::cacheHit(size_t address, unsigned short accessSize) {
	for (int i = 0; i < associativity; i++) {
		if (cacheLine[i].valid(address)) {
			cacheLine[i].access(address, accessSize, currentTime);
			return true;
		}
	}
	return false;
}
void CacheSet::cacheMiss(size_t address, unsigned short accessSize, std::string accessSite, std::string varInfo) {
	    // See if there is an empty cache line or find someone to evict
	    CacheLine *line = findCleanOrVictim(currentTime);
	    line->setAndAccess(address, accessSize, accessSite, varInfo, currentTime);
}
void CacheSet::access(size_t address, unsigned short accessSize, std::string accessSite, std::string varInfo) {
    currentTime++;

	if(!cacheHit(address, accessSize)) {
		cacheMiss(address, accessSize, accessSite, varInfo);
	}
}
CacheLine* CacheSet::getCacheLine() {
	return cacheLine;
}
