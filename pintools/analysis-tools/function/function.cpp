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
#include "../util/access-parser.h"

using namespace std;

map<string, VariableAccess> variablesAccessed;

Function::Function(string name, string path) {
	this->name = name;
	this->path = AccessParser::stripLineNumbers(path);
	this->pathAndName = getPath() + getName();
	this->timesCalled = 0;
}

string Function::getName() {
	return name;
}

string Function::getPath() {
	return path;
}

string Function::getPathAndName() {
	return pathAndName;
}

int Function::getTimesCalled() {
	return timesCalled;
}

void Function::called() {
	timesCalled++;
}

map<string, VariableAccess> Function::getVariablesAccessed() {
	return variablesAccessed;
}

void Function::addVariableAccess(VariableAccess variable) {
	map<string, VariableAccess>::iterator found = variablesAccessed.find(variable.getName());

	if(found == variablesAccessed.end()) {
		pair <string, VariableAccess> variableAccess (variable.getName(), variable);
		variablesAccessed.insert(variableAccess);
	}
}
