/*
 * FunctionAnalyzer.cpp
 *
 *  Created on: Jul 5, 2015
 *      Author: conor
 */

#include "function-analyzer.h"
#include <iostream>

std::vector<Function> functionsAccessed;

using namespace std;

void FunctionAnalyzer::addFunction(Function function, VariableAccess variableAccess) {
	functionsAccessed.push_back(function);
}

void FunctionAnalyzer::analyzeFunctions() {
	for(Function function : functionsAccessed) {
		cout << function.getName() << endl;
	}
}
