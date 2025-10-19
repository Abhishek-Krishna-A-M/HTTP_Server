#include "./include/Server.h"
#include "./include/Launch.h"
#include "./include/Config.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    struct Server server;

    // Load config
    struct ServerConfig config = load_config("server.json");

    // Initialize server
    server.port = config.port;
    server.config = &config;
    server.domain = AF_INET;
    server.service = SOCK_STREAM;
    server.protocol = 0;
    server.interface = INADDR_ANY;
    server.backlog = 10;
    server.launch = launch_server;

    server.socket = socket(server.domain, server.service, server.protocol);
    if (server.socket < 0) { perror("Socket failed"); return 1; }

    server.address.sin_family = server.domain;
    server.address.sin_port = htons(server.port);
    server.address.sin_addr.s_addr = htonl(server.interface);

    if (bind(server.socket, (struct sockaddr*)&server.address, sizeof(server.address)) < 0) {
        perror("Bind failed"); return 1;
    }

    if (listen(server.socket, server.backlog) < 0) {
        perror("Listen failed"); return 1;
    }

    // Launch server
    server.launch(&server);

    return 0;
}
