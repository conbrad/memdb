#include "waste-record.h"

WasteRecord::WasteRecord(): mVarInfo(""), mAddress(0) {
}

WasteRecord::WasteRecord(std::string varInfo, size_t address) {
	mVarInfo = varInfo;
	mAddress = address;
}

WasteRecord::~WasteRecord(){}
