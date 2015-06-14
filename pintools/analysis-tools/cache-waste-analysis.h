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
	void inputError();

public:
	CacheWasteAnalysis(int numSets, int assoc, int cacheLineSize);
	~CacheWasteAnalysis();
	void parseAndSimulate(std::string line);
	void zeroReuseDetail();
	void zeroReuseSummary();
	void lowUtilDetail();
	void lowUtilSummary();
	std::unordered_multimap <std::string, LowUtilRecord> getLowUtilMap();
	std::unordered_multimap <std::string, ZeroReuseRecord> getZeroReuseMap();
};

#endif /* CACHE_WASTE_ANALYSIS_H_ */
