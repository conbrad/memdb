#include "waste-record-collection.h"
#include "util/map-summarizer.h"
#include <iostream>

using namespace std;

namespace wasteRecordMaps {
	unordered_multimap <std::string, ZeroReuseRecord> zeroReuseMap;
	unordered_multimap <std::string, LowUtilRecord> lowUtilMap;
}

void WasteRecordCollection::addZeroReuseRecord(string accessSite, string variableInfo, size_t address) {
	    wasteRecordMaps::zeroReuseMap.insert(
	    		pair<string, ZeroReuseRecord>
	    		(accessSite, ZeroReuseRecord(variableInfo, address)));
}

void WasteRecordCollection::addLowUtilRecord(string accessSite, string variableInfo, size_t address, int byteUseCount) {
	    wasteRecordMaps::lowUtilMap.insert(
	    		pair<string, LowUtilRecord>
	    		(accessSite, LowUtilRecord(variableInfo, address, byteUseCount)));
}

void WasteRecordCollection::printWasteMaps() {
	cout << "*************************************************" << endl;
	cout << "               ZERO REUSE MAP                    " << endl;
	cout << "*************************************************" << endl;
	for (auto it = wasteRecordMaps::zeroReuseMap.begin(); it != wasteRecordMaps::zeroReuseMap.end(); it++) {
		cout << it->first << endl;
		cout << (ZeroReuseRecord&) (it->second) << endl;
	}
}

void WasteRecordCollection::summarizeZeroReuseMap() {
	cout << "*************************************************" << endl;
	cout << "         ZERO REUSE MAP SUMMARIZED               " << endl;
	cout << "*************************************************" << endl;

	multimap <int, tuple<string, vector<ZeroReuseRecord>>> groupedZeroReuseMap;
	MapSummarizer::summarizeMap(wasteRecordMaps::zeroReuseMap, groupedZeroReuseMap);
	MapSummarizer::printSummarizedMap(groupedZeroReuseMap);
}

void WasteRecordCollection::summarizeLowUtilMap() {
	cout << "*************************************************" << endl;
	cout << "         LOW UTILIZATION MAP SUMMARIZED          " << endl;
	cout << "*************************************************" << endl;

	multimap <int, tuple<string, vector<LowUtilRecord>>> groupedLowUtilMap;
	MapSummarizer::summarizeMap(wasteRecordMaps::lowUtilMap, groupedLowUtilMap);
	MapSummarizer::printSummarizedMap(groupedLowUtilMap);
}

