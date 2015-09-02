#include "waste-record-collection.h"
#include "util/map-summarizer.h"
#include "util/access-parser.h"
#include <iostream>
#include <algorithm>
#include <cstdint>

using namespace std;
const string UNKNOWN_PATH = "<unknown>";

namespace wasteRecordMaps {
	map <std::string, ZeroReuseRecord> zeroReuseMap;
	map <std::string, LowUtilRecord> lowUtilMap;
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

void WasteRecordCollection::cacheMiss() {
	wasteRecordMaps::cacheMisses++;
}

void WasteRecordCollection::addZeroReuseRecord(string accessSite, string variableInfo, size_t address) {
	if(accessSite.compare(UNKNOWN_PATH) == 0) {
		return;
	}

	string variableName = AccessParser::variableNameFromInfo(variableInfo);
	map<string, ZeroReuseRecord>::iterator found = wasteRecordMaps::zeroReuseMap.find(accessSite + variableName);

	if(found != wasteRecordMaps::zeroReuseMap.end()) {

	} else {
		ZeroReuseRecord record(variableInfo, address);
			wasteRecordMaps::zeroReuseMap.insert(
			    		pair<string, ZeroReuseRecord>
			    		(accessSite + variableName, record));
	}
}

void WasteRecordCollection::addLowUtilRecord(string accessSite, string variableInfo, size_t address, int byteUseCount) {
	if(accessSite.compare(UNKNOWN_PATH) == 0) {
			return;
	}

	string variableName = AccessParser::variableNameFromInfo(variableInfo);
	map<string, LowUtilRecord>::iterator found = wasteRecordMaps::lowUtilMap.find(accessSite + variableName);

	if(found != wasteRecordMaps::lowUtilMap.end()) {
		found->second.accessed();
	} else {
		LowUtilRecord record(variableInfo, address, byteUseCount);
			wasteRecordMaps::lowUtilMap.insert(
			    		pair<string, LowUtilRecord>
			    		(accessSite + variableName, record));
	}
}

void WasteRecordCollection::printWasteMaps() {
	cout << "*************************************************" << endl;
	cout << "               ZERO REUSE MAP                    " << endl;
	cout << "*************************************************" << endl;
	for (auto &it : wasteRecordMaps::zeroReuseMap) {
		cout << it.first << endl;
		cout << it.second << endl;
	}

	cout << "*************************************************" << endl;
	cout << "               LOW UTIL MAP                      " << endl;
	cout << "*************************************************" << endl;
	for (auto &it : wasteRecordMaps::lowUtilMap) {
		cout << it.first << endl;
		cout << it.second << endl;
	}
}

void WasteRecordCollection::summarizeZeroReuseMap() {
	cout << "*************************************************" << endl;
	cout << "         ZERO REUSE MAP SUMMARIZED               " << endl;
	cout << "*************************************************" << endl;
	for(auto &kv : wasteRecordMaps::zeroReuseMap) {
		cout << "Path: " << kv.second.getPath() << endl;
		cout << "Variable: " << kv.second.getVariableName() << endl;
		cout << "Type: " << kv.second.getType() << endl;
		cout << "Access count: " << kv.second.getAccessCount() << "\n" << endl;
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
		cout << "Path: " << kv.second.getPath() << endl;
		cout << "Variable: " << kv.second.getVariableName() << endl;
		cout << "Type: " << kv.second.getType() << endl;
		cout << "Access count: " << kv.second.getAccessCount() << "\n" << endl;
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
