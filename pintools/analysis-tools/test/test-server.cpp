#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include "test-server.h"

static int sockfd;
static struct sockaddr_in spark_server;
static const char *spark_address = "127.0.0.1.";
static uint16_t spark_port = 12345;
static const unsigned short HIT = 1;
static const unsigned short MISS = 0;

TestServer::TestServer() {

}

TestServer::~TestServer() {

}

void TestServer::initSocket() {
	sockfd = socket(AF_INET , SOCK_STREAM , 0);
	if (sockfd < 0) {
		printf("Could not create test server socket");
	}

	spark_server.sin_addr.s_addr = inet_addr(spark_address);
	spark_server.sin_family = AF_INET;
	spark_server.sin_port = htons(spark_port);

	int err;
    err = bind(sockfd, (struct sockaddr *)&spark_server, sizeof(spark_server));
    if (err < 0) {
        perror("Could not bind to localhost");
    }
}

void TestServer::listenForLogs() {
    printf("Listening for logs...\n");
}

void TestServer::exitSocket() {

}
