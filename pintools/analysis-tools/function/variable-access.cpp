/*
 * VariableAccess.cpp
 *
 *  Created on: Jul 5, 2015
 *      Author: conor
 */

#include "variable-access.h"
#include <string>

using namespace std;

VariableAccess::VariableAccess(int size, int line, int col, string name) {
	this->size = size;
	this->line = line;
	this->col = col;
	this->name = name;
}
int VariableAccess::getSize() {
	return size;
}
int VariableAccess::getLine() {
	return line;
}
int VariableAccess::getCol() {
	return col;
}

string VariableAccess::getName() {
	return name;
}

VariableAccess::~VariableAccess() {
	// TODO Auto-generated destructor stub
}

