/*
 * low-util-record.h
 *
 *  Created on: Jun 9, 2015
 *      Author: conor
 */

#ifndef LOW_REUSE_RECORD_H_
#define LOW_REUSE_RECORD_H_

#include "waste-record.h"
#include "../util/binaryinstrumentation.h"
#include <string>

class LowUtilRecord: public WasteRecord {
private:
    logentry log;
	int byteUseCount;

public:
	LowUtilRecord(logentry log, int byteUsedCount);
	~LowUtilRecord();
	friend std::ostream& operator<< (std::ostream& stream, const LowUtilRecord& lowReuseRecord);
};

#endif /* LOW_REUSE_RECORD_H_ */
