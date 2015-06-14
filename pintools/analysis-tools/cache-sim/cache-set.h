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

class CacheSet {
private:
	int associativity;
	int cacheLineSize;

public:
    CacheLine *cacheLines;

    /* a virtual time ticks every time someone
     * accesses this cache set. */
    size_t currentTime;

    CacheSet();
    void printParams();
    CacheLine* findCleanOrVictim(size_t timeNow);
    void access(size_t address, unsigned short accessSize, std::string accessSite, std::string varInfo);
    void printZeroReuseDetail();
    void zeroReuseSummary(std::multimap <int, std::tuple<std::string, std::vector<ZeroReuseRecord>>> groupedZeroReuseMap);
    void printLowUtilDetail();
    void lowUtilSummary(std::multimap <int, std::tuple<std::string, std::vector<LowUtilRecord>>> groupedLowUtilMap);
};

#endif /* CACHE_SET_H_ */
