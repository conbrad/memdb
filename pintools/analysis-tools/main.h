/*
 * main.h
 *
 *  Created on: Jun 11, 2015
 *      Author: conor
 */

#ifndef MAIN_H_
#define MAIN_H_

/* We need this in order to use the bitset class */
#define MAX_LINE_SIZE 64

extern int CACHE_LINE_SIZE;
extern int ASSOC;
extern bool WANT_RAW_OUTPUT;

#include "record/zero-reuse-record.h"
#include "record/low-util-record.h"

class Main {
public:
	static void addZeroReuseRecord(std::pair<std::string, ZeroReuseRecord> entry);
	static void addLowUtilRecord(std::pair<std::string, LowUtilRecord> entry);
};


#endif /* MAIN_H_ */
