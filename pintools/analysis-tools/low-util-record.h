/*
 * low-util-record.h
 *
 *  Created on: Jun 9, 2015
 *      Author: conor
 */

#ifndef LOW_REUSE_RECORD_H_
#define LOW_REUSE_RECORD_H_

#include "waste-record.h"
#include <string>

class LowUtilRecord: public WasteRecord {
private:
	int mByteUseCount;

public:
	LowUtilRecord(std::string varInfo, std::size_t address, int byteUsedCount);
	~LowUtilRecord();
	friend std::ostream& operator<< (std::ostream& stream, const LowUtilRecord& lowReuseRecord);
};

#endif /* LOW_REUSE_RECORD_H_ */
