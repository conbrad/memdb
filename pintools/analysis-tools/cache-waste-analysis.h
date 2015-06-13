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
    std::string accessSite;
    std::string varInfo;
    Cache *cache;
    size_t address;
    unsigned short accessSize;
	bool accessRecord(std::istringstream& str);
	void verboseOutput(const std::string& line);
	void parseSize(const std::string& line);
	void parseAddress(const std::string& line);
	void parseAccessSite();
	void parseVarInfo();
	void inputError();

public:
	CacheWasteAnalysis(int numSets, int assoc, int cacheLineSize);
	~CacheWasteAnalysis();
	void parseAndSimulate(std::string line);
	void zeroReuseSummary();
};

#endif /* CACHE_WASTE_ANALYSIS_H_ */
