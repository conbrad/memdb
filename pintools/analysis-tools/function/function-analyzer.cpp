/*
 * FunctionAnalyzer.cpp
 *
 *  Created on: Jul 5, 2015
 *      Author: conor
 */

#include "function-analyzer.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

map<string, Function> functionsAccessed;
const string UNKNOWN_PATH = "<unknown>";

void FunctionAnalyzer::addFunction(Function function, VariableAccess variableAccess) {
	map<string, Function>::iterator found = functionsAccessed.find(function.getPath() + function.getName());

	if(function.getPath().compare(UNKNOWN_PATH) == 0) {
		return;
	}

	if(found != functionsAccessed.end()) {
		found->second.called();
		found->second.addVariableAccess(variableAccess);
	} else {
		function.addVariableAccess(variableAccess);
		pair <string, Function> functionAccess (function.getPath() + function.getName(), function);
		functionsAccessed.insert(functionAccess);
	}
}

void FunctionAnalyzer::analyzeFunctions() {

	for(auto &functionName : functionsAccessed) {
		cout << functionName.second.getName() << ", count: " << functionName.second.getTimesCalled() << endl;

		cout << "seperate these variables into the following structs to minimize cache misses" << endl;

		int currentSize = 0;
		for(auto &variable : functionName.second.getVariablesAccessed()) {
			if(currentSize + variable.second.getSize() < 64) {
				currentSize += variable.second.getSize();
			} else {
				cout << "--------------------------------------------" << endl;
				currentSize = 0 + variable.second.getSize();
			}
			cout << "\t line: " << variable.second.getLine();
			cout << ", column: " << variable.second.getCol();
			cout << "\t - " << variable.second.getName() << endl;

		}
		cout << "--------------------------------------------" << endl;
	}
}
