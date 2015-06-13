#include "cache-line.h"
#include <bitset>
#include <iostream>
#include <assert.h>

#include "../main.h"
#include "../util/map-summarizer.h"

using namespace std;

const float LOW_UTIL_THRESHOLD = 0.5;
std::unordered_multimap <std::string, ZeroReuseRecord> zeroReuseMap;
std::unordered_multimap <std::string, LowUtilRecord> lowUtilMap;
CacheLine::CacheLine() {
    	/* this is ugly, but C++ doesn't
    	 * allow to allocate an array and
    	 * initialize all members with the
    	 * same constructor at the same time.
    	*/

    	/* Size is given in bytes */
	    lineSize = CACHE_LINE_SIZE;
	    address = 0;
	    tag = 0;
	    tagMaskBits = 0;
	    initAccessSize = 0;
	    accessSite = "";
	    varInfo = "";
	    timesReusedBeforeEvicted = 0;
	    timeStamp = 0;
	    bytesUsed = new bitset<MAX_LINE_SIZE>(lineSize);
	    bytesUsed->reset();

}

/* Print info about the access that caused this line to be
 * brought into the cache */
void CacheLine::printFaultingAccessInfo() {
	    cout<< "0x" << hex << address << dec << " " << initAccessSize << " "
		<< accessSite << varInfo << endl;
}

void CacheLine::setAndAccess(size_t address, unsigned short accessSize, string accessSite, string varInfo, size_t timeStamp) {
    this->address = address;
    this->initAccessSize = accessSize;
    tag = address >> tagMaskBits;
    this->accessSite = accessSite;
    this->varInfo = varInfo;
    timesReusedBeforeEvicted = 0;
    bytesUsed->reset();

    access(address, accessSize, timeStamp);
}

bool CacheLine::valid(size_t address) {
    if(address >> tagMaskBits == tag)
    	return true;

    return false;
}

/* Set to '1' the bits corresponding to this address
 * within the cache line, to mark the corresponding bytes
 * as "accessed".
 * If those bits are already marked as accessed, we increment
 * the reuse counter.
 */
void CacheLine::access(size_t address, unsigned short accessSize, size_t timeStamp) {
    int lineOffset = address % lineSize;

    assert(valid(address));
    assert(lineOffset + accessSize <= lineSize);

    this->timeStamp = timeStamp;

    /* We only check if the first bit is set, assuming that if
     * we access the same valid address twice, the data represents
     * the same variable (and thus the same access size) as before
     */
    if(bytesUsed->test(lineOffset)) {
    	timesReusedBeforeEvicted++;
    } else {
    	for(int i = lineOffset; i < min(lineOffset + accessSize, lineSize); i++) {
    		bytesUsed->set(i);
    	}
    }
}

void CacheLine::resetLine() {
	address = 0;
	tag = 0;
	accessSite = "";
	varInfo = "";
	timesReusedBeforeEvicted = 0;
	bytesUsed->reset();
}

void CacheLine::evict() {

    /* We are being evicted. Print our stats, update waste maps and clear. */
    if(WANT_RAW_OUTPUT) {
    	cout << bytesUsed->count() << "\t" << timesReusedBeforeEvicted
    			<< "\t" << accessSite << "[" << varInfo << "]\t"
				<< "0x" << hex << address << dec << endl;
    }

    if(timesReusedBeforeEvicted == 0) {
    	zeroReuseMap.insert(pair<string, ZeroReuseRecord>
    					    (accessSite,
    					     ZeroReuseRecord(varInfo, address)));
    }

    if((float)(bytesUsed->count()) / (float)lineSize < LOW_UTIL_THRESHOLD) {
		lowUtilMap.insert(pair<string, LowUtilRecord>
				  (accessSite,
				   LowUtilRecord(varInfo, address, bytesUsed->count())));
    }
	resetLine();
}

void CacheLine::printZeroReuseSummary(multimap <int, tuple<string, vector<ZeroReuseRecord>>> groupedZeroReuseMap) {
	MapSummarizer::summarizeMap(zeroReuseMap, groupedZeroReuseMap);
	//MapSummarizer::printSummarizedMap(groupedZeroReuseMap);
}

void CacheLine::printParams() {
    cout << "Line size = " << lineSize << endl;
}
