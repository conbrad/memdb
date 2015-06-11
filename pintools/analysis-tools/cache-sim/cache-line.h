/*
 * cache-line.h
 *
 *  Created on: Jun 10, 2015
 *      Author: conor
 */

#ifndef CACHE_LINE_H_
#define CACHE_LINE_H_

#include <string>
#include <bitset>
#include <unordered_map>
#include "../zero-reuse-record.h"
#include "../low-util-record.h"
#include "../main.h"

class CacheLine {

private:
	/* In bytes */
    int lineSize;
    int tagMaskBits;

    std::unordered_multimap <std::string, ZeroReuseRecord> zeroReuseMap;
    std::unordered_multimap <std::string, LowUtilRecord> lowUtilMap;
public:
    const float LOW_UTIL_THRESHOLD = 0.5;
    // TODO make fields private for safety
    /* virtual address responsible for populating this cache line */
    std::size_t address;
    std::size_t tag;
    /* which code location caused that data to be brought
     * into the cache line? */
    std::string accessSite;
    /* The size of the access that brought
     *this line into cache */
    unsigned short initAccessSize;
    /* the name and the type of the corresponding variable,
     * if we know it. */
    std::string varInfo;

    /* This is a bitmap. There is a bit for each byte in the
     * cache line. If a byte sitting in the cache line is
     * accessed by the user program, we mark it as "accessed"
     * by setting the corresponding bit to "1".
    */
    std::bitset<MAX_LINE_SIZE> *bytesUsed;

    /* Virtual time of access */
    size_t timeStamp;
    unsigned short timesReusedBeforeEvicted;

    CacheLine();
    void printFaultingAccessInfo();
    void setAndAccess(size_t address, unsigned short accessSize, std::string accessSite, std::string varInfo, size_t timeStamp);
    bool valid(size_t address);
    void access(size_t address, unsigned short accessSize, size_t timeStamp);
    void evict();
    void printParams();
};

#endif /* CACHE_LINE_H_ */
