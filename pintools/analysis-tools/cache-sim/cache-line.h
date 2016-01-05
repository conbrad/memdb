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
const int NULL_LOGENTRY = 0;
class CacheLine {

private:
    int lineSize;						// in bytes
    int tagMaskBits;					// in bytes

    std::size_t address;				// virtual address responsible for populating this cache line */
    std::size_t tag;
    std::size_t virtualTimeStamp;		// virtual time of access
    logentry accessLog;
    unsigned short initAccessSize;	    // The size of the access that brought this line into cache
    unsigned short timesReusedBeforeEvicted;
    static const int FUNCTION_CALL_THRESHOLD = 0;

    /* This is a bitmap. There is a bit for each byte in the
     * cache line. If a byte sitting in the cache line is
     * accessed by the user program, we mark it as "accessed"
     * by setting the corresponding bit to "1".
    */
    std::bitset<MAX_LINE_SIZE> *bytesUsed;
	void clearLine();
	void incrementFunctionCount(std::string functionName);
	void printRawOutput();

public:

    CacheLine();
    int access(size_t address, unsigned short accessSize, size_t timeStamp);
    bool valid(size_t address);
    void setAndAccess(size_t address, unsigned short accessSize, logentry accessLog, size_t timeStamp);
    void evict();
    int getSize();
    int unused();
    std::size_t getVirtualTimeStamp();
    std::size_t amountUsed();
    void printParams();
    void summarizeZeroReuseMap();
    void summarizeLowUtilMap();
    void printFunctionAccessCounts();
    void printLineAccesses();
    void printFaultingAccessInfo();
    void printWasteMaps();
    std::string printAmountUsed();
};

#endif /* CACHE_LINE_H_ */
