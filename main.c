#include "include/Server.h"
#include "include/Launch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int port = 8080;
    char www_root[256] = "www";

    if (argc >= 2) port = atoi(argv[1]);
    if (argc >= 3) strncpy(www_root, argv[2], sizeof(www_root));

    printf("Server starting on port %d, www root: %s\n", port, www_root);

    struct Server server = server_constructor(
        AF_INET, SOCK_STREAM, 0,
        INADDR_ANY, port, 10,
        launch_server
    );

    server.launch(&server);

    return 0;
}
