/*
 * miss-sender.h
 *
 *  Created on: Nov 2, 2015
 *      Author: conor
 */

#ifndef MISS_SENDER_H_
#define MISS_SENDER_H_

#include <map>
#include "json11.hpp"
#include "binaryinstrumentation.h"

enum class metric_type {
	COUNT,
	CACHE_HEAT,
	CLOSE
};

struct miss_data {
	size_t timestamp;
	size_t slice_length;
	metric_type type;
	std::map<int, logentry> data;
};

// TODO make user configurable
static const char *CLIENT_PATH = "client_trace_logs";
static const char *SERVER_PATH = "server_trace_logs";


class MissSender {
public:
	static void sendMiss(miss_data data);
private:
	MissSender(){};	// disallows instantiation
	static void initSocket();
	static void exitSocket();
};

#endif /* MISS_SENDER_H_ */
