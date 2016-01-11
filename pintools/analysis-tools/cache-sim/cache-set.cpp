#include "cache-set.h"
#include <assert.h>
#include <iostream>
#include "../main.h"
#include "../waste-record-collection.h"
#include "../util/access-sender.h"
#include "../util/binaryinstrumentation.h"

using namespace std;

CacheSet::CacheSet() {
	    associativity = ASSOC;
	    cacheLineSize = CACHE_LINE_SIZE;
	    currentTime = 0;
        bytesTransferred = 0;
        bytesWasted = 0;    
        numMisses = 0;
	    cacheLine = new CacheLine[associativity];
        setCacheLineIDs();
}

void CacheSet::setCacheSetID(int id) {
    this->id = id;
}

int CacheSet::getCacheSetID() {
    return id;
}

void CacheSet::setCacheLineIDs() {
    for(int i = 0; i < associativity; i++) {
        cacheLine[i].setLineID(i);
    }
}

// Return clean or evicted line, uses virtual timestamps for true LRU policy
CacheLine* CacheSet::getLRUCacheLine() {
    size_t minTime = currentTime;
    size_t minIndex = -1;

    for(int i = 0; i < associativity; i++) {
    	if(cacheLine[i].getTimestamp() < minTime) {
    		minTime = cacheLine[i].getTimestamp();
    		minIndex = i;
    	}
    }
    assert(minIndex >= 0);

    return &(cacheLine[minIndex]);
}

bool CacheSet::cacheHit(size_t address, unsigned short accessSize) {
    for (int i = 0; i < associativity; i++) {
		if (cacheLine[i].valid(address)) {
            assert(currentTime > 1);

			cacheLine[i].access(address, accessSize, currentTime);
			return true;
		}
	}
	return false;
}

void CacheSet::cacheMiss(size_t address, unsigned short accessSize, logentry accessLog) {
    numMisses++;

    CacheLine *line = getLRUCacheLine();
    
    if(!line->isClean()) {
        bytesWasted += line->unused();
        line->evict();
    }
    
    bytesTransferred += line->getSize();
	line->setTag(address, accessSize);
    line->access(address, accessSize, currentTime);
}

void CacheSet::access(size_t address, unsigned short accessSize, logentry accessLog) {
    currentTime++;
    AccessType accessType = miss;

    if(cacheHit(address, accessSize)) {
        accessType = hit;
    } else {
        cacheMiss(address, accessSize, accessLog);
        accessType = miss;
    }

    if(!SPARK_OFF) {
        sendAccess(accessLog, accessType);
    }
}

void CacheSet::sendAccess(logentry accessLog, AccessType accessType) {
    switch(accessType) {
        case hit:   
            AccessSender::sendHit(accessLog.entry.access);
        break;
        case miss:  
            AccessSender::sendMiss(accessLog.entry.access);
        break;
        default:
            cerr << "Access not a hit or miss!" << endl;
            exit(-1);
        break;
    }
}

CacheLine* CacheSet::getCacheLine() {
	return cacheLine;
}

unsigned int CacheSet::getBytesTransferred() {
    return bytesTransferred;
}

unsigned int CacheSet::getBytesWasted() {
    return bytesWasted;
}

unsigned int CacheSet::getNumMisses() {
    return numMisses;
}
