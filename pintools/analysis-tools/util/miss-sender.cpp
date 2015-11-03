/*
 * MissSender.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: conor
 */

#include "miss-sender.h"
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>

using namespace std;
int sockfd;
struct sockaddr_un client_address;
struct sockaddr_un server_address;

// TODO make user configurable
const char *CLIENT_PATH = "client_trace_logs";
const char *SERVER_PATH = "server_trace_logs";


MissSender::MissSender() {
	initSocket();
}

void MissSender::initSocket() {
    printf("Initializing socket");
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Error opening socket\n");
    }
    client_address.sun_family = AF_UNIX;
    strcpy(client_address.sun_path, CLIENT_PATH);
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, SERVER_PATH);

    int err;
    err = bind(sockfd, (struct sockaddr *)&client_address, sizeof(client_address));
    if (err < 0) {
        perror("bind");
    }
    err = connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (err < 0) {
        perror("Could not connect to socket");
    }
}

void MissSender::sendMiss(miss_data data) {
	send(sockfd, &data, sizeof(data), 0);
}

void MissSender::exitSocket() {
    printf("Sending exit log");

    miss_data endData;
    endData.type = metric_type::CLOSE;

    send(sockfd, &endData, sizeof(endData), 0);

    unlink(CLIENT_PATH);
    printf("Closing socket %s...\n", SERVER_PATH);
    close(sockfd);
}

MissSender::~MissSender() {
	exitSocket();
}

