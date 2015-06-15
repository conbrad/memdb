/*
 * map-summarizer.h
 *
 *	Static, stateless methods for summarizing waste and low-util maps
 *
 *  Created on: Jun 12, 2015
 *      Author: conor
 */

#ifndef SUMMARIZER_H_
#define SUMMARIZER_H_

#include <unordered_map>
#include <map>
#include <tuple>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class MapSummarizer {
private:
	MapSummarizer(){}	// disallows instantiation
public:
	/* Iterate the map. Once we encounter a new source line,
	 * count the number of its associated waste records,
	 * put that in the summarized map, where the count is the key,
	 * and the value is the list (vector) of associated waste records.
	 */
	template <class T>
	static void summarizeMap(std::unordered_multimap<std::string, T> &ungroupedMap,
			       	  	  	 std::multimap<int, std::tuple<std::string, std::vector<T>>> &groupedMap) {

		for(auto record = ungroupedMap.begin(); record != ungroupedMap.end(); record++) {
			std::vector<T> curVector;
			std::string curAccessSite = record->first;

			do {
				curVector.push_back(record->second);
				++record;
			} while (record != ungroupedMap.end() && curAccessSite.compare(record->first)==0);

			std::tuple<std::string, std::vector<T>> gRecs = make_tuple(curAccessSite, curVector);
			groupedMap.insert(make_pair(curVector.size(), gRecs));

			if(record == ungroupedMap.end())
			  break;
	    }
	}

	template <class T>
	static void printSummarizedMap(std::multimap<int, std::tuple<std::string, std::vector<T>>> &groupedMap)
	{
	    for(auto it = groupedMap.rbegin(); it != groupedMap.rend(); it++)
	    {
	        cout << it->first << " waste occurrences" << endl;

	        std::tuple<std::string, std::vector<T>> gRecs = it->second;

	        std::string accessSite = get<0>(gRecs);
	        std::vector<T> recs = get<1>(gRecs);

	        cout << accessSite << endl;

	        for(int i = 0; i < recs.size(); i++) {
	        	cout << recs[i] << endl;
	        }
	    }
	}
};

#endif /* SUMMARIZER_H_ */
