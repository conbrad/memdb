/*
 * miss-sender.h
 *
 *  Created on: Nov 2, 2015
 *      Author: conor
 */

#ifndef MISS_SENDER_H_
#define MISS_SENDER_H_

#include <map>

enum class metric_type {
	COUNT,
	CACHE_HEAT,
	CLOSE
};

struct miss_data {
	size_t timestamp;
	size_t slice_length;
	metric_type type;
	std::map<int, std::string> data;
};

class MissSender {
public:
	MissSender();
	void sendMiss(miss_data data);
	virtual ~MissSender();
private:
	void initSocket();
	void exitSocket();
};

#endif /* ANALYSIS_TOOLS_UTIL_MISS_SENDER_H_ */
