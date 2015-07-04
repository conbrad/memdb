//============================================================================
// Name        : convert2memtracker.cpp
// Author      : Conor
// Version     : 1.0
// Description : Converts llvm instrumentation tool format to memtracker format
//
//				 Currently expects map_functions.json, map_variables.json,
//				 map_types.json, map_sources.json and access.trace
//				 (as output by default from the LLVM tool) to be in the same
//				 folder.
//
// Dependencies: json11 - https://github.com/dropbox/json11.git
//============================================================================

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <map>
#include "json11.hpp"
#include <cassert>

using namespace std;

namespace {
	const char* MEMTRACKER_READ = "read: ";
	const char* MEMTRACKER_WRITE = "write: ";
	const char* LLVM_READ = "r";
	const char* LLVM_WRITE = "w";
	const char* UNKNOWN = "<unknown>";

	string CURRENT_DIR = ".";
	const string functionsJson = "map_functions.json";
	const string sourcesJson = "map_sources.json";
	const string typesJson = "map_types.json";
	const string variablesJson = "map_variables.json";
	const string accessTrace = "access.trace";

	const char memtrackerFile[] = "memtracker.trace";
	const char *accessTraceFile = accessTrace.c_str();
	ofstream memtrackerTrace (memtrackerFile);

	bool functionJsonFound = false;
	bool sourceJsonFound = false;
	bool typeJsonFound = false;
	bool variableJsonFound = false;
	bool accessTraceFound = false;

    struct IdMap {
        map<string, int> id_map;

        IdMap(string filename) {
            ifstream mapin (filename);
            if (!mapin.is_open()) {
                return;
            }
            string fname;
            stringstream ss;
            string err;
            ss << mapin.rdbuf();
            json11::Json jsmap;
            jsmap = json11::Json::parse(ss.str(), err);
            for (auto entry : jsmap.object_items()) {
                id_map[entry.first] = entry.second.int_value();
            }
        }

        string getKey(int value) {
        	for (auto it = id_map.begin(); it != id_map.end(); ++it) {
        	    if (it->second == value) {
        	        return it->first;
        	    }
        	}
        	return UNKNOWN;
        }
    };

	IdMap functionMap = IdMap(functionsJson);
	IdMap variableMap = IdMap(variablesJson);
	IdMap sourceMap = IdMap(sourcesJson);
	IdMap typeMap = IdMap(typesJson);
}

void convertLLVMTrace();
void parseLine(string line);
string parseFunction(string word);
string parseAccessSource(string word);
string parseVariable(string word);
string parseType(string word);

void inputError();
vector<string> getdir (string dir);
void checkForFiles();
bool allFound();
void notFoundError();
void fini();


int main(int argc, char* argv[]) {
	if(argc == 2) {
		CURRENT_DIR = argv[1];
	}
	if(argc > 2) {
		inputError();
	}
	checkForFiles();
	convertLLVMTrace();
	fini();
    return 0;
}

void convertLLVMTrace() {
	ifstream traceFile (accessTraceFile);
	ofstream memtrackerTrace (memtrackerFile);
	string line;
    if (traceFile.is_open()) {
        while (getline(traceFile, line)) {
        	parseLine(line);
        }
        memtrackerTrace.close();
        traceFile.close();
    } else {
        cout << "Unable to open " << accessTrace << endl;
    }
}

void parseLine(string line) {
	istringstream lineStream(line);
	string word;
    if(!lineStream.eof()) {
    	lineStream >> word;
		if (!(word.compare(LLVM_READ) == 0) && !(word.compare(LLVM_WRITE) == 0)) {
    		return;
    	}
		if (word.compare(LLVM_WRITE) == 0) {
			memtrackerTrace << MEMTRACKER_WRITE;
    	}
		if (word.compare(LLVM_READ) == 0) {
			memtrackerTrace << MEMTRACKER_READ;
    	}

    	int linePosition = 1;
    	while(lineStream) {
    		lineStream >> word;
    		switch(linePosition++) {
    			case 1:	memtrackerTrace << word << " ";
    				break;
    			case 2:	memtrackerTrace << word << " ";
    				break;
    			case 3: memtrackerTrace << word << " ";
    				break;
    			case 4: memtrackerTrace << parseFunction(word) << " ";
    				break;
    			case 5:	memtrackerTrace << parseAccessSource(word) << " ";
    				break;
    			case 6:	memtrackerTrace << parseVariable(word) << " ";
    			    break;
    			case 7:	memtrackerTrace << parseType(word) << " ";
    				break;
    		}
    	}
    	memtrackerTrace << "\n";
    }
}

string parseType(string word) {
	return typeMap.getKey(atoi(word.c_str()));
}

string parseVariable(string word) {
	return variableMap.getKey(atoi(word.c_str()));
}

string parseAccessSource(string word) {
	string delimiter = ":";
	string sourceId = word.substr(0, word.find(delimiter));
	string result = sourceMap.getKey(atoi(sourceId.c_str()));
	result += word.substr(word.find(delimiter), word.size() - 1);
	return result;
}

string parseFunction(string word) {
	return functionMap.getKey(atoi(word.c_str()));
}

/****************************************************************
 *
 * 						Helper Functions
 *
 ***************************************************************
 */

void inputError() {
	cerr << "Expects either a single argument to be the path where the LLVM JSON and trace logs exist," << endl;
	cerr << "or no arguments but the logs must be in the working directory" << endl << endl;
	cerr << "LLVM JSON files must use the default file name given by the tool: map_functions.json," << endl;
	cerr << "map_sources.json, map_types.json, map_variables.json" << endl << endl;
	cerr << "The access trace must use the default file name too: access.trace" << endl;
	exit(-1);
}

void checkForFiles() {
    string dir = string(CURRENT_DIR);
    vector<string> files = getdir(dir);

    for (unsigned int i = 0;i < files.size();i++) {
    	if (files[i].compare(functionsJson) == 0) {
    		functionJsonFound = true;

    		if(allFound()) {
    			break;
    		}
    	}
    	if(files[i].compare(sourcesJson) == 0) {
    		sourceJsonFound = true;

    		if(allFound()) {
    			break;
    		}
    	}
    	if(files[i].compare(typesJson) == 0) {
    		typeJsonFound = true;

    		if(allFound()) {
    			break;
    		}
    	}
    	if(files[i].compare(variablesJson) == 0) {
    		variableJsonFound = true;

    		if(allFound()) {
    			break;
    		}
    	}
    	if(files[i].compare(accessTrace) == 0) {
    		accessTraceFound = true;

    		if(allFound()) {
    			break;
    		}
    	}
    }

    if(!allFound()) {
		notFoundError();
    }
}

vector<string> getdir (string dir) {
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        exit(-1);
    }

    vector<string> files;
    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return files;
}

bool allFound() {
	return functionJsonFound &&
		   sourceJsonFound &&
		   typeJsonFound &&
		   variableJsonFound &&
		   accessTraceFound;
}

void notFoundError() {
	cerr << "Did not find all json files:" << endl;
	if (!functionJsonFound) {
		cerr << functionsJson << " missing" << endl;
	}
	if (!sourceJsonFound) {
		cerr << sourcesJson << " missing" << endl;
	}
	if (!variableJsonFound) {
		cerr << variablesJson << " missing" << endl;
	}
	if (!typeJsonFound) {
		cerr << typesJson << " missing" << endl;
	}
	if(!accessTraceFound) {
		cerr << accessTrace << " missing" << endl;
	}
	exit(-1);
}

void fini() {
	cout << "Finished conversion, memtracker trace in: " << memtrackerFile
			<< endl;
}
