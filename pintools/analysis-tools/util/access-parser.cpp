#include "access-parser.h"
#include <sstream>
#include <vector>

using namespace std;

const string DELIM  = ":";

string AccessParser::functionNameFromPath(const string &accessPath) {
	string buffer;
    stringstream ss(accessPath);

    // the first word is the function name
    ss >> buffer;

    return buffer;
}

string AccessParser::variableNameFromPath(const string &varInfo) {
	string buffer;
    stringstream ss(varInfo);

    // the first word is the variable name
    ss >> buffer;

    return buffer;
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
