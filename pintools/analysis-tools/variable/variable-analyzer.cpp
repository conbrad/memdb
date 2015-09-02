/*
 * FunctionAnalyzer.cpp
 *
 *  Created on: Jul 5, 2015
 *      Author: conor
 */

#include "variable-analyzer.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>

using namespace std;

map<string, VariableAccess> variables;
const string UNKNOWN_PATH = "<unknown>";

void VariableAnalyzer::addVariable(VariableAccess variableAccess) {
	map<string, VariableAccess>::iterator found = variables.find(
			variableAccess.getPath() +
			variableAccess.getName());

	if(variableAccess.getPath().compare(UNKNOWN_PATH) == 0) {
		return;
	}

	if(found != variables.end()) {
		found->second.called();
	} else {

		pair <string, VariableAccess> variable (variableAccess.getPath() + variableAccess.getName(), variableAccess);
		variables.insert(variable);
	}
}

void VariableAnalyzer::analyzeVariables(int numSets, int assoc, int cacheLineSize) {
	structify(numSets, assoc, cacheLineSize);
}

void VariableAnalyzer::structify(int numSets, int assoc, int cacheLineSize) {
	//cout << variableAccess.second.getName() << ", count: " << variableAccess.second.getTimesCalled() << endl;

	cout << "Separate these variables into the following structs to minimize cache misses\n" << endl;

	int currentSize = 0;
	int structCount = 1;

	for(auto &variable : variables) {
		if(currentSize == 0) {
			cout << "struct " << "struct" << structCount << " {" << endl;
			structCount++;
		}

		if(currentSize + variable.second.getSize() < cacheLineSize) {
			currentSize += variable.second.getSize();
		} else {
			cout << "}\n" << endl;
			structCount++;
			cout << "struct " << "struct" << structCount << " {" << endl;
			currentSize = 0 + variable.second.getSize();
		}
		cout << "\t /* line: " << variable.second.getLine();
		cout << ", column: " << variable.second.getCol() << " */ " << endl;
		cout << "\t" << parseType(variable.second.getType()) << " " << variable.second.getName() << ";" << endl;
	}

	cout << "}" << endl;
	cout << "--------------------------------------------" << endl;
}

string VariableAnalyzer::parseType(const string loggedType) {
	if(loggedType.find("i32") != std::string::npos) {
		return "int";
	}
	if(loggedType.find("float") != std::string::npos) {
		return "float";
	}
	return loggedType;
}
