/*
 * access-log-receiver.h
 *
 */

#ifndef ACCESS_LOG_RECEIVER_H_
#define ACCESS_LOG_RECEIVER_H_

#include "binaryinstrumentation.h"
#include <string>

class AccessLogReceiver {
private:
	AccessLogReceiver();
	static void initSocket();
	static void exitSocket();
public:
	AccessLogReceiver(std::string sock_path);
	static int sizeOf(accesslog access);
	static bool isEof();
	logentry readAccess();
};

#endif /* ACCESS_LOG_RECEIVER_H_ */
