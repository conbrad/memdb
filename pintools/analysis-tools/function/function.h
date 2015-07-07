/*
 * function.h
 *
 *  Created on: Jul 5, 2015
 *      Author: conor
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include "variable-access.h"
#include <vector>
class Function {
private:
	std::string name;
	std::string path;
	std::vector<VariableAccess> variablesAccessed;

public:
	Function(std::string name, std::string path);
	std::string getName();
	std::string getPath();
	std::vector<VariableAccess> getVariablesAccessed();
	void addVariableAccess(VariableAccess variable);
};

#endif /* FUNCTION_H_ */
