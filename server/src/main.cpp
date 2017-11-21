#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"

using namespace std;

int main(void) {
    Server server(1024);
    server.setup();

    while (1) {
        server.listen();
    }

    return 0;
}
