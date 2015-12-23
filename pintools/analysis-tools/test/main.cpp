#include "test-server.h"

int main(int argc, char *argv[]) {
    TestServer *testServer = new TestServer();
    
    testServer->initSocket();
    testServer->listenForLogs();
    testServer->exitSocket();

    delete testServer;
}
