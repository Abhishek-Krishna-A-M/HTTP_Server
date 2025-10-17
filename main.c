#include "launch.h"
#include "Server.h"
#include <arpa/inet.h>

int main() {
    struct Server server = server_constructor(
        AF_INET, SOCK_STREAM, 0,
        INADDR_ANY, 8080, 10,
        launch_server
    );

    // Start the server
    server.launch(&server);

    return 0;
}

