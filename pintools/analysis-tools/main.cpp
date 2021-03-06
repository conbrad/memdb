#include <iostream>
#include <fstream>
#include <unistd.h>
#include <iomanip>
#include <csignal>
#include "main.h"

#include "cache-waste-analysis.h"
#include "util/map-summarizer.h"
#include "util/binaryinstrumentation.h"
#include "util/access-log-receiver.h"
#include "waste-record-collection.h"
#include "main.h"

using namespace std;

CacheWasteAnalysis *cacheAnalyzer;
char *SOCKET_PATH = NULL;
string DEFAULT_TRACE_MAP_PATH = ".";

/* Default cache size parameters for a 2MB 4-way set associative cache */
int NUM_SETS = 8*1024;
int ASSOC = 4; /* 4-way set associative */
int CACHE_LINE_SIZE = 64;  /* in bytes */
bool WANT_RAW_OUTPUT = false;
bool EXIT_ON_EOF = false; /* exit once exit log is received */
bool SPARK_OFF = false; /* sends to spark server by default */

const char SOCKET_PATH_OPTION = 'p';
const char ASSOCIATIVITY_OPTION = 'a';
const char FILENAME_OPTION = 'f';
const char CACHE_LINE_SIZE_OPTION = 'l';
const char RAW_OUTPUT_OPTION = 'r';
const char NUM_CACHE_SET_OPTION = 's';
const char EXIT_OPTION = 'e';
const char SPARK_OFF_OPTION = 'o';

void parseInputOptions(int argc, char* argv[], char* fname);
void analyzeTrace();

void noSocketPathError();
void noInputFileError();
void openFileError(char* fname);
void optionErrorMessage();

void socketPathMessage();
void userAssociativityMessage(char* nptr);
void userLineSizeMessage(char* nptr);
void userCacheSetMessage(char* nptr);
void printRawOutputDetails();
void printStats();

void signalHandler(int sigNum) {
    cout << "Printing stats..." << endl;
    printStats();

    delete cacheAnalyzer;
    exit(sigNum);
}

int main(int argc, char *argv[]) {
    char *fileName = NULL;

    signal(SIGINT, signalHandler);
	parseInputOptions(argc, argv, fileName);
	analyzeTrace();
}

/* Right now we don't check that the number of sets and the cache line
 * size are a power of two, but we probably should.
 */
void parseInputOptions(int argc, char* argv[], char* fname) {
	char c;
	char *nptr;

	while ((c = getopt(argc, argv, "a:f:l:p:s:r:e:o")) != -1)
		switch (c) {
			case SOCKET_PATH_OPTION:
				SOCKET_PATH = optarg;
				socketPathMessage();
				break;
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
            case EXIT_OPTION:
                EXIT_ON_EOF = true;
                break;
            case SPARK_OFF_OPTION:
                SPARK_OFF = true;
                break;
			case '?':
			default:
				optionErrorMessage();
		}
	if(SOCKET_PATH == NULL) {
	    cout << "Please enter a socket path using -p to receive trace logs from." << endl;
	    exit(-1);
	}
}

void analyzeTrace() {
	ifstream traceFile;
	cacheAnalyzer =	new CacheWasteAnalysis(NUM_SETS, ASSOC, CACHE_LINE_SIZE);

	AccessLogReceiver accessLogReceiver(SOCKET_PATH);

	// Receive logs over socket from instrumentation tool
	logentry accessLog;
	while(1) {
		accessLog = accessLogReceiver.readAccess();

        if(accessLogReceiver.isEof()) {
            printStats();

            if(EXIT_ON_EOF) {
                break;
            }
        }

		if(accessLog.entry_type == LOG_ACCESS) {
			cacheAnalyzer->simulate(accessLog);
		}
	}
}

void printStats() {
    cout << "-------------------------------" << endl;
    cout << "Total bytes brought into cache:" << endl;
    cout << "-------------------------------" << endl;
    cacheAnalyzer->printTotalBytes();
    cout << "-------------------------------" << endl;
    cout << "Total bytes wasted:" << endl;
    cout << "-------------------------------" << endl;
    cacheAnalyzer->printWastedBytes();
    cacheAnalyzer->printNumAccesses();
    cout << "-------------------------------" << endl;
    cacheAnalyzer->printNumMisses();
    cout << "-------------------------------" << endl;
}

void noInputFileError() {
	cerr << "Please provide input trace file with the -f option." << endl;
	exit(-1);
}

void noSocketPathError() {
	cerr << "Please provide a socket path with the -p option." << endl;
	exit(-1);
}

void openFileError(char* fname) {
	cerr << "Failed to open file " << fname << endl;
	exit(-1);
}

void socketPathMessage() {
	cout << "Socket path set to " << SOCKET_PATH << endl;
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
