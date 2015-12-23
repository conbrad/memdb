/*
 * access-sender.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: conor
 */
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include "access-sender.h"

static int sockfd;
static struct sockaddr_in spark_server;
static const char *spark_address = "127.0.0.1.";
static uint16_t spark_port = 12345;
static const unsigned short HIT = 1;
static const unsigned short MISS = 0;

void AccessSender::initSocket() {
	sockfd = socket(AF_INET , SOCK_STREAM , 0);
	if (sockfd < 0) {
		printf("Could not create socket");
	}

	spark_server.sin_addr.s_addr = inet_addr(spark_address);
	spark_server.sin_family = AF_INET;
	spark_server.sin_port = htons(spark_port);

	int err;
    err = connect(sockfd, (struct sockaddr *)&spark_server, sizeof(spark_server));
    if (err < 0) {
        perror("Could not connect to spark server");
    }
}

// uses col field in entry.access to describe a hit access
void AccessSender::sendHit(accesslog access) {
	if(!(sockfd) || sockfd < 0) {
		initSocket();
	}
	access.col = HIT;
	send(sockfd, &access, sizeof(access), 0);
}

// uses col field in entry.access to describe a miss acess
void AccessSender::sendMiss(accesslog access) {
	if(!(sockfd) || sockfd < 0) {
			initSocket();
	}
	access.col = MISS;
	send(sockfd, &access, sizeof(access), 0);
}

void AccessSender::exitSocket() {
	// TODO need to send spark server eof?
	printf("Closing connection to spark server");
	close(sockfd);
}
