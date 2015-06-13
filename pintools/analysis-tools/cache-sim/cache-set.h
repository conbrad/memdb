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
    void zeroReuseSummary(std::multimap <int, std::tuple<std::string, std::vector<ZeroReuseRecord>>> groupedZeroReuseMap);
};

#endif /* CACHE_SET_H_ */
