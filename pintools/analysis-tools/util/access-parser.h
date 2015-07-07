/*
 * access-parser.h
 *
 *  Created on: Jul 6, 2015
 *      Author: conor
 */

#ifndef ACCESS_PARSER_H_
#define ACCESS_PARSER_H_

#include <string>

class AccessParser {
private:
	AccessParser(){};		// disallow instantiation
public:
	static std::string functionNameFromPath(const std::string &accessPath);
	static std::string variableNameFromPath(const std::string &accessPath);
	static int lineNumFromPath(const std::string &accessPath);
	static int colNumFromPath(const std::string &accessPath);
};



#endif /* ACCESS_PARSER_H_ */
