/*
 * access-sender.h
 *
 *  Created on: Nov 28, 2015
 *      Author: conor
 */

#ifndef ACCESS_SENDER_H_
#define ACCESS_SENDER_H_

#include "binaryinstrumentation.h"

class AccessSender {
private:
	AccessSender(){};	// disallows instantiation
	static void initSocket();
	static void exitSocket();
public:
	static void sendHit(accesslog access);
	static void sendMiss(accesslog access);
};

#endif /* ACCESS_SENDER_H_ */
