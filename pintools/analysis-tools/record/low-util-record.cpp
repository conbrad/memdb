#include "low-util-record.h"
#include <iostream>
#include "../main.h"
#include "../util/access-parser.h"

using namespace std;

LowUtilRecord::LowUtilRecord(std::string varInfo, std::size_t address, int byteUseCount) {
	    mVarInfo = varInfo;
	    mAddress = address;
	    mByteUseCount = byteUseCount;
	    this->variableName = AccessParser::variableNameFromInfo(varInfo);
	    this->type = AccessParser::typeFromInfo(varInfo);
}

std::ostream& operator<< (std::ostream& stream, const LowUtilRecord& lowReuseRecord) {
	cout << "\t--------------------------------------------" << endl;
	cout << "\t" << lowReuseRecord.mVarInfo << endl;
	cout << "\t0x" << hex << lowReuseRecord.mAddress << dec << endl;
	cout << "\t" << lowReuseRecord.mByteUseCount << "/" << CACHE_LINE_SIZE << endl;
}

LowUtilRecord::~LowUtilRecord(){}
