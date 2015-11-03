/*
 * FunctionAnalyzer.cpp
 *
 *  Created on: Jul 5, 2015
 *      Author: conor
 */

#include "variable-analyzer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <sys/time.h>

#include "../util/map-utils.h"

using namespace std;

map<string, VariableAccess> variables;
const string UNKNOWN_PATH = "<unknown>";

map<string, int> variableAccessCounts;

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
	addVariableMissCount(variableAccess);
}

void VariableAnalyzer::addVariableMissCount(VariableAccess variableAccess) {
	map<string, int>::iterator found = variableAccessCounts.find(
				variableAccess.getPath() +
				variableAccess.getName());

	// we wouldn't be called if path was UNKNOWN_PATH
	// so no guard check is needed

	if(found != variableAccessCounts.end()) {
		found->second++;
	} else {
		pair <string, int> variableCacheMiss (variableAccess.getPath() + variableAccess.getName(), 1);
		variableAccessCounts.insert(variableCacheMiss);
	}
}

void VariableAnalyzer::cacheMissesPerVariable(int maxVariables) {
	// names and opens accessCountFile based on timestamp
	ofstream accessCountFile;
	accessCountFile.open(getTimestamp() + "_access_count.txt");
	accessCountFile << "Listing of top "
					<< maxVariables
					<< " causing cache misses,"
					<< " there are " << variableAccessCounts.size()
					<< " total cache misses \n"<< endl;

	int i = 0;
	for(auto accessCount : mapUtils::flip_map(variableAccessCounts)) {
		accessCountFile << "-------------------------------------------" << endl;
		accessCountFile << "Variable: " << accessCount.second << "\n\n" << endl;
		accessCountFile << "Misses: " << accessCount.first << endl;
		accessCountFile << "------------------------------------------- \n" << endl;
		i++;
		if(i == maxVariables) {
			break;
		}
	}
	accessCountFile.close();
}

string VariableAnalyzer::getTimestamp() {
	struct timeval timestamp;
	gettimeofday(&timestamp, NULL);
	return std::to_string(timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000);
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
