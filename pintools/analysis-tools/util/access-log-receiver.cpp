/*
 * AccessLogReceiver.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: conor
 */

#include "access-log-receiver.h"
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <string>
#include <iostream>

using namespace std;

static struct sockaddr_un client_address;
static struct sockaddr_un server_address;
static int sockfd;
static std::string socket_path;
static bool eof = false;

// TODO make user configurable
//static const char *CLIENT_PATH = "/home/work/llvm_log_client";
//static const char *SERVER_PATH = "server_trace_logs";

AccessLogReceiver::AccessLogReceiver(string sock_path) {
	socket_path = sock_path;
	initSocket();
}

void AccessLogReceiver::initSocket() {
    cout << "Initializing socket..." << endl;

    unlink(socket_path.c_str());

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Error opening socket\n");
    }
    //client_address.sun_family = AF_UNIX;
    //strcpy(client_address.sun_path, socket_path.c_str());
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, socket_path.c_str());

    int err;
    err = bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (err < 0) {
        perror("Could not bind to socket");
        exit(-1);
    }

    listen(sockfd, 1);
    cout << "Listening for trace logs..." << endl;
}

logentry AccessLogReceiver::readAccess() {
    logentry buf;
    int bytes;

    while (1) {
        bytes = read(sockfd, &buf, sizeof(buf));
        if (bytes > 0) {
	    //cout << "Processing trace logs" << endl;
            if (buf.entry_type == LOG_FN) {
                if (buf.entry.fn.functionId == -1) {
                    cout << "End of instrumentation trace..." << endl;
                    eof = true;
                    break;
                }
            }
            return buf;
        }
    }
    return buf;
}

int AccessLogReceiver::sizeOf(accesslog access) {
	switch(access.value_type) {
		case I8:
			return 8;
		case I16:
			return 16;
		case I32:
			return 32;
		case I64:
			return 64;
		case F32:
			return 32;
		case F64:
			return 64;
		case PTR:
			return sizeof(access.value);
		default:
			return 8;
	}
}

bool AccessLogReceiver::isEof() {
	return eof;
}

void AccessLogReceiver::exitSocket() {
    unlink(socket_path.c_str());
    close(sockfd);
}
