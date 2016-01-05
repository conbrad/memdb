#include "low-util-record.h"
#include <iostream>
#include "../main.h"
#include "../util/access-parser.h"

using namespace std;

LowUtilRecord::LowUtilRecord(logentry log, int byteUseCount) {
	    this->log = log;
        this->byteUseCount = byteUseCount;
}

std::ostream& operator<< (std::ostream& stream, const LowUtilRecord& lowReuseRecord) {
    if(lowReuseRecord.log.entry_type == LOG_ACCESS) {
        cout << "\t" << lowReuseRecord.log.entry.access.varId << endl;
    	cout << "\t0x" << hex << lowReuseRecord.log.entry.access.ptr << dec << endl;
    }
    if(lowReuseRecord.log.entry_type == LOG_ALLOC) {
        cout << "\t" << lowReuseRecord.log.entry.alloc.size << endl;
    	cout << "\t0x" << hex << lowReuseRecord.log.entry.alloc.addr << dec << endl;
    }   
}

LowUtilRecord::~LowUtilRecord(){}
