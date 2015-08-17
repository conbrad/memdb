#include "waste-record.h"

WasteRecord::WasteRecord(): mVarInfo(""), mAddress(0) {
	accessCount = 1;
}

WasteRecord::WasteRecord(std::string varInfo, size_t address) {
	mVarInfo = varInfo;
	mAddress = address;
	accessCount = 1;
}

void WasteRecord::accessed() {
	accessCount++;
}

int WasteRecord::getAccessCount() {
	return accessCount;
}

std::string WasteRecord::getVariableName() {
	return variableName;
}

std::string WasteRecord::getType() {
	return type;
}

std::string WasteRecord::getPath() {
	return path;
}
WasteRecord::~WasteRecord(){}
