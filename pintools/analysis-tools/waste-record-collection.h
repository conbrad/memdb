/*
 * waste-record-collection.h
 *
 * Static, stateless methods for collecting waste records
 *
 *  Created on: Jul 4, 2015
 *      Author: conor
 */

#ifndef WASTE_RECORD_COLLECTION_H_
#define WASTE_RECORD_COLLECTION_H_

#include "record/zero-reuse-record.h"
#include "record/low-util-record.h"
#include <unordered_map>

class WasteRecordCollection {
private:
	WasteRecordCollection(){}	// Disallow instantiation
public:

	static void addZeroReuseRecord(std::string accessSite, std::string variableInfo, std::size_t address);
	static void addLowUtilRecord(std::string accessSite, std::string variableInfo, std::size_t address, int byteUseCount);
	static void printWasteMaps();
	static void summarizeZeroReuseMap();
	static void summarizeLowUtilMap();
};


#endif /* WASTE_RECORD_COLLECTION_H_ */
