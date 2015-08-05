/*
 * function-variable-analyzer.h
 *
 *  Created on: Jul 5, 2015
 *      Author: conor
 */

#ifndef FUNCTION_ANALYZER_H_
#define FUNCTION_ANALYZER_H_

#include "variable-access.h"

class VariableAnalyzer {
private:
	VariableAnalyzer(){}; 	// disallows instantiation
	static void structify(std::pair<const std::string, VariableAccess> &function);
	static std::string parseType(const std::string loggedType);
public:
	static void addVariable(VariableAccess variableAccess);
	static void analyzeVariables();
};

#endif /* FUNCTION_ANALYZER_H_ */
