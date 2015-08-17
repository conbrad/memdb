#include "zero-reuse-record.h"
#include "../util/access-parser.h"
#include <iostream>
using namespace std;

ZeroReuseRecord::ZeroReuseRecord(std::string varInfo, std::size_t address) {
	mVarInfo = varInfo;
	mAddress = address;
	accessCount = 1;
	this->variableName = AccessParser::variableNameFromInfo(varInfo);
	this->type = AccessParser::typeFromInfo(varInfo);
}

std::ostream& operator<< (std::ostream& stream, const ZeroReuseRecord& zeroReuseRecord) {
	cout << "\t" << zeroReuseRecord.mVarInfo << endl;
	cout << "\t0x" << hex << zeroReuseRecord.mAddress << dec << endl;
}

ZeroReuseRecord::~ZeroReuseRecord(){}
