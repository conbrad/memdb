/*
 * cache.h
 *
 *  Created on: Jun 10, 2015
 *      Author: conor
 */

#ifndef CACHE_H_
#define CACHE_H_

#include <string>
#include "cache-set.h"

class Cache {
private:
	int numSets;
	int assoc;
	int lineSize;
	int tagMaskBits;
	void __access(size_t address, unsigned short accessSize, std::string accessSite, std::string varInfo);
	void verboseSetOutput(size_t address, int setNum);
	void verboseSpanningAccessOutput(size_t address, unsigned short accessSize, std::string accessSite, std::string varInfo);
	void verboseSplitAccessOutput(size_t address, uint16_t bytesFittingIntoFirstLine);
	void verboseSpilledAccessOutput(size_t addressOfFirstByteNotFitting, uint16_t sizeOfSpillingAccess);

public:
	// TODO make private for safety
    CacheSet *set;
    Cache(int numSets, int assoc, int lineSize);
    void access(size_t address, unsigned short accessSize, std::string accessSite, std::string varInfo);
    void printParams();
    void printWasteMaps();
    void summarizeZeroReuseMap();
    void summarizeLowUtilMap();
    void printLineAccesses();
    void printFunctionAccessCount();
};
#endif /* CACHE_H_ */
