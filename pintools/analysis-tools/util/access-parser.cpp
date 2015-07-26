#include "access-parser.h"
#include <sstream>
#include <vector>

using namespace std;

const string DELIM  = ":";
const int ACTUAL_PATH_OFFSET = 2;

string AccessParser::functionNameFromPath(const string &accessPath) {
	string buffer;
    stringstream ss(accessPath);

    // the first word is the function name
    ss >> buffer;

    return buffer;
}

string AccessParser::variableNameFromInfo(const string &varInfo) {
	string buffer;
    stringstream ss(varInfo);

    // the second word is the variable type
    ss >> buffer;

    return buffer;
}

string AccessParser::typeFromInfo(const string &varInfo) {
	string buffer;
    stringstream ss(varInfo);

    // the first word is the variable name
    ss >> buffer;
    ss >> buffer;

    return buffer;
}

string AccessParser::getNthWord(const string &input, int numWordsToSkip) {
	string buffer;
	stringstream ss(input);

	for(int i = 0; i < numWordsToSkip; i++) {
		ss >> buffer;
	}

	return buffer;
}

string AccessParser::stripLineNumbers(const string &accessPath) {
	string actualPath = getNthWord(accessPath, ACTUAL_PATH_OFFSET);

	auto start = 0U;

	// find first instance of ':'
    auto end = actualPath.find(DELIM);

    string result = actualPath.substr(start, end - start);
    return result;
}

int AccessParser::lineNumFromPath(const string &accessPath) {

    // find first instance of ':'
    auto end = accessPath.find(DELIM);

    // second substring will be the line
    auto start = end + DELIM.length();
    end = accessPath.find(DELIM, start);

    string lineNumber = accessPath.substr(start, end - start);
    return atoi(lineNumber.c_str());
}

int AccessParser::colNumFromPath(const string &accessPath) {

    // find first instance of ':'
	auto start = 0U;
    auto end = accessPath.find(DELIM);

    // third substring will be the column
    for(int i=0; i < 2; i++) {
        start = end + DELIM.length();
        end = accessPath.find(DELIM, start);
    }

    string lineNumber = accessPath.substr(start, end - start);
    return atoi(lineNumber.c_str());
}
