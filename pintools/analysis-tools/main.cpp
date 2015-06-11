#include <iostream>
#include <fstream>
#include <unistd.h>
#include "main.h"

#include "cache-waste-analysis.h"

using namespace std;

/* Default cache size parameters for a 2MB 4-way set associative cache */
int NUM_SETS = 8*1024;
int ASSOC = 4; /* 4-way set associative */
int CACHE_LINE_SIZE = 64;  /* in bytes */
bool WANT_RAW_OUTPUT = false;

const char ASSOCIATIVITY_OPTION = 'a';
const char FILENAME_OPTION = 'f';
const char CACHE_LINE_SIZE_OPTION = 'l';
const char RAW_OUTPUT_OPTION = 'r';
const char NUM_CACHE_SET_OPTION = 's';

char* parseInputOptions(int argc, char* argv[], char* fname);
void analyzeTrace(char* fileName);

void noInputFileError();
void openFileError(char* fname);
void optionErrorMessage();

void userAssociativityMessage(char* nptr);
void userLineSizeMessage(char* nptr);
void userCacheSetMessage(char* nptr);

int main(int argc, char *argv[]) {
    char *fileName = NULL;

	fileName = parseInputOptions(argc, argv, fileName);
	analyzeTrace(fileName);
//    /* Print the waste maps */
//    cout << "*************************************************" << endl;
//    cout << "               ZERO REUSE MAP                    " << endl;
//    cout << "*************************************************" << endl;
//
//    for(auto it = zeroReuseMap.begin(); it != zeroReuseMap.end(); it++) {
//        cout << it->first << endl;
//        cout << (ZeroReuseRecord&) it->second << endl;
//    }
//
//    cout << endl;
//
//    /* Print the waste maps */
//    cout << "*************************************************" << endl;
//    cout << "               LOW UTILIZATION MAP               " << endl;
//    cout << "*************************************************" << endl;
//
//    for(auto it = lowUtilMap.begin(); it != lowUtilMap.end(); it++) {
//        cout << it->first << endl;
//        cout << it->second << endl;
//    }
//
//    summarizeMap<ZeroReuseRecord>(zeroReuseMap, groupedZeroReuseMap);
//    cout << "*************************************************" << endl;
//    cout << "         ZERO REUSE MAP SUMMARIZED               " << endl;
//    cout << "*************************************************" << endl;
//    printSummarizedMap<ZeroReuseRecord>(groupedZeroReuseMap);
//
//    summarizeMap<LowUtilRecord>(lowUtilMap, groupedLowUtilMap);
//    cout << endl;
//    cout << "*************************************************" << endl;
//    cout << "         LOW UTILIZATION MAP SUMMARIZED          " << endl;
//    cout << "*************************************************" << endl;
//    printSummarizedMap<LowUtilRecord>(groupedLowUtilMap);
}

/* Right now we don't check that the number of sets and the cache line
 * size are a power of two, but we probably should.
 */
char* parseInputOptions(int argc, char* argv[], char* fname) {
	char c;
	char *nptr;

	while ((c = getopt(argc, argv, "a:f:l:s:r")) != -1)
		switch (c) {
		case ASSOCIATIVITY_OPTION:
			ASSOC = (int) (strtol(optarg, &nptr, 10));
			userAssociativityMessage(nptr);
			break;
		case FILENAME_OPTION:
			fname = optarg;
			break;
		case CACHE_LINE_SIZE_OPTION:
			CACHE_LINE_SIZE = strtol(optarg, &nptr, 10);
			userLineSizeMessage(nptr);
			break;
		case RAW_OUTPUT_OPTION:
			WANT_RAW_OUTPUT = true;
			break;
		case NUM_CACHE_SET_OPTION:
			NUM_SETS = (int) (strtol(optarg, &nptr, 10));
			userCacheSetMessage(nptr);
			break;
		case '?':
		default:
			optionErrorMessage();
		}
	if (fname == NULL) {
		noInputFileError();
	}
	return fname;
}

void analyzeTrace(char* fileName) {
	ifstream traceFile;
	CacheWasteAnalysis *cacheAnalyzer =
			new CacheWasteAnalysis(NUM_SETS, ASSOC, CACHE_LINE_SIZE);

	traceFile.open(fileName);
	if (!traceFile.is_open()) {
		openFileError(fileName);
	}
	string line;
	while (!traceFile.eof()) {
		getline(traceFile, line);
		cacheAnalyzer->parseAndSimulate(line);
	}
	traceFile.close();

	delete cacheAnalyzer;
}

void noInputFileError() {
	cerr << "Please provide input trace file with the -f option." << endl;
	exit(-1);
}

void openFileError(char* fname) {
	cerr << "Failed to open file " << fname << endl;
	exit(-1);
}

void userAssociativityMessage(char* nptr) {
	if (nptr == optarg && ASSOC == 0) {
		cerr << "Invalid argument for associativity: " << optarg << endl;
		exit(-1);
	} else {
		cout << "Associativity set to " << ASSOC << endl;
	}
}

void userLineSizeMessage(char* nptr) {
	if (nptr == optarg && CACHE_LINE_SIZE == 0) {
		cerr << "Invalid argument for the cache line size: " << optarg << endl;
		exit(-1);
	} else {
		cout << "Associativity set to " << CACHE_LINE_SIZE << endl;
	}
}

void userCacheSetMessage(char* nptr) {
	if (nptr == optarg && NUM_SETS == 0) {
		cerr << "Invalid argument for the number of cache sets: " << optarg
				<< endl;
		exit(-1);
	} else {
		cout << "Number of cache sets set to " << NUM_SETS << endl;
	}
}

void optionErrorMessage() {
	cerr << "Unknown option or missing option argument." << endl;
	exit(-1);
}