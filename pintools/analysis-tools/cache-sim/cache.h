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

public:
	// TODO make private for safety
    CacheSet *sets;
    Cache(int numSets, int assoc, int lineSize);
    void access(size_t address, unsigned short accessSize, std::string accessSite, std::string varInfo);
    void printParams();
};
#endif /* CACHE_H_ */
