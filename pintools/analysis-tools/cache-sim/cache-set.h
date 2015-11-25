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
	int associativity;
	int cacheLineSize;
    size_t currentTime;	// a virtual time ticks every time someone accesses this cache set
	CacheLine *cacheLine;

	void attemptCacheAccess(size_t address, unsigned short accessSize);
	bool cacheHit(size_t address, unsigned short accessSize);
	void cacheMiss(size_t address, unsigned short accessSize, logentry accessLog);

public:
    CacheSet();
    CacheLine* findCleanOrVictim(size_t timeNow);
    CacheLine* getCacheLine();
    void printParams();
    void access(size_t address, unsigned short accessSize, logentry accessLog);
    void printZeroReuseDetail();
    void zeroReuseSummary(std::multimap <int, std::tuple<std::string, std::vector<ZeroReuseRecord>>> groupedZeroReuseMap);
    void printLowUtilDetail();
    void lowUtilSummary(std::multimap <int, std::tuple<std::string, std::vector<LowUtilRecord>>> groupedLowUtilMap);
};

#endif /* CACHE_SET_H_ */
