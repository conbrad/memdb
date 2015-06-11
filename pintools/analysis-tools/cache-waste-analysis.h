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

class CacheWasteAnalysis {
private:
    std::string word;
    size_t address;
    unsigned short accessSize;
    std::string accessSite;
    std::string varInfo;
    Cache *cache;
	void verboseOutput(std::string line);
public:
	CacheWasteAnalysis(int numSets, int assoc, int cacheLineSize);
	~CacheWasteAnalysis();
	void parseAndSimulate(std::string line);
};

#endif /* CACHE_WASTE_ANALYSIS_H_ */
