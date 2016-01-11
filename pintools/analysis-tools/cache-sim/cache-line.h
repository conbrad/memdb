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
#include <map>
#include <tuple>
#include <vector>
#include "../util/binaryinstrumentation.h"
#include "../record/zero-reuse-record.h"
#include "../record/low-util-record.h"
#include "../main.h"
#include "cache-line-access.h"
#include "function-location.h"

const float LOW_UTIL_THRESHOLD = 0.5;
class CacheLine {

private:
    int id;
    int lineSize;						// in bytes
    int tagMaskBits;					 

    std::size_t address;				// virtual address responsible for populating this cache line */
    std::size_t tag;
    std::size_t timestamp;		        // virtual time of access
    unsigned short initAccessSize;	    // size of the access that brought this line into cache
    unsigned short reused;

    // Bit for each byte in cache line, if byte is accessed, set it
    // If already set, it has been accessed before
    std::bitset<MAX_LINE_SIZE> *bytesUsed;

public:

    CacheLine();
    void setTag(size_t address, unsigned short accessSize);
    void evict();
    void setLineID(int id);
    bool valid(size_t address);
    bool isClean();
    int access(size_t address, unsigned short accessSize, size_t timeStamp);
    int getSize();
    int unused();
    int getLineID();
    std::size_t getTimestamp();
    std::size_t amountUsed();
    void printParams();
};

#endif /* CACHE_LINE_H_ */
