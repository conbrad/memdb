/*
 * variable-access.h
 *
 *  Created on: Jul 5, 2015
 *      Author: conor
 */

#ifndef VARIABLE_ACCESS_H_
#define VARIABLE_ACCESS_H_

#include <string>

class VariableAccess {
private:
	int size;
	int line;
	int col;
	std::string name;
	std::string type;
public:
	VariableAccess(int size, int line, int col, std::string name, std::string type);
	int getSize();
	int getLine();
	int getCol();
	std::string getName();
	std::string getType();
	virtual ~VariableAccess();
};

#endif /* VARIABLE_ACCESS_H_ */
