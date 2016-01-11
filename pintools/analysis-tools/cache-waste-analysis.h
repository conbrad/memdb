/*
 * cache-waste-analysis.h
 *
 *  Created on: Jun 9, 2015
 *      Author: conor
 */

#ifndef CACHE_WASTE_ANALYSIS_H_
#define CACHE_WASTE_ANALYSIS_H_

const int VERBOSE = 0;

#include "cache-sim/cache.h"
#include "util/binaryinstrumentation.h"

class CacheWasteAnalysis {
private:
    Cache *cache;

public:
	CacheWasteAnalysis(int numSets, int assoc, int cacheLineSize);
	~CacheWasteAnalysis();
	void simulate(logentry accessLog);
	// <unused>
    void printWasteMaps();
	void summarizeZeroReuseMap();
	void summarizeLowUtilMap();
    // </unused>
	void printMissTotal();
    void printTotalBytes();
    void printWastedBytes();
    void printNumAccesses();
    void printNumMisses();
};

#endif /* CACHE_WASTE_ANALYSIS_H_ */
