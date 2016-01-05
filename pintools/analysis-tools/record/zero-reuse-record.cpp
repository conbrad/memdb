#include "zero-reuse-record.h"
#include "../util/binaryinstrumentation.h"
#include <iostream>
using namespace std;

ZeroReuseRecord::ZeroReuseRecord(logentry log) {
	accessCount = 1;
	this->log = log;
}

// TODO assumes access log, should handle alloc log
std::ostream& operator<< (std::ostream& stream, const ZeroReuseRecord& zeroReuseRecord) {
	if(zeroReuseRecord.log.entry_type == LOG_ACCESS) {
        cout << "\t" << zeroReuseRecord.log.entry.access.varId << endl;
    	cout << "\t0x" << hex << zeroReuseRecord.log.entry.access.ptr << dec << endl;
    }
    if(zeroReuseRecord.log.entry_type == LOG_ALLOC) {
        cout << "\t" << zeroReuseRecord.log.entry.alloc.size << endl;
    	cout << "\t0x" << hex << zeroReuseRecord.log.entry.alloc.addr << dec << endl;
    }
}

ZeroReuseRecord::~ZeroReuseRecord(){}
