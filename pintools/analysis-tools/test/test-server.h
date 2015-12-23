/*
 * test-server.h
 *
 *  Created on: Jun 9, 2015
 *      Author: conor
 */

#ifndef TEST_SERVER_H_
#define TEST_SERVER_H_

#include "../util/binaryinstrumentation.h"

class TestServer {
private:

public:
	TestServer();
    ~TestServer();
    void initSocket();
    void listenForLogs();
    void exitSocket();
};

#endif /* TEST_SERVER_H_ */
