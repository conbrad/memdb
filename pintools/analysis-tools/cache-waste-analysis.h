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
    std::string word;
    std::string accessSite;
    std::string varInfo;
    Cache *cache;
    size_t address;
    unsigned short accessSize;
	void verboseOutput(const std::string& line);
	void inputError();

public:
	CacheWasteAnalysis(int numSets, int assoc, int cacheLineSize);
	~CacheWasteAnalysis();
	void parseAndSimulate(logentry accessLog);
	void printWasteMaps();
	void summarizeZeroReuseMap();
	void summarizeLowUtilMap();
    void printFullCacheLines();
	void printLineAccesses();
	void printFunctionAccessCount();
	void analyzeVariableAccesses();
	void printMissTotal();
};

#endif /* CACHE_WASTE_ANALYSIS_H_ */
