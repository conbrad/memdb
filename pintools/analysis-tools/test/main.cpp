#include <csignal>
#include <iostream>
#include "test-server.h"

using namespace std;

TestServer testServer;

void signalHandler(int sigNum) {
    cout << "Closing socket..." << endl;
    testServer.exitSocket();

    exit(sigNum);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signalHandler);
    testServer.initSocket();
    testServer.listenForLogs();
}
