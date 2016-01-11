/*
 * cache-set.h
 *
 *  Created on: Jun 10, 2015
 *      Author: conor
 */

#ifndef CACHE_SET_H_
#define CACHE_SET_H_

#include <string>
#include <map>
#include <tuple>
#include <vector>

#include "cache-line.h"
#include "../record/zero-reuse-record.h"
#include "../record/low-util-record.h"
#include "../util/binaryinstrumentation.h"

class CacheSet {
private:
	CacheLine *cacheLine;
    enum AccessType { hit, miss };
    int id;
	int associativity;
	int cacheLineSize;
    int numMisses;
    size_t currentTime;	// a virtual time ticks every time someone accesses this cache set
    unsigned int bytesTransferred;
    unsigned int bytesWasted;
	bool cacheHit(size_t address, unsigned short accessSize);
	void cacheMiss(size_t address, unsigned short accessSize, logentry accessLog);
    void sendAccess(logentry accessLog, AccessType accessType);
public:
    CacheSet();
    CacheLine* getLRUCacheLine();
    CacheLine* getCacheLine();
    void setCacheSetID(int id);
    void setCacheLineIDs();
    void printParams();
    void access(size_t address, unsigned short accessSize, logentry accessLog);
    int getCacheSetID();
    unsigned int getBytesTransferred();
    unsigned int getBytesWasted();
    unsigned int getNumMisses();
};

#endif /* CACHE_SET_H_ */
