/*
 * Function.cpp
 *
 *  Created on: Jul 5, 2015
 *      Author: conor
 */

#include "function.h"
#include <string>
#include <vector>
#include "variable-access.h"

using namespace std;

Function::Function(string name, string path) {
	this->name = name;
	this->path = path;
	this->variablesAccessed = vector<VariableAccess>();
}

string Function::getName() {
	return name;
}

string Function::getPath() {
	return path;
}

vector<VariableAccess> Function::getVariablesAccessed() {
	return variablesAccessed;
}

void Function::addVariableAccess(VariableAccess variable) {
	variablesAccessed.push_back(variable);
}
