/*
 * VariableAccess.cpp
 *
 *  Created on: Jul 5, 2015
 *      Author: conor
 */

#include "variable-access.h"
#include <string>

using namespace std;

VariableAccess::VariableAccess(VariableDetails variableDetails) {
	this->variableDetails = variableDetails;
	this->timesCalled = 1;
}
int VariableAccess::getSize() {
	return variableDetails.size;
}
int VariableAccess::getLine() {
	return variableDetails.line;
}
int VariableAccess::getCol() {
	return variableDetails.col;
}

string VariableAccess::getName() {
	return variableDetails.name;
}

string VariableAccess::getType() {
	return variableDetails.type;
}

string VariableAccess::getPath() {
	return variableDetails.path;
}

int VariableAccess::getTimesCalled() {
	return timesCalled;
}

void VariableAccess::called() {
	timesCalled++;
}

VariableAccess::~VariableAccess() {
	// TODO Auto-generated destructor stub
}

