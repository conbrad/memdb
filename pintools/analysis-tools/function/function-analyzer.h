/*
 * function-variable-analyzer.h
 *
 *  Created on: Jul 5, 2015
 *      Author: conor
 */

#ifndef FUNCTION_ANALYZER_H_
#define FUNCTION_ANALYZER_H_

#include "function.h"

class FunctionAnalyzer {
private:
	FunctionAnalyzer(){}; 	// disallows instantiation
	static std::vector<Function> functionsAccessed;
public:
	void addFunction(Function function, VariableAccess variableAccess);
	void analyzeFunctions();
};

#endif /* FUNCTION_ANALYZER_H_ */
