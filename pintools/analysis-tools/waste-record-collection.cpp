#include "waste-record-collection.h"
#include "util/map-summarizer.h"
#include "util/access-parser.h"
#include "util/binaryinstrumentation.h"
#include <iostream>
#include <algorithm>
#include <cstdint>

using namespace std;
const string UNKNOWN_PATH = "<unknown>";

namespace wasteRecordMaps {
	map <logentry, ZeroReuseRecord> zeroReuseMap;
	map <logentry, LowUtilRecord> lowUtilMap;
	uint64_t cacheMisses = 0;
	template <typename A, typename B>
	multimap<B, A> flip_map(map<A,B> & src) {
	    multimap<B,A> dst;

	    for(auto &kv : src) {
	        dst.insert(pair<B, A>(kv.second, kv.first));
	    }
	    return dst;
	}
}
// '<' operator defined so we can use
// logentry as a key in the waste record maps

bool operator < (const logentry &l, const logentry &r) { 
// if access log, use varId, 
// if alloc log use address? or line, col?
    if(l.entry_type != LOG_ACCESS) {
        printf("oops");
        exit(-1);
    }

    if(r.entry_type != LOG_ACCESS) {
        printf("oops");
        exit(-1);
    }

    return l.entry.access.varId < r.entry.access.varId;
}

void WasteRecordCollection::cacheMiss() {
	wasteRecordMaps::cacheMisses++;
}

void WasteRecordCollection::addZeroReuseRecord(logentry accessLog, size_t address) {
//	if(accessSite.compare(UNKNOWN_PATH) == 0) {
//		return;
//	}

	map<logentry, ZeroReuseRecord>::iterator found = wasteRecordMaps::zeroReuseMap.find(accessLog);

	if(found != wasteRecordMaps::zeroReuseMap.end()) {
        found->second.accessed();
	} else {
  	    ZeroReuseRecord record(accessLog);
  		    wasteRecordMaps::zeroReuseMap.insert(
  		    		pair<logentry, ZeroReuseRecord>
	    	    	(accessLog, record));
	}
}

// TODO handle logentry keys
void WasteRecordCollection::addLowUtilRecord(logentry accessLog, size_t address, int byteUseCount) {
//	if(accessSite.compare(UNKNOWN_PATH) == 0) {
//			return;
//	}
//
	map<logentry, LowUtilRecord>::iterator found = wasteRecordMaps::lowUtilMap.find(accessLog);

	if(found != wasteRecordMaps::lowUtilMap.end()) {
		found->second.accessed();
	} else {
		LowUtilRecord record(accessLog, byteUseCount);
			wasteRecordMaps::lowUtilMap.insert(
			    		pair<logentry, LowUtilRecord>
			    		(accessLog, record));
	}
}

void WasteRecordCollection::printWasteMaps() {
	cout << "*************************************************" << endl;
	cout << "               ZERO REUSE MAP                    " << endl;
	cout << "*************************************************" << endl;
	for (auto &it : wasteRecordMaps::zeroReuseMap) {
		//cout << it.first << endl;
		//cout << it.second << endl;
	}

	cout << "*************************************************" << endl;
	cout << "               LOW UTIL MAP                      " << endl;
	cout << "*************************************************" << endl;
	for (auto &it : wasteRecordMaps::lowUtilMap) {
		//cout << it.first << endl;
		//cout << it.second << endl;
	}
}

void WasteRecordCollection::summarizeZeroReuseMap() {
	cout << "*************************************************" << endl;
	cout << "         ZERO REUSE MAP SUMMARIZED               " << endl;
	cout << "*************************************************" << endl;
	for(auto &kv : wasteRecordMaps::zeroReuseMap) {
		//cout << "Path: " << kv.second.getPath() << endl;
		//cout << "Variable: " << kv.second.getVariableName() << endl;
		//cout << "Type: " << kv.second.getType() << endl;
		//cout << "Access count: " << kv.second.getAccessCount() << "\n" << endl;
	}
//	multimap <int, tuple<string, vector<ZeroReuseRecord>>> groupedZeroReuseMap;
//	MapSummarizer::summarizeMap(wasteRecordMaps::zeroReuseMap, groupedZeroReuseMap);
//	MapSummarizer::printSummarizedMap(groupedZeroReuseMap);
}

void WasteRecordCollection::summarizeLowUtilMap() {
	cout << "*************************************************" << endl;
	cout << "         LOW UTILIZATION MAP SUMMARIZED          " << endl;
	cout << "*************************************************" << endl;
	for(auto &kv : wasteRecordMaps::lowUtilMap) {
		//cout << "Path: " << kv.second.getPath() << endl;
		//cout << "Variable: " << kv.second.getVariableName() << endl;
		//cout << "Type: " << kv.second.getType() << endl;
		//cout << "Access count: " << kv.second.getAccessCount() << "\n" << endl;
	}
//	multimap <int, tuple<string, vector<LowUtilRecord>>> groupedLowUtilMap;
//	MapSummarizer::summarizeMap(wasteRecordMaps::lowUtilMap, groupedLowUtilMap);
//	MapSummarizer::printSummarizedMap(groupedLowUtilMap);
}
 void WasteRecordCollection::printMissTotal() {
	 cout << "*************************************************" << endl;
	 cout << "         		CACHE MISS TOTAL		           " << endl;
	 cout << "*************************************************" << endl;
	 cout << wasteRecordMaps::cacheMisses << endl;

 }
