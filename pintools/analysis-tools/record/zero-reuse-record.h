/*
 * zero-reuse-record.h
 *
 *  Created on: Jun 9, 2015
 *      Author: conor
 */

#ifndef ZERO_REUSE_RECORD_H_
#define ZERO_REUSE_RECORD_H_

#include "waste-record.h"
#include <string>

class ZeroReuseRecord: public WasteRecord {
private:
	int accessCount;
public:
    ZeroReuseRecord(std::string varInfo, std::size_t address);
    ~ZeroReuseRecord();
    friend std::ostream& operator<< (std::ostream& stream, const ZeroReuseRecord& zeroReuseRecord);
};

#endif /* ZERO_REUSE_RECORD_H_ */
