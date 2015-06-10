/*
 * cache-set.h
 *
 *  Created on: Jun 10, 2015
 *      Author: conor
 */

#ifndef CACHE_SET_H_
#define CACHE_SET_H_

#include <string>
#include "cache-line.h"

class CacheSet {
public:
    int assoc, lineSize;
    CacheLine *lines;

    /* a virtual time ticks every time someone
     * accesses this cache set. */
    size_t curTime;

    CacheSet();
    CacheLine* findCleanOrVictim(size_t timeNow);
    void access(size_t address, unsigned short accessSize, std::string accessSite, std::string varInfo);
    void printParams();
};

#endif /* CACHE_SET_H_ */
