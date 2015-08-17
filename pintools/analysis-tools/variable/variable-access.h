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
public:
	struct VariableDetails {
		int size;
		int line;
		int col;
		std::string name;
		std::string type;
		std::string path;
	};
	VariableAccess(VariableDetails variableDetails);
	int getSize();
	int getLine();
	int getCol();
	int getTimesCalled();
	std::string getName();
	std::string getType();
	std::string getPath();
	void called();
	virtual ~VariableAccess();

private:
	VariableDetails variableDetails;
	int timesCalled;
};

#endif /* VARIABLE_ACCESS_H_ */
