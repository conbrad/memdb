/*
 * function.h
 *
 *  Created on: Jul 5, 2015
 *      Author: conor
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include "variable-access.h"
#include <map>
class Function {
private:
	int timesCalled;
	std::string name;
	std::string path;
	std::string pathAndName;

public:
	Function(std::string name, std::string path);
	std::string getName();
	std::string getPath();
	std::string getPathAndName();
	std::map<std::string, VariableAccess> getVariablesAccessed();
	void addVariableAccess(VariableAccess variable);
	int getTimesCalled();
	void called();
};

#endif /* FUNCTION_H_ */
