/*
 * waste-record-collection.h
 *
 *  Created on: Jul 4, 2015
 *      Author: conor
 */

#ifndef WASTE_RECORD_COLLECTION_H_
#define WASTE_RECORD_COLLECTION_H_

#include "record/zero-reuse-record.h"
#include "record/low-util-record.h"
#include "util/binaryinstrumentation.h"
#include <unordered_map>

class WasteRecordCollection {
private:
	WasteRecordCollection(){}	// Disallow instantiation
public:
	static void cacheMiss();
	static void addZeroReuseRecord(logentry accessLog, std::size_t address);
	static void addLowUtilRecord(logentry accessLog, std::size_t address, int byteUseCount);
	static void printWasteMaps();
	static void summarizeZeroReuseMap();
	static void summarizeLowUtilMap();
	static void printMissTotal();
};


#endif /* WASTE_RECORD_COLLECTION_H_ */
