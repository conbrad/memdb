/*
 * zero-reuse-record.h
 *
 *  Created on: Jun 9, 2015
 *      Author: conor
 */

#ifndef WASTE_RECORD_H_
#define WASTE_RECORD_H_

#include <string>

class WasteRecord {

protected:
	std::string mVarInfo;
	std::size_t mAddress;

public:
	WasteRecord();
	WasteRecord(std::string varInfo, std::size_t address);
	virtual ~WasteRecord();
};


#endif /* WASTE_RECORD_H_ */
