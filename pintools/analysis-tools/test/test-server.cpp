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
static const char *spark_address = "localhost";
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
		printf("Could not create test server socket\n");
	}

	spark_server.sin_addr.s_addr = INADDR_ANY;
	spark_server.sin_family = AF_INET;
	spark_server.sin_port = htons(spark_port);

	int err;
    err = bind(sockfd, (struct sockaddr *)&spark_server, sizeof(spark_server));
    if (err < 0) {
        perror("Could not bind to localhost\n");
    }

    listen(sockfd, 1);
}

void TestServer::listenForLogs() {
    printf("Listening for logs...\n");
    logentry buf;
    int bytes;

    while (1) {
        //printf("Any bytes?\n");
        bytes = read(sockfd, &buf, sizeof(buf));
        printf("logentry: %d\n", buf.entry.access.col);
        if (bytes > 0) {
            printf("Received something!\n");
            if (buf.entry_type == LOG_ACCESS) {
                switch(buf.entry.access.col) {
                    case HIT:
                        printf("HIT\n");
                        break;
                    case MISS:
                        printf("MISS\n");
                        break;
                    default:
                        printf("NONE\n");
                        break;
                }
            }
        }
    }
}

void TestServer::exitSocket() {
    close(sockfd);
}
